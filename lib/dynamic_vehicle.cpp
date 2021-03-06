#include <sstream>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include "dynamic_box.h"
#include "dynamic_cylinder.h"
#include "dynamic_model.h"
#include "dynamic_sphere.h"
#include "dynamic_vehicle.h"
#include "motion_state.h"
#include "shapes.h"
#include "world.h"

DynamicVehicle::DynamicVehicle() {
	// m_body = new DynamicModel("4wd.osga/models/hmmwv.ive", btScalar(800), true);
	m_body = new DynamicBox(osg::Vec3(0.8f, 2.2f, 0.1f), 80);
	m_body->getBody()->setActivationState(DISABLE_DEACTIVATION);
	m_body->getBody()->setDamping(0.95, 0.95);

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

	front_axle_geometry = new osg::MatrixTransform();
	front_axle_geometry->setMatrix(osg::Matrix::translate(osg::Vec3(0, 2, -1)));

	osg::ref_ptr<osg::MatrixTransform> front_axle_axle_geometry = new osg::MatrixTransform();
	front_axle_axle_geometry->setMatrix(osg::Matrix::rotate(3.14/2, osg::Vec3(0.0, 1.0, 0.0)));
	front_axle_axle_geometry->addChild(create_cylinder(axle_radius, axle_length));
	front_axle_geometry->addChild(front_axle_axle_geometry.get());

	btTransform axle_and_wheel_trans = btTransform::getIdentity();
	axle_and_wheel_trans.setOrigin(btVector3(0, 2, -1));
    btRigidBody::btRigidBodyConstructionInfo rb(
		axle_compound_mass,
		new MotionState(front_axle_geometry.get()),
		axle_compound,
		axle_inertia);
	front_axle_body = new btRigidBody(rb);
	front_axle_body->setFriction(100);
	front_axle_body->setActivationState(DISABLE_DEACTIVATION);

	btVector3 parentAxis(0.f, 0.f, 1.f);
	btVector3 childAxis(1.f, 0.f, 0.f);
	btVector3 left_position(-1, 2, -1);
	front_axle_spring_left = new btHinge2Constraint(
		*m_body->getBody(), *front_axle_body, left_position, parentAxis, childAxis);
	front_axle_spring_left->setUpperLimit(0.f);
	front_axle_spring_left->setLowerLimit(0.f);
	front_axle_spring_left->setLimit(4, -1, 1);
	front_axle_spring_left->enableSpring(2, true);
	front_axle_spring_left->setStiffness(2, 900);
	front_axle_spring_left->setDamping(2, 10);
	front_axle_spring_left->setLimit(2, 0, 0.5);
	front_axle_spring_left->setEquilibriumPoint(2);

	btVector3 right_position(1, 2, -1);
	front_axle_spring_right = new btHinge2Constraint(
		*m_body->getBody(), *front_axle_body, right_position, parentAxis, childAxis);
	front_axle_spring_right->setUpperLimit(0.f);
	front_axle_spring_right->setLowerLimit(0.f);
	front_axle_spring_right->setLimit(4, -1, 1);
	front_axle_spring_right->enableSpring(2, true);
	front_axle_spring_right->setStiffness(2, 900);
	front_axle_spring_right->setDamping(2, 10);
	front_axle_spring_right->setLimit(2, 0, 0.5);
	front_axle_spring_right->setEquilibriumPoint(2);
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

	rear_axle_geometry = new osg::MatrixTransform();
	rear_axle_geometry->setMatrix(osg::Matrix::translate(osg::Vec3(0, -2, -1)));

	osg::ref_ptr<osg::MatrixTransform> rear_axle_axle_geometry = new osg::MatrixTransform();
	rear_axle_axle_geometry->setMatrix(osg::Matrix::rotate(3.14/2, osg::Vec3(0.0, 1.0, 0.0)));
	rear_axle_axle_geometry->addChild(create_cylinder(axle_radius, axle_length));
	rear_axle_geometry->addChild(rear_axle_axle_geometry.get());

	osg::ref_ptr<osg::MatrixTransform> left_wheel_geometry = new osg::MatrixTransform();
	left_wheel_geometry->setMatrix(
		osg::Matrix::rotate(3.14/2, osg::Vec3(0.0, 1.0, 0.0)) *
		osg::Matrix::translate(osg::Vec3(-1, 0, 0)));
	left_wheel_geometry->addChild(create_cylinder(wheel_radius, wheel_width));
	rear_axle_geometry->addChild(left_wheel_geometry.get());

	osg::ref_ptr<osg::MatrixTransform> right_wheel_geometry = new osg::MatrixTransform();
	right_wheel_geometry->setMatrix(
		osg::Matrix::rotate(3.14/2, osg::Vec3(0.0, 1.0, 0.0)) *
		osg::Matrix::translate(osg::Vec3(1, 0, 0)));
	right_wheel_geometry->addChild(create_cylinder(wheel_radius, wheel_width));
	rear_axle_geometry->addChild(right_wheel_geometry.get());

    btRigidBody::btRigidBodyConstructionInfo rb(
		axle_compound_mass,
		new MotionState(rear_axle_geometry.get()),
		axle_compound,
		axle_inertia);
	rear_axle_body = new btRigidBody(rb);
	rear_axle_body->setFriction(100);
	rear_axle_body->setDamping(0.5, 0.5);
	rear_axle_body->setActivationState(DISABLE_DEACTIVATION);

	btVector3 parentAxis(0.f, 0.f, 1.f);
	btVector3 childAxis(1.f, 0.f, 0.f);
	btVector3 left_position(-1, -2, -1);
	rear_axle_spring_left = new btHinge2Constraint(
		*m_body->getBody(), *rear_axle_body, left_position, parentAxis, childAxis);
	rear_axle_spring_left->setUpperLimit(0.f);
	rear_axle_spring_left->setLowerLimit(0.f);
	rear_axle_spring_left->setLimit(4, -1, 1);
	rear_axle_spring_left->enableSpring(2, true);
	rear_axle_spring_left->setStiffness(2, 900);
	rear_axle_spring_left->setDamping(2, 0.1);
	rear_axle_spring_left->setLimit(2, 0, 0.5);
	rear_axle_spring_left->setEquilibriumPoint(2);
	rear_axle_spring_left->setDbgDrawSize(0.8);

	btVector3 right_position(1, -2, -1);
	rear_axle_spring_right = new btHinge2Constraint(
		*m_body->getBody(), *rear_axle_body, right_position, parentAxis, childAxis);
	rear_axle_spring_right->setUpperLimit(0.f);
	rear_axle_spring_right->setLowerLimit(0.f);
	rear_axle_spring_right->setLimit(4, -1, 1);
	rear_axle_spring_right->enableSpring(2, true);
	rear_axle_spring_right->setStiffness(2, 900);
	rear_axle_spring_right->setDamping(2, 0.10000);
	rear_axle_spring_right->setLimit(2, 0, 0.5);
	rear_axle_spring_right->setEquilibriumPoint(2);
	rear_axle_spring_right->setDbgDrawSize(0.8);
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
	world->getRoot()->addChild(front_axle_geometry.get());
	world->getRoot()->addChild(rear_axle_geometry.get());
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
	rear_axle_body->applyTorque(btVector3(-20000, 0, 0));
	// m_wheels[0]->getBody()->applyTorque(btVector3(-1000, 0, 0));
	// m_wheels[1]->getBody()->applyTorque(btVector3(-1000, 0, 0));
}

void DynamicVehicle::removeTorque() {
	rear_axle_body->applyTorque(btVector3(20000, 0, 0));
	// m_wheels[0]->getBody()->applyTorque(btVector3(1000, 0, 0));
	// m_wheels[1]->getBody()->applyTorque(btVector3(1000, 0, 0));
}
