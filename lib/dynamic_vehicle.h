#ifndef __DYNAMIC_VEHICLE_H__
#define __DYNAMIC_VEHICLE_H__

#include <osg/Vec3>
#include "dynamic_object.h"

class DynamicVehicle : public DynamicObject {
public:
	DynamicVehicle();

	virtual btRigidBody *getBody();
	virtual osg::MatrixTransform *getNode();
	virtual void addToWorld(World* world);

private:
	DynamicObject *m_body;
	DynamicObject *m_wheel_front_left;
	DynamicObject *m_wheel_front_right;
	DynamicObject *m_wheel_back_left;
	DynamicObject *m_wheel_back_right;

	btHinge2Constraint *m_wheel_front_left_c;
	btHinge2Constraint *m_wheel_front_right_c;
	btHinge2Constraint *m_wheel_back_left_c;
	btHinge2Constraint *m_wheel_back_right_c;
};

#endif
