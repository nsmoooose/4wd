#include <osg/Geode>
#include <osg/Quat>
#include <osg/ShapeDrawable>
#include "dynamic_cylinder.h"
#include "motion_state.h"
#include "world.h"

DynamicCylinder::DynamicCylinder(float radius, float height, btScalar mass) : m_rigid_body(NULL) {
	osg::ref_ptr<osg::Cylinder> osg_shape = new osg::Cylinder();
	osg_shape->setRadius(radius);
	osg_shape->setHeight(height);
	osg::ref_ptr<osg::ShapeDrawable> osg_drawable = new osg::ShapeDrawable(osg_shape);
	osg::ref_ptr<osg::Geode> osg_geode = new osg::Geode();
	osg_geode->addDrawable(osg_drawable.get());
	getNode()->addChild(osg_geode.get());

	btCylinderShape *bt_shape = new btCylinderShapeZ(
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

btRigidBody *DynamicCylinder::getBody() {
	return m_rigid_body;
}

void DynamicCylinder::addToWorld(World* world) {
	world->getDynamics()->addRigidBody(m_rigid_body);
	world->getRoot()->addChild(getNode());
}
