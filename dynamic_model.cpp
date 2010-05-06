#include <iostream>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include "dynamic_model.h"
#include "motion_state.h"
#include "tools.h"

DynamicModel::DynamicModel(const char *path, btScalar mass) {
	osg::ref_ptr<osg::Node> model;
	model = osgDB::readNodeFile(path);
	if(!model) {
		std::cerr << path << " was not loaded." << std::endl;
		throw int(1);
	}
	getNode()->addChild(model.get());

	btTriangleMeshShape *shape = btTriMeshCollisionShapeFromOSG(model.get());
	btVector3 inertia;
	if(mass != 0.0f) {
		shape->calculateLocalInertia(mass, inertia);
	}
    btRigidBody::btRigidBodyConstructionInfo rb(
		mass,
		new MotionState(getNode()),
		shape,
		inertia);
	m_rigid_body = new btRigidBody(rb);
}
