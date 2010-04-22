#include "dynamic_object.h"

DynamicObject::DynamicObject() : m_transform(new osg::MatrixTransform) {
}

btRigidBody *DynamicObject::getBody() {
	return m_rigid_body;
}

osg::MatrixTransform *DynamicObject::getNode() {
	return m_transform.get();
}
