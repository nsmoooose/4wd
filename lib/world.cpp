#include <iostream>
#include "dynamic_vehicle.h"
#include "world.h"

World::World() : m_pause(false), m_simulation_time(0.0f), m_root(new osg::Group) {
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
    btVector3 worldAabbMin(-10000, -10000, -10000);
    btVector3 worldAabbMax(10000, 10000, 10000);
    btBroadphaseInterface *inter = new btDbvtBroadphase();
    m_dynamics = new btDiscreteDynamicsWorld(dispatcher, inter, solver, collisionConfiguration);
    m_dynamics->setGravity(btVector3(0, 0, -10));

	m_start_tick = m_timer.tick();
}

double World::getSimulationTime() {
	if(m_pause) {
		return m_simulation_time;
	}
	else {
		return m_timer.time_s();
	}
}

void World::setPause(bool value) {
	m_pause = value;
	if(m_pause) {
		m_start_tick = m_timer.tick();
	}
	else {
		m_timer.setStartTick(m_timer.getStartTick()+(m_timer.tick()-m_start_tick));
	}
}

osg::Group *World::getRoot() {
	return m_root.get();
}

btDynamicsWorld *World::getDynamics() {
	return m_dynamics;
}

void World::addDynamicObject(const std::string &id, DynamicObject *object) {
	m_dynamic_objects[id] = object;
	object->addToWorld(this);
}

DynamicObject* World::getDynamicObject(const std::string &id) {
	return m_dynamic_objects[id];
}

void World::render(osgViewer::ViewerBase* viewer, GLDebugDrawer* debug_drawer) {
    double prevSimTime = getSimulationTime();
    while( !viewer->done() )
    {
		debug_drawer->BeginDraw();

        double currSimTime = getSimulationTime();
		if(!getPause()) {
			m_dynamics->stepSimulation(currSimTime - prevSimTime, 7);
		}
		prevSimTime = currSimTime;

		m_dynamics->debugDrawWorld();
		debug_drawer->EndDraw();
        viewer->frame(currSimTime);
	}
}
