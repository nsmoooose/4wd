#ifndef __DYNAMIC_VEHICLE_H__
#define __DYNAMIC_VEHICLE_H__

#include <osg/Vec3>
#include "dynamic_object.h"

class DynamicVehicle : public DynamicObject {
public:
	DynamicVehicle(btDynamicsWorld* dynamicsWorld);

	btRaycastVehicle *m_vehicle;

	float engineForce;
	float breakingForce;

	float maxEngineForce;
	float maxBreakingForce;

	float vehicleSteering;
	float steeringIncrement;
	float steeringClamp;
	float wheelRadius;
	float wheelWidth;
	float wheelFriction;
	float suspensionStiffness;
	float suspensionDamping;
	float suspensionCompression;
	float rollInfluence;

private:
};

#endif
