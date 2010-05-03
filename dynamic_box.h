#ifndef __DYNAMIC_BOX_H__
#define __DYNAMIC_BOX_H__

#include <osg/Vec3>
#include "dynamic_object.h"

class DynamicBox : public DynamicObject {
public:
	DynamicBox(osg::Vec3 size, btVector3 inertia, btScalar mass);

private:
};

class DynamicCylinder : public DynamicObject {
public:
	DynamicCylinder(float radius, float height, btVector3 inertia, btScalar mass);

private:
};

class DynamicSphere : public DynamicObject {
public:
	DynamicSphere(float radius, btVector3 inertia, btScalar mass);

private:
};

class DynamicVehicle : public DynamicObject {
public:
	DynamicVehicle();

private:
};

#endif
