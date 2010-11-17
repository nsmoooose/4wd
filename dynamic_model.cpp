#include <iostream>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include "dynamic_model.h"
#include "motion_state.h"
#include "tools.h"

DynamicModel::DynamicModel(const char *path, btScalar mass, bool hull) {
	osg::ref_ptr<osg::Node> model;
	model = osgDB::readNodeFile(path);
	if(!model) {
		std::cerr << path << " was not loaded." << std::endl;
		throw int(1);
	}
	getNode()->addChild(model.get());

	btCollisionShape* shape = NULL;
	if(hull)
		shape = btConvexHullCollisionShapeFromOSG(model.get());
	else
		shape = btTriMeshCollisionShapeFromOSG(model.get());

	btVector3 inertia(0, 0, 0);
	if(mass != 0.0f) {
		shape->calculateLocalInertia(mass, inertia);
	}

	/* Use compound shape to change the center of mass. */
	/*
	btCompoundShape* compound = new btCompoundShape();
	btTransform local_trans;
	local_trans.setIdentity();
	local_trans.setOrigin(btVector3(0,0,1));
	compound->addChildShape(local_trans, shape);
	*/

    btRigidBody::btRigidBodyConstructionInfo rb(
		mass,
		new MotionState(getNode()),
		shape,
		inertia);
	m_rigid_body = new btRigidBody(rb);
}
