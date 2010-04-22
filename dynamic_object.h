#ifndef __DYNAMIC_OBJECT_H__
#define __DYNAMIC_OBJECT_H__

#include <btBulletDynamicsCommon.h>
#include <osg/MatrixTransform>

class DynamicObject {
public:
	DynamicObject();
	osg::MatrixTransform *getNode();
	btRigidBody *getBody();

protected:
	osg::ref_ptr<osg::MatrixTransform> m_transform;
	btRigidBody *m_rigid_body;
};

#endif
