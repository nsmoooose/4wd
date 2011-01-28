#include <sstream>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include "dynamic_box.h"
#include "dynamic_cylinder.h"
#include "dynamic_model.h"
#include "dynamic_sphere.h"
#include "dynamic_vehicle.h"
#include "motion_state.h"
#include "world.h"

DynamicVehicle::DynamicVehicle() {
	btCompoundShape* compound = new btCompoundShape();

	// m_body = new DynamicModel("4wd.osga/models/hmmwv.ive", btScalar(800), true);
	m_body = new DynamicBox(osg::Vec3(0.8f, 2.2f, 0.1f), 800);
	m_body->getBody()->setActivationState(DISABLE_DEACTIVATION);



	/*
	 * 0 == front left
	 * 1 == front right
	 * 2 == back left
	 * 3 == back right
	 */

	/*
	  DOF index used in enableSpring() and setStiffness() means:
	  0 : translation X
	  1 : translation Y
	  2 : translation Z
	  3 : rotation X (3rd Euler rotational around new position of X axis, range [-PI+epsilon, PI-epsilon] )
	  4 : rotation Y (2nd Euler rotational around new position of Y axis, range [-PI/2+epsilon, PI/2-epsilon] )
	  5 : rotation Z (1st Euler rotational around Z axis, range [-PI+epsilon, PI-epsilon] )

	  Rotation
	  X == wheel is rotating with the speed of the vehicle.
	  Y == locked
	  Z == turn left or right. Should have a limitation here.

	  Translation
	  X == locked
	  Y == locked
	  Z == Use spring to damp.
	*/

	btVector3 positions[4] = {
		btVector3(-1.f, 2.f, -2.f),
		btVector3(1.f, 2.f, -2.f),
		btVector3(-1.f, -2.f, -2.f),
		btVector3(1.f, -2.f, -2.f)
	};

	for(int i=0;i<4;++i) {
		DynamicObject* wheel = new DynamicCylinder(0.5, 0.30, 9.0);
		wheel->setRotation(3.14/2, 0.0, 1.0, 0.0);
		wheel->setPosition(positions[i][0], positions[i][1], positions[i][2]);
		wheel->getBody()->setFriction(100.0f);
		wheel->getBody()->setActivationState(DISABLE_DEACTIVATION);

		btVector3 parentAxis(0.f, 0.f, 1.f);
		btVector3 childAxis(1.f, 0.f, 0.f);
		btHinge2Constraint* constraint = new btHinge2Constraint(
			*m_body->getBody(), *wheel->getBody(), positions[i], parentAxis, childAxis);
		if(i >= 0) {
			constraint->setUpperLimit(0.f);
			constraint->setLowerLimit(0.f);
		}
		else {
			constraint->setUpperLimit(0.f);
			constraint->setLowerLimit(1.f);
		}
		constraint->setStiffness(2, 2000);
		constraint->setDamping(2, 2000);
		constraint->setEquilibriumPoint();

		m_wheels[i] = wheel;
		m_wheels_c[i] = constraint;
	}
}

btRigidBody *DynamicVehicle::getBody() {
	return m_body->getBody();
}

osg::MatrixTransform *DynamicVehicle::getNode() {
	return m_body->getNode();
}

void DynamicVehicle::addToWorld(World* world) {
	btDynamicsWorld* dynamics = world->getDynamics();

	dynamics->addRigidBody(m_body->getBody());
	world->getRoot()->addChild(m_body->getNode());

	for(int i=0;i<4;++i) {
		dynamics->addRigidBody(m_wheels[i]->getBody());
		world->getRoot()->addChild(m_wheels[i]->getNode());
		world->getDynamics()->addConstraint(m_wheels_c[i], true);
	}
}

std::string DynamicVehicle::toString() {
	btTransform transform = m_wheels[0]->getBody()->getWorldTransform();
	btQuaternion quat = transform.getRotation();
	btScalar angle = quat.getAngle();
	btVector3 axis = quat.getAxis();

	std::stringstream str;
	str << "Angle: " << angle << " Axis: (" << axis[0] << ", " << axis[1] << ", " << axis[2] << ")" << std::ends;
	return str.str();
}

void DynamicVehicle::turnLeft() {
	for(int i=0;i<2;++i) {
		m_wheels[i]->setRotation(-0.5, 0.0, 1.0, 1.0);
		m_wheels_c[i]->setEquilibriumPoint();
	}
}

void DynamicVehicle::turnRight() {
	for(int i=0;i<2;++i) {
		m_wheels[i]->setRotation(0.5, 0.0, 0.0, 1.0);
	}
}

void DynamicVehicle::addTorque() {
	m_wheels[0]->getBody()->applyTorque(btVector3(-1000, 0, 0));
	m_wheels[1]->getBody()->applyTorque(btVector3(-1000, 0, 0));
}

void DynamicVehicle::removeTorque() {
	m_wheels[0]->getBody()->applyTorque(btVector3(1000, 0, 0));
	m_wheels[1]->getBody()->applyTorque(btVector3(1000, 0, 0));
}
