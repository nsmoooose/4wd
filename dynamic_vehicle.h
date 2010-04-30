#ifndef __DYNAMIC_CYLINDER_H__
#define __DYNAMIC_CYLINDER_H__

#include <osg/Vec3>
#include "dynamic_object.h"

class DynamicVehicle : public DynamicObject {
public:
	DynamicVehicle(osg::Vec3 size, btVector3 inertia, btScalar mass);

private:
};

#endif
