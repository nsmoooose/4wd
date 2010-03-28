#include <btBulletDynamicsCommon.h>
#include <osgViewer/Viewer>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
	std::cout << "Starting 4WD" << std::endl;

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
    btVector3 worldAabbMin(-10000, -10000, -10000);
    btVector3 worldAabbMax(10000, 10000, 10000);
    btBroadphaseInterface *inter = new btAxisSweep3(worldAabbMin, worldAabbMax, 1000);
    btDynamicsWorld *dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, inter, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3( 0, 0, -10 ));

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(10, 30, 600, 500);

    double prevSimTime = viewer.getFrameStamp()->getSimulationTime();
    viewer.realize();
    while( !viewer.done() )
    {
        double currSimTime = viewer.getFrameStamp()->getSimulationTime();
        dynamicsWorld->stepSimulation( currSimTime - prevSimTime );
        prevSimTime = currSimTime;
        viewer.frame();
    }
	return 0;
}
