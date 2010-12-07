#include <osg/Geode>
#include <osg/ShapeDrawable>
#include "dynamic_vehicle.h"
#include "motion_state.h"

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

DynamicVehicle::DynamicVehicle(btDynamicsWorld *dynamicsWorld) {
	engineForce = 0.f;
	breakingForce = 0.f;

	maxEngineForce = 1000.f;
	maxBreakingForce = 100.f;

	vehicleSteering = 0.0f;
	steeringIncrement = 0.04f;
	steeringClamp = 0.3f;
	wheelRadius = 0.5f;
	wheelWidth = 0.4f;
	wheelFriction = 1000;
	suspensionStiffness = 20.f;
	suspensionDamping = 2.3f;
	suspensionCompression = 4.4f;
	rollInfluence = 0.1f;

	osg::Vec3 size(1.5f, 3.0f, 1.0f);

	osg::ref_ptr<osg::Box> box = new osg::Box();
	box->setHalfLengths(size);
	osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(shape.get());
	getNode()->addChild(geode.get());

	btCompoundShape *vehicle_shape = new btCompoundShape();

	/* Add the body of the vehicle to the compound object. */
	btCollisionShape* vehicle_body_shape = new btBoxShape(btVector3(size._v[0], size._v[1], size._v[2]));
	btTransform local_trans;
	local_trans.setIdentity();
	local_trans.setOrigin(btVector3(0,1.0,2.5));
	vehicle_shape->addChildShape(local_trans, vehicle_body_shape);
	btScalar vehicle_mass = 800;
	btVector3 vehicle_inertia(0, 0, 0);
	vehicle_shape->calculateLocalInertia(vehicle_mass, vehicle_inertia);
	MotionState *vehicle_motion_state = new MotionState(getNode());
	btRigidBody::btRigidBodyConstructionInfo info(vehicle_mass, vehicle_motion_state, vehicle_shape, vehicle_inertia);
	m_rigid_body = new btRigidBody(info);
	m_rigid_body->setCenterOfMassTransform(btTransform::getIdentity());
	m_rigid_body->setLinearVelocity(btVector3(0,0,0));
	m_rigid_body->setAngularVelocity(btVector3(0,0,0));

	btRaycastVehicle::btVehicleTuning tuning;
	btVehicleRaycaster *vehicleRayCaster = new btDefaultVehicleRaycaster(dynamicsWorld);
	m_vehicle = new btRaycastVehicle(tuning, m_rigid_body, vehicleRayCaster);
	m_rigid_body->setActivationState(DISABLE_DEACTIVATION);

	int rightIndex = 0;
	int upIndex = 2;
	int forwardIndex = 1;
	btScalar suspensionRestLength(0.6);
	btVector3 wheelDirectionCS0(0,0,-1);
	btVector3 wheelAxleCS(1,0,0);

	float connectionHeight = 1.2f;
	m_vehicle->setCoordinateSystem(rightIndex, upIndex, forwardIndex);
	btVector3 connectionPointCS0(1-(0.3*wheelWidth),2*1-wheelRadius, connectionHeight);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);
	connectionPointCS0 = btVector3(-1+(0.3*wheelWidth),2*1-wheelRadius, connectionHeight);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, true);

	connectionPointCS0 = btVector3(-1+(0.3*wheelWidth),-2*1+wheelRadius, connectionHeight);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);
	connectionPointCS0 = btVector3(1-(0.3*wheelWidth),-2*1+wheelRadius, connectionHeight);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, false);

	for (int i=0;i<m_vehicle->getNumWheels();i++) {
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
	}
}
