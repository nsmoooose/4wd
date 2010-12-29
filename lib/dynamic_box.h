#ifndef __DYNAMIC_BOX_H__
#define __DYNAMIC_BOX_H__

#include <osg/Vec3>
#include "dynamic_object.h"

class DynamicBox : public DynamicObject {
public:
	DynamicBox(osg::Vec3 size, btScalar mass);

	virtual btRigidBody *getBody();
	virtual void addToWorld(World* world);

private:
	btRigidBody *m_rigid_body;
};

#endif
