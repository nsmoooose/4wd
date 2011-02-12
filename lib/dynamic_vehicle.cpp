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
	// m_body = new DynamicModel("4wd.osga/models/hmmwv.ive", btScalar(800), true);
	m_body = new DynamicBox(osg::Vec3(0.8f, 2.2f, 0.1f), 80);
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

	createRearAxle();
	createFrontAxle();
}

void DynamicVehicle::createFrontAxle() {
	btScalar wheel_radius = 0.5;
	btScalar wheel_width = 0.3;
	btCylinderShape* wheel = new btCylinderShapeZ(btVector3(wheel_radius, wheel_radius, wheel_width/2));

	btScalar axle_radius = 0.05;
	btScalar axle_length = 2.0;
	btCylinderShape* axle = new btCylinderShapeZ(btVector3(axle_radius, axle_radius, axle_length/2));

	btQuaternion axle_quat = btQuaternion::getIdentity();
	axle_quat.setRotation(btVector3(0, 1, 0), 3.14/2);

	btScalar axle_compound_mass = 100.0;
	btCompoundShape* axle_compound = new btCompoundShape();
	btTransform axle_trans = btTransform::getIdentity();
	axle_trans.setRotation(axle_quat);
	axle_compound->addChildShape(axle_trans, axle);

	btTransform wheel_l_trans = btTransform::getIdentity();
	wheel_l_trans.setOrigin(btVector3(-1, 0, 0));
	wheel_l_trans.setRotation(axle_quat);
	axle_compound->addChildShape(wheel_l_trans, wheel);

	btTransform wheel_r_trans = btTransform::getIdentity();
	wheel_r_trans.setOrigin(btVector3(1, 0, 0));
	wheel_r_trans.setRotation(axle_quat);
	axle_compound->addChildShape(wheel_r_trans, wheel);

	btVector3 axle_inertia(0, 0, 0);
	axle_compound->calculateLocalInertia(axle_compound_mass, axle_inertia);
	btTransform axle_and_wheel_trans = btTransform::getIdentity();
	axle_and_wheel_trans.setOrigin(btVector3(0, 2, -2));
    btRigidBody::btRigidBodyConstructionInfo rb(
		axle_compound_mass,
		new btDefaultMotionState(axle_and_wheel_trans),
		axle_compound,
		axle_inertia);
	front_axle_body = new btRigidBody(rb);
	front_axle_body->setFriction(100);
	front_axle_body->setActivationState(DISABLE_DEACTIVATION);

	btVector3 parentAxis(0.f, 0.f, 1.f);
	btVector3 childAxis(1.f, 0.f, 0.f);
	btVector3 left_position(-1, 2, -2);
	front_axle_spring_left = new btHinge2Constraint(
		*m_body->getBody(), *front_axle_body, left_position, parentAxis, childAxis);
	front_axle_spring_left->setUpperLimit(0.f);
	front_axle_spring_left->setLowerLimit(0.f);
	front_axle_spring_left->setLimit(4, -1, 1);

	btVector3 right_position(1, 2, -2);
	front_axle_spring_right = new btHinge2Constraint(
		*m_body->getBody(), *front_axle_body, right_position, parentAxis, childAxis);
	front_axle_spring_right->setUpperLimit(0.f);
	front_axle_spring_right->setLowerLimit(0.f);
	front_axle_spring_right->setLimit(4, -1, 1);
}

