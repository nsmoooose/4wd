#ifndef __DYNAMIC_OBJECT_H__
#define __DYNAMIC_OBJECT_H__

#include <btBulletDynamicsCommon.h>
#include <osg/MatrixTransform>

class World;

class DynamicObject {
public:
	DynamicObject();
	virtual ~DynamicObject() {}
	osg::MatrixTransform *getNode();

	void setRotation(float angle, float x, float y, float z);
	void setPosition(float x, float y, float z);

	virtual btRigidBody *getBody() = 0;
	virtual void addToWorld(World* world) = 0;

protected:
	osg::ref_ptr<osg::MatrixTransform> m_transform;
};

#endif
