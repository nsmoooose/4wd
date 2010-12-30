
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
	// m_body = new DynamicModel("4wd.osga/models/hmmwv.ive", btScalar(800), true);
	m_body = new DynamicBox(osg::Vec3(0.4f, 0.4f, 0.8f), 800);
	// m_body = new DynamicSphere(2.0, 300);
	m_wheel_front_left = new DynamicBox(osg::Vec3(0.15, 0.55, 0.55), 5);
	m_wheel_front_left->setPosition(-1, 2, 0);
	// m_wheel_front_left = new DynamicCylinder(0.5, 0.30, 9.0);
	m_wheel_front_right = new DynamicCylinder(0.5, 0.30, 9.0);
	m_wheel_front_right->setPosition(1, 2, 0);
	m_wheel_back_left = new DynamicCylinder(0.5, 0.30, 9.0);
	m_wheel_back_left->setPosition(-1, -2, 0);
	m_wheel_back_right = new DynamicCylinder(0.5, 0.30, 9.0);
	m_wheel_back_right->setPosition(1, -2, 0);

	btVector3 parentAxis(0.f, 0.f, 1.f);
	btVector3 childAxis(1.f, 0.f, 0.f);
	btVector3 anchor(0.f, 0.f, 0.f);
	m_wheel_front_left_c = new btHinge2Constraint(
		*m_body->getBody(), *m_wheel_front_left->getBody(), anchor, parentAxis, childAxis);
	m_wheel_front_left_c->setUpperLimit(0.f);
	m_wheel_front_left_c->setLowerLimit(0.f);

	anchor = btVector3(0.f, 0.f, 0.f);
	m_wheel_front_right_c = new btHinge2Constraint(
		*m_body->getBody(), *m_wheel_front_right->getBody(), anchor, parentAxis, childAxis);
	m_wheel_front_right_c->setUpperLimit(0.f);
	m_wheel_front_right_c->setLowerLimit(0.f);

	anchor = btVector3(0.f, 0.f, 0.f);
	m_wheel_back_left_c = new btHinge2Constraint(
		*m_body->getBody(), *m_wheel_back_left->getBody(), anchor, parentAxis, childAxis);
	m_wheel_back_left_c->setUpperLimit(0.f);
	m_wheel_back_left_c->setLowerLimit(0.f);

	anchor = btVector3(0.f, 0.f, 0.f);
	m_wheel_back_right_c = new btHinge2Constraint(
		*m_body->getBody(), *m_wheel_back_right->getBody(), anchor, parentAxis, childAxis);
	m_wheel_back_right_c->setUpperLimit(0.f);
	m_wheel_back_right_c->setLowerLimit(0.f);


#if 0

/// DOF index used in enableSpring() and setStiffness() means:
/// 0 : translation X
/// 1 : translation Y
/// 2 : translation Z
/// 3 : rotation X (3rd Euler rotational around new position of X axis, range [-PI+epsilon, PI-epsilon] )
/// 4 : rotation Y (2nd Euler rotational around new position of Y axis, range [-PI/2+epsilon, PI/2-epsilon] )
/// 5 : rotation Z (1st Euler rotational around Z axis, range [-PI+epsilon, PI-epsilon] )
/*
  Rotation
	X == wheel is rotating with the speed of the vehicle.
	Y == locked
	Z == turn left or right. Should have a limitation here.

  Translation
    X == locked
    Y == locked
    Z == Use spring to damp.
*/

    btHinge2Constraint(btRigidBody& rbA, btRigidBody& rbB, btVector3& anchor,
					   btVector3& axis1, btVector3& axis2);
	void setUpperLimit(btScalar ang1max) {
		setAngularUpperLimit(btVector3(-1.f, 0.f, ang1max));
	}

	void setLowerLimit(btScalar ang1min) {
		setAngularLowerLimit(btVector3( 1.f, 0.f, ang1min));
	}
#endif
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

	dynamics->addRigidBody(m_wheel_front_left->getBody());
	world->getRoot()->addChild(m_wheel_front_left->getNode());
	dynamics->addRigidBody(m_wheel_front_right->getBody());
	world->getRoot()->addChild(m_wheel_front_right->getNode());

	dynamics->addRigidBody(m_wheel_back_left->getBody());
	world->getRoot()->addChild(m_wheel_back_left->getNode());
	dynamics->addRigidBody(m_wheel_back_right->getBody());
	world->getRoot()->addChild(m_wheel_back_right->getNode());

	world->getDynamics()->addConstraint(m_wheel_front_left_c, true);
	world->getDynamics()->addConstraint(m_wheel_front_right_c, true);
	world->getDynamics()->addConstraint(m_wheel_back_left_c, true);
	world->getDynamics()->addConstraint(m_wheel_back_right_c, true);
}