void DynamicVehicle::createRearAxle() {
	btScalar wheel_radius = 0.5;
	btScalar wheel_width = 0.3;
	btCylinderShape* wheel = new btCylinderShapeZ(btVector3(wheel_radius, wheel_radius, wheel_width/2));

	btScalar axle_radius = 0.05;
	btScalar axle_length = 2.0;
	btCylinderShape* axle = new btCylinderShapeZ(btVector3(axle_radius, axle_radius, axle_length/2));

	btQuaternion axle_quat = btQuaternion::getIdentity();
	axle_quat.setRotation(btVector3(0, 1, 0), 3.14/2);

	btScalar axle_compound_mass = 100.0;
	btCompoundShape* axle_compound = new btCompoundShape();
	btTransform axle_trans = btTransform::getIdentity();
	axle_trans.setRotation(axle_quat);
	axle_compound->addChildShape(axle_trans, axle);

	btTransform wheel_l_trans = btTransform::getIdentity();
	wheel_l_trans.setOrigin(btVector3(-1, 0, 0));
	wheel_l_trans.setRotation(axle_quat);
	axle_compound->addChildShape(wheel_l_trans, wheel);

	btTransform wheel_r_trans = btTransform::getIdentity();
	wheel_r_trans.setOrigin(btVector3(1, 0, 0));
	wheel_r_trans.setRotation(axle_quat);
	axle_compound->addChildShape(wheel_r_trans, wheel);

	btVector3 axle_inertia(0, 0, 0);
	axle_compound->calculateLocalInertia(axle_compound_mass, axle_inertia);
	btTransform axle_and_wheel_trans = btTransform::getIdentity();
	axle_and_wheel_trans.setOrigin(btVector3(0, -2, -2));
    btRigidBody::btRigidBodyConstructionInfo rb(
		axle_compound_mass,
		new btDefaultMotionState(axle_and_wheel_trans),
		axle_compound,
		axle_inertia);
	rear_axle_body = new btRigidBody(rb);
	rear_axle_body->setFriction(100);
	rear_axle_body->setActivationState(DISABLE_DEACTIVATION);

	btVector3 parentAxis(0.f, 0.f, 1.f);
	btVector3 childAxis(1.f, 0.f, 0.f);
	btVector3 left_position(-1, -2, -2);
	rear_axle_spring_left = new btHinge2Constraint(
		*m_body->getBody(), *rear_axle_body, left_position, parentAxis, childAxis);
	rear_axle_spring_left->setUpperLimit(0.f);
	rear_axle_spring_left->setLowerLimit(0.f);
	rear_axle_spring_left->setLimit(4, -1, 1);

	btVector3 right_position(1, -2, -2);
	rear_axle_spring_right = new btHinge2Constraint(
		*m_body->getBody(), *rear_axle_body, right_position, parentAxis, childAxis);
	rear_axle_spring_right->setUpperLimit(0.f);
	rear_axle_spring_right->setLowerLimit(0.f);
	rear_axle_spring_right->setLimit(4, -1, 1);
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
	dynamics->addRigidBody(rear_axle_body);
	dynamics->addRigidBody(front_axle_body);
	dynamics->addConstraint(front_axle_spring_left, true);
	dynamics->addConstraint(front_axle_spring_right, true);
	dynamics->addConstraint(rear_axle_spring_left, true);
	dynamics->addConstraint(rear_axle_spring_right, true);
	world->getRoot()->addChild(m_body->getNode());
}

std::string DynamicVehicle::toString() {
	// btTransform transform = m_wheels[0]->getBody()->getWorldTransform();
	// btQuaternion quat = transform.getRotation();
	// btScalar angle = quat.getAngle();
	// btVector3 axis = quat.getAxis();

	std::stringstream str;
//	str << "Angle: " << angle << " Axis: (" << axis[0] << ", " << axis[1] << ", " << axis[2] << ")" << std::ends;
	return str.str();
}

void DynamicVehicle::turnLeft() {
	// for(int i=0;i<2;++i) {
	// 	m_wheels[i]->setRotation(-0.5, 0.0, 1.0, 1.0);
	// 	m_wheels_c[i]->setEquilibriumPoint();
	// }
}

void DynamicVehicle::turnRight() {
	// for(int i=0;i<2;++i) {
	// 	m_wheels[i]->setRotation(0.5, 0.0, 0.0, 1.0);
	// }
}

void DynamicVehicle::addTorque() {
	rear_axle_body->applyTorque(btVector3(-1000, 0, 0));
	// m_wheels[0]->getBody()->applyTorque(btVector3(-1000, 0, 0));
	// m_wheels[1]->getBody()->applyTorque(btVector3(-1000, 0, 0));
}

void DynamicVehicle::removeTorque() {
	rear_axle_body->applyTorque(btVector3(1000, 0, 0));
	// m_wheels[0]->getBody()->applyTorque(btVector3(1000, 0, 0));
	// m_wheels[1]->getBody()->applyTorque(btVector3(1000, 0, 0));
}
