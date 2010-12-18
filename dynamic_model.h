#ifndef __DYNAMIC_MODEL_H__
#define __DYNAMIC_MODEL_H__

#include "dynamic_object.h"

class DynamicModel : public DynamicObject {
public:
	DynamicModel(const char *path, btScalar mass, bool hull=false);

	virtual btRigidBody *getBody();
	virtual void addToWorld(World* world);

private:
	btRigidBody *m_rigid_body;
};

#endif
