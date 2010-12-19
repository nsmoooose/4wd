#include <osg/Geode>
#include <osg/ShapeDrawable>
#include "dynamic_box.h"
#include "dynamic_cylinder.h"
#include "dynamic_vehicle.h"
#include "motion_state.h"
#include "world.h"

DynamicVehicle::DynamicVehicle() {
	m_body = new DynamicBox(osg::Vec3(1.5f, 3.0f, 1.0f), 800);
	m_wheel_front_left = new DynamicCylinder(0.5, 0.30, 9.0);
	m_wheel_front_right = new DynamicCylinder(0.5, 0.30, 9.0);
	m_wheel_back_left = new DynamicCylinder(0.5, 0.30, 9.0);
	m_wheel_back_right = new DynamicCylinder(0.5, 0.30, 9.0);


#if 0
   	btHinge2Constraint* pHinge2 = new btHinge2Constraint(*pBodyA, *pBodyB, anchor, parentAxis, childAxis);

    btHinge2Constraint(btRigidBody& rbA, btRigidBody& rbB, btVector3& anchor, btVector3& axis1, btVector3& axis2);
	void setUpperLimit(btScalar ang1max) { setAngularUpperLimit(btVector3(-1.f, 0.f, ang1max)); }
	void setLowerLimit(btScalar ang1min) { setAngularLowerLimit(btVector3( 1.f, 0.f, ang1min)); }
#endif
}

btRigidBody *DynamicVehicle::getBody() {
	return m_body->getBody();
}

osg::MatrixTransform *DynamicVehicle::getNode() {
	return m_body->getNode();
}

void DynamicVehicle::addToWorld(World* world) {
	btDynamicsWorld* dynamics = world->getDynamics();

	dynamics->addRigidBody(m_body->getBody());
	world->getRoot()->addChild(m_body->getNode());

	dynamics->addRigidBody(m_wheel_front_left->getBody());
	world->getRoot()->addChild(m_wheel_front_left->getNode());
	dynamics->addRigidBody(m_wheel_front_right->getBody());
	world->getRoot()->addChild(m_wheel_front_right->getNode());

	dynamics->addRigidBody(m_wheel_back_left->getBody());
	world->getRoot()->addChild(m_wheel_back_left->getNode());
	dynamics->addRigidBody(m_wheel_back_right->getBody());
	world->getRoot()->addChild(m_wheel_back_right->getNode());

	// world->addDynamics()->addConstraint();
}
