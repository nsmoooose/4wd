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
	void createFrontAxle();
	void createRearAxle();

	DynamicObject *m_body;

	btHinge2Constraint* front_axle_spring_left;
	btHinge2Constraint* front_axle_spring_right;
	btRigidBody* front_axle_body;

	btHinge2Constraint* rear_axle_spring_left;
	btHinge2Constraint* rear_axle_spring_right;
	btRigidBody* rear_axle_body;
};

#endif
