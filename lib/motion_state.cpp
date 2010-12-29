#include "motion_state.h"

MotionState::MotionState(osg::MatrixTransform* transform) {
	m_transform = transform;
}

void MotionState::getWorldTransform(btTransform &worldTrans) const {
	osg::Matrix matrix = m_transform->getMatrix();
	osg::Vec3 trans = matrix.getTrans();
	osg::Quat quat = matrix.getRotate();
	worldTrans.setRotation(btQuaternion(quat._v[0], quat._v[1], quat._v[2], quat._v[3]));
	worldTrans.setOrigin(btVector3(trans._v[0], trans._v[1], trans._v[2]));
}

void MotionState::setWorldTransform(const btTransform &worldTrans) {
	btScalar ogl[16];
	worldTrans.getOpenGLMatrix(ogl);
	m_transform->setMatrix(osg::Matrix(ogl));
}
