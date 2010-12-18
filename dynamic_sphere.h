#ifndef __DYNAMIC_SPHERE_H__
#define __DYNAMIC_SPHERE_H__

#include "dynamic_object.h"

class DynamicSphere : public DynamicObject {
public:
	DynamicSphere(float radius, btScalar mass);

	virtual btRigidBody *getBody();
	virtual void addToWorld(World* world);

private:
	btRigidBody *m_rigid_body;
};

#endif
