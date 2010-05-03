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
