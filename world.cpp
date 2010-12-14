#include "dynamic_vehicle.h"
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
		m_dynamics->addAction(vehicle->m_vehicle);
		m_dynamics->addRigidBody(vehicle->getBody());
	}
	else {
		m_dynamics->addRigidBody(object->getBody());
	}
}

DynamicObject* World::getDynamicObject(const std::string &id) {
	return m_dynamic_objects[id];
}
