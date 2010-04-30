#ifndef __DYNAMIC_CYLINDER_H__
#define __DYNAMIC_CYLINDER_H__

#include <osg/Vec3>
#include "dynamic_object.h"

class DynamicCylinder : public DynamicObject {
public:
	DynamicCylinder(osg::Vec3 size, btVector3 inertia, btScalar mass);

private:
};

#endif
