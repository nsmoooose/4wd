#include <osg/Geode>
#include <osg/ShapeDrawable>
#include "dynamic_box.h"
#include "motion_state.h"
#include "world.h"

DynamicBox::DynamicBox(osg::Vec3 size, btScalar mass) : m_rigid_body(NULL) {
	osg::ref_ptr<osg::Box> box = new osg::Box();
	box->setHalfLengths(size);
	osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(shape.get());
	getNode()->addChild(geode.get());

	btBoxShape *box_shape = new btBoxShape(
		btVector3(size._v[0], size._v[1], size._v[2]));
	btVector3 inertia(0, 0, 0);
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

btRigidBody *DynamicBox::getBody() {
	return m_rigid_body;
}

void DynamicBox::addToWorld(World* world) {
	world->getDynamics()->addRigidBody(m_rigid_body);
	world->getRoot()->addChild(getNode());
}
