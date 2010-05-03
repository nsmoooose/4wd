#include "dynamic_box.h"
#include "world.h"

World::World() : m_root(new osg::Group) {
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
    btVector3 worldAabbMin(-10000, -10000, -10000);
    btVector3 worldAabbMax(10000, 10000, 10000);
    btBroadphaseInterface *inter = new btDbvtBroadphase();
    m_dynamics = new btDiscreteDynamicsWorld(dispatcher, inter, solver, collisionConfiguration);
    m_dynamics->setGravity(btVector3(0, 0, -10));
}

osg::Group *World::getRoot() {
	return m_root.get();
}

btDynamicsWorld *World::getDynamics() {
	return m_dynamics;
}

void World::addDynamicObject(const std::string &id, DynamicObject *object) {
	m_dynamic_objects[id] = object;
	m_root->addChild(object->getNode());
	DynamicVehicle *vehicle = dynamic_cast<DynamicVehicle*>(object);
	if(vehicle) {
		btRigidBody *body = vehicle->getBody();
		btRaycastVehicle::btVehicleTuning tuning;
		btVehicleRaycaster *vehicleRayCaster = new btDefaultVehicleRaycaster(m_dynamics);
		btRaycastVehicle *vehicle_raycast = new btRaycastVehicle(tuning, body, vehicleRayCaster);
		m_dynamics->addVehicle(vehicle_raycast);
	}
	else {
		m_dynamics->addRigidBody(object->getBody());
	}
}
