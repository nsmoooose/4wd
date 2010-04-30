#include "dynamic_object.h"

DynamicObject::DynamicObject() : m_transform(new osg::MatrixTransform) {
}

btRigidBody *DynamicObject::getBody() {
	return m_rigid_body;
}

osg::MatrixTransform *DynamicObject::getNode() {
	return m_transform.get();
}

	/*
	osg::Matrix matrix;
	matrix.makeRotate(45.0, osg::Vec3(1.0, 0.0, 0.0));
	matrix.setTrans(0.0, 0.0, 14.0);
	box1->getNode()->setMatrix(matrix);
	*/


void DynamicObject::setPosition(float x, float y, float z) {
	btTransform pos;
	pos.setIdentity();
	pos.setOrigin(btVector3(x, y, z));
	getBody()->setWorldTransform(pos);
}
