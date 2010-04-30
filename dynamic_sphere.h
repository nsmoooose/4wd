#ifndef __DYNAMIC_SPHERE_H__
#define __DYNAMIC_SPHERE_H__

#include <osg/Vec3>
#include "dynamic_object.h"

class DynamicSphere : public DynamicObject {
public:
	DynamicSphere(osg::Vec3 size, btVector3 inertia, btScalar mass);

private:
};

#endif
