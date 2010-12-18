#ifndef __DYNAMIC_CYLINDER_H__
#define __DYNAMIC_CYLINDER_H__

#include <osg/Vec3>
#include "dynamic_object.h"

class DynamicCylinder : public DynamicObject {
public:
	DynamicCylinder(float radius, float height, btScalar mass);

	virtual btRigidBody *getBody();
	virtual void addToWorld(World* world);

private:
	btRigidBody *m_rigid_body;
};

#endif
