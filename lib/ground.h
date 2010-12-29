#ifndef __GROUND_H__
#define __GROUND_H__

#include <osg/Vec3>
#include "dynamic_object.h"

class Ground : public DynamicObject {
public:
	Ground(osg::Vec3 size, btVector3 inertia, btScalar mass);

private:
};

#endif
