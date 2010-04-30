#include "dynamic_vehicle.h"

#if 0

btRigidBody *localCreateRigidBody(
	float mass, const btTransform& startTransform, btCollisionShape* shape) {
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0,0,0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, myMotionState, shape, localInertia);
	return new btRigidBody(info);
}

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(0,-10,0));

	btCompoundShape* car_shape = new btCompoundShape();

	btCollisionShape* suppShape = new btBoxShape(btVector3(0.5f,0.1f,0.5f));
	btTransform local_trans;
	local_trans.setIdentity();
	local_trans.setOrigin(btVector3(0,1.0,2.5));
	car_shape->addChildShape(suppLocalTrans, suppShape);

	btRigidBody *carChassis = localCreateRigidBody(800.0f, transform, car_shape);
	int rightIndex = 0;
	int upIndex = 2;
	int forwardIndex = 1;
#define CUBE_HALF_EXTENTS 1
	float wheelRadius = 0.5f;
	float wheelWidth = 0.4f;
	float wheelFriction = 1000;//BT_LARGE_FLOAT;
	float suspensionStiffness = 20.f;
	float suspensionDamping = 2.3f;
	float suspensionCompression = 4.4f;
	float rollInfluence = 0.1f;//1.0f;
	btScalar suspensionRestLength(0.6);
	btVector3 wheelDirectionCS0(0,0,-1);
	btVector3 wheelAxleCS(1,0,0);



	btRaycastVehicle::btVehicleTuning tuning;
	btVehicleRaycaster *vehicleRayCaster = new btDefaultVehicleRaycaster(dynamicsWorld);
	btRaycastVehicle *vehicle = new btRaycastVehicle(tuning, carChassis, vehicleRayCaster);
	carChassis->setActivationState(DISABLE_DEACTIVATION);
	dynamicsWorld->addVehicle(vehicle);

	float connectionHeight = 1.2f;
	vehicle->setCoordinateSystem(rightIndex, upIndex, forwardIndex);
	btVector3 connectionPointCS0(CUBE_HALF_EXTENTS-(0.3*wheelWidth),2*CUBE_HALF_EXTENTS-wheelRadius, connectionHeight);
	vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);
	connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),2*CUBE_HALF_EXTENTS-wheelRadius, connectionHeight);
	vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);

	connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),-2*CUBE_HALF_EXTENTS+wheelRadius, connectionHeight);
	vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);
	connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS-(0.3*wheelWidth),-2*CUBE_HALF_EXTENTS+wheelRadius, connectionHeight);
	vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);

	for (int i=0;i<vehicle->getNumWheels();i++) {
		btWheelInfo& wheel = vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
	}

#endif


DynamicVehicle::DynamicVehicle(osg::Vec3 size, btVector3 inertia, btScalar mass) {
}
