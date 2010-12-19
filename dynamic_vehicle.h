#ifndef __DYNAMIC_VEHICLE_H__
#define __DYNAMIC_VEHICLE_H__

#include <osg/Vec3>
#include "dynamic_object.h"

class DynamicVehicle : public DynamicObject {
public:
	DynamicVehicle();

	DynamicObject *m_body;
	DynamicObject *m_wheel_front_left;
	DynamicObject *m_wheel_front_right;
	DynamicObject *m_wheel_back_left;
	DynamicObject *m_wheel_back_right;

	virtual btRigidBody *getBody();
	virtual osg::MatrixTransform *getNode();
	virtual void addToWorld(World* world);
};

#endif
