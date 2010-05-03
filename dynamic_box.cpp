#include <osg/Geode>
#include <osg/ShapeDrawable>
#include "dynamic_box.h"
#include "motion_state.h"

DynamicBox::DynamicBox(osg::Vec3 size, btVector3 inertia, btScalar mass) {
	osg::ref_ptr<osg::Box> box = new osg::Box();
	box->setHalfLengths(size);
	osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(shape.get());
	getNode()->addChild(geode.get());

	btBoxShape *box_shape = new btBoxShape(
		btVector3(size._v[0], size._v[1], size._v[2]));
	if(mass != 0.0f) {
		box_shape->calculateLocalInertia(mass, inertia);
	}
    btRigidBody::btRigidBodyConstructionInfo rb(
		mass,
		new MotionState(getNode()),
		box_shape,
		inertia);
	m_rigid_body = new btRigidBody(rb);
}

DynamicCylinder::DynamicCylinder(float radius, float height, btVector3 inertia, btScalar mass) {
	osg::ref_ptr<osg::Cylinder> osg_shape = new osg::Cylinder();
	osg_shape->setRadius(radius);
	osg_shape->setHeight(height);
	osg::ref_ptr<osg::ShapeDrawable> osg_drawable = new osg::ShapeDrawable(osg_shape);
	osg::ref_ptr<osg::Geode> osg_geode = new osg::Geode();
	osg_geode->addDrawable(osg_drawable.get());
	getNode()->addChild(osg_geode.get());

	btCylinderShape *bt_shape = new btCylinderShape(
		btVector3(radius, radius, height / 2.0));
	if(mass != 0.0f) {
		bt_shape->calculateLocalInertia(mass, inertia);
	}
    btRigidBody::btRigidBodyConstructionInfo rb(
		mass,
		new MotionState(getNode()),
		bt_shape,
		inertia);
	m_rigid_body = new btRigidBody(rb);
}

DynamicSphere::DynamicSphere(float radius, btVector3 inertia, btScalar mass) {
	osg::ref_ptr<osg::Sphere> osg_shape = new osg::Sphere();
	osg_shape->setRadius(radius);
	osg::ref_ptr<osg::ShapeDrawable> osg_drawable = new osg::ShapeDrawable(osg_shape);
	osg::ref_ptr<osg::Geode> osg_geode = new osg::Geode();
	osg_geode->addDrawable(osg_drawable.get());
	getNode()->addChild(osg_geode.get());

	btVector3 bt_pos(0, 0, 0);
	btScalar bt_radius(radius);
	btMultiSphereShape *bt_shape = new btMultiSphereShape(
		&bt_pos, &bt_radius, 1);
	if(mass != 0.0f) {
		bt_shape->calculateLocalInertia(mass, inertia);
	}
    btRigidBody::btRigidBodyConstructionInfo rb(
		mass,
		new MotionState(getNode()),
		bt_shape,
		inertia);
	m_rigid_body = new btRigidBody(rb);
}

DynamicVehicle::DynamicVehicle() {
	/* group of transform nodes */
	osg::ref_ptr<osg::Group> osg_group = new osg::Group;

	osg::ref_ptr<osg::Box> box = new osg::Box();
	box->setHalfLengths(osg::Vec3(5.0f, 1.0f, 5.0f));
	osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(shape.get());
	getNode()->addChild(geode.get());




	btCompoundShape *vehicle_shape = new btCompoundShape();
	btCollisionShape *chassis_shape = new btBoxShape(btVector3(5.0f, 1.0f, 5.0f));
	/*
	btTransform local_trans;
	local_trans.setIdentity();
	local_trans.setOrigin(btVector3(0,1.0,2.5));
	vehicle_shape->addChildShape(local_trans, chassis_shape);
	*/

	btScalar vehicle_mass = 10;
	bool is_dynamic_vehicle = (vehicle_mass != 0.f);
	btVector3 vehicle_inertia(0,0,0);
	if (is_dynamic_vehicle)
		vehicle_shape->calculateLocalInertia(vehicle_mass, vehicle_inertia);
	MotionState *vehicle_motion_state = new MotionState(getNode());
	btRigidBody::btRigidBodyConstructionInfo info(vehicle_mass, vehicle_motion_state, vehicle_shape, vehicle_inertia);
	m_rigid_body = new btRigidBody(info);
	/*
	m_rigid_body->setActivationState(DISABLE_DEACTIVATION);
	*/
}
