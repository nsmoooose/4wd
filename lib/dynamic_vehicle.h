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

	virtual void addTorque();
	virtual void removeTorque();
	virtual void turnLeft();
	virtual void turnRight();

	virtual std::string toString();

private:
	DynamicObject *m_body;
	DynamicObject* m_wheels[4];
	btHinge2Constraint* m_wheels_c[4];
};

#endif
