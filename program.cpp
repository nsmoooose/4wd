#include <btBulletDynamicsCommon.h>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <iostream>
#include <string>

osg::MatrixTransform *create_box(osg::Vec3 size) {
	osg::ref_ptr<osg::Box> box = new osg::Box();
    box->setHalfLengths(size);

	osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(shape.get());

	osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform();
    transform->addChild(geode.get());
    return transform.release();
}

osg::Node* createWorld() {
	osg::ref_ptr<osg::MatrixTransform> ground = create_box(osg::Vec3(10, 10, 0.01));
	return ground.release();
}

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

	// btAlignedObjectArray<btCollisionShape*> m_collisionShapes;

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(10, 30, 600, 500);
	viewer.setSceneData(createWorld());
    viewer.addEventHandler(new osgViewer::StatsHandler);
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());

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
