#include <cmath>
#include "dynamic_object.h"

DynamicObject::DynamicObject() :
	m_transform(new osg::MatrixTransform) {
}

osg::MatrixTransform *DynamicObject::getNode() {
	return m_transform.get();
}

void DynamicObject::setPosition(float x, float y, float z) {
	btTransform &pos = getBody()->getWorldTransform();
	pos.setOrigin(btVector3(x, y, z));

	/* Force update the position in open scene graph. */
	btScalar ogl[16];
	pos.getOpenGLMatrix(ogl);
	getNode()->setMatrix(osg::Matrix(ogl));
}

void DynamicObject::setRotation(float angle, float x, float y, float z) {
	double inversenorm  = 1.0/sqrt(x*x + y*y + z*z);
	double coshalfangle = cos(0.5*angle);
	double sinhalfangle = sin(0.5*angle);

	float _x = x * sinhalfangle * inversenorm;
	float _y = y * sinhalfangle * inversenorm;
	float _z = z * sinhalfangle * inversenorm;
	float _w = coshalfangle;

	btTransform &pos = getBody()->getWorldTransform();
	pos.setRotation(btQuaternion(_x, _y, _z, _w));

	/* Force update the position in open scene graph. */
	btScalar ogl[16];
	pos.getOpenGLMatrix(ogl);
	getNode()->setMatrix(osg::Matrix(ogl));
}
