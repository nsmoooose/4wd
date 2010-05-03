#ifndef __DYNAMIC_SPHERE_H__
#define __DYNAMIC_SPHERE_H__

#include "dynamic_object.h"

class DynamicSphere : public DynamicObject {
public:
	DynamicSphere(float radius, btVector3 inertia, btScalar mass);

private:
};

#endif
