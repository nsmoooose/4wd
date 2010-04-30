#include <btBulletDynamicsCommon.h>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <iostream>
#include <string>

#include "world.h"
#include "dynamic_box.h"
#include "motion_state.h"
/*
   osg::Cylinder(const osg::Vec3 &center, float radius, float height)
   btCylinderShape(const btVector3 &halfExtents);

   osg::Sphere(const osg::Vec3 &center, float radius)
   btMultiSphereShape(const btVector3 *positions, const btScalar *radi, int	numSpheres)
*/

void create_osg_box(osg::MatrixTransform *transform, osg::Vec3 size) {
	osg::ref_ptr<osg::Box> box = new osg::Box();
	box->setHalfLengths(size);
	osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(shape.get());
	transform->addChild(geode.get());
}

void create_box_model(osg::MatrixTransform *transform, btDynamicsWorld *dynamicsWorld,
				osg::Vec3 size, btVector3 inertia, btScalar mass) {
	create_osg_box(transform, size);

	btBoxShape *shape = new btBoxShape(btVector3(size._v[0], size._v[1], size._v[2]));
    btRigidBody::btRigidBodyConstructionInfo rb(
		mass,
		new MotionState(transform),
		shape,
		inertia);
	dynamicsWorld->addRigidBody(new btRigidBody(rb));
}

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

void createWorld(World &world, osg::Group *worldNode, btDynamicsWorld *dynamicsWorld) {
#if 0
	osg::ref_ptr<osg::Node> terrain = osgDB::readNodeFile("puget.ive");
	if(terrain) {
		worldNode->addChild(terrain.get());
	}
#endif

	world.addDynamicObject(
		"ground",
		new DynamicBox(osg::Vec3(20, 20, 0.1), btVector3(0,0,0), btScalar(0)));

	DynamicBox *box1 = new DynamicBox(osg::Vec3(0.4, 0.4, 5), btVector3(1, 1, 1), btScalar(1.0));
	box1->setPosition(0, 0, 14);
	world.addDynamicObject("box1", box1);

	DynamicBox *box2 = new DynamicBox(osg::Vec3(7, 3, 3), btVector3(1, 1, 1), btScalar(3.0));
	box2->setPosition(0, 0, 33);
	world.addDynamicObject("box2", box2);

#if 0
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
}

int main(int argc, char *argv[]) {
	std::cout << "Starting 4WD" << std::endl;

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(10, 30, 600, 500);

	World world;
	osg::ref_ptr<osg::Group> root = world.getRoot();
	createWorld(world, root.get(), world.getDynamics());

	viewer.setSceneData(root.get());
    viewer.addEventHandler(new osgViewer::StatsHandler);
    viewer.addEventHandler(new osgViewer::HelpHandler);
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());

    double prevSimTime = viewer.getFrameStamp()->getSimulationTime();
    viewer.realize();
    while( !viewer.done() )
    {
        double currSimTime = viewer.getFrameStamp()->getSimulationTime();
        world.getDynamics()->stepSimulation( currSimTime - prevSimTime );
        prevSimTime = currSimTime;
        viewer.frame();
    }
	return 0;
}
