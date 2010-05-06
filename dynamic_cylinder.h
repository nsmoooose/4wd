#ifndef __DYNAMIC_CYLINDER_H__
#define __DYNAMIC_CYLINDER_H__

#include <osg/Vec3>
#include "dynamic_object.h"

class DynamicCylinder : public DynamicObject {
public:
	DynamicCylinder(float radius, float height, btScalar mass);

private:
};

#endif
