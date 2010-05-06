#include <osg/Geode>
#include <osg/ShapeDrawable>
#include "dynamic_sphere.h"
#include "motion_state.h"

DynamicSphere::DynamicSphere(float radius, btScalar mass) {
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
	btVector3 inertia(0, 0, 0);
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
