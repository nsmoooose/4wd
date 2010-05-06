#include <osg/Geode>
#include <osg/ShapeDrawable>
#include "dynamic_cylinder.h"
#include "motion_state.h"

DynamicCylinder::DynamicCylinder(float radius, float height, btScalar mass) {
	osg::ref_ptr<osg::Cylinder> osg_shape = new osg::Cylinder();
	osg_shape->setRadius(radius);
	osg_shape->setHeight(height);
	osg::ref_ptr<osg::ShapeDrawable> osg_drawable = new osg::ShapeDrawable(osg_shape);
	osg::ref_ptr<osg::Geode> osg_geode = new osg::Geode();
	osg_geode->addDrawable(osg_drawable.get());
	getNode()->addChild(osg_geode.get());

	btCylinderShape *bt_shape = new btCylinderShape(
		btVector3(radius, radius, height / 2.0));
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
