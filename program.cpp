#include <btBulletDynamicsCommon.h>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <iostream>
#include <string>

#include "dynamic_box.h"
#include "dynamic_cylinder.h"
#include "dynamic_model.h"
#include "dynamic_sphere.h"
#include "dynamic_vehicle.h"
#include "world.h"

void createWorld(World &world, osg::Group *worldNode, btDynamicsWorld *dynamicsWorld) {
	world.addDynamicObject(
		"ground",
		new DynamicBox(osg::Vec3(20, 20, 0.1), btScalar(0)));

	DynamicBox *box1 = new DynamicBox(osg::Vec3(0.4, 0.4, 5), btScalar(1.0));
	box1->setRotation(5.0, 1.0, 0.0, 0.0);
	box1->setPosition(0, 0, 14);
	world.addDynamicObject("box1", box1);

	DynamicBox *box2 = new DynamicBox(osg::Vec3(7, 3, 3), btScalar(3.0));
	box2->setRotation(-45.0, 0.0, 1.0, 0.0);
	box2->setPosition(0, 0, 33);
	world.addDynamicObject("box2", box2);

	DynamicCylinder *cyl1 = new DynamicCylinder(2, 2, btScalar(10.0));
	cyl1->setRotation(-45.0, 0.0, 1.0, 0.0);
	cyl1->setPosition(0, 0, 80);
	world.addDynamicObject("cyl1", cyl1);

	DynamicSphere *sphere1 = new DynamicSphere(2, btScalar(10.0));
	sphere1->setPosition(0, 0, 20);
	world.addDynamicObject("sphere1", sphere1);

	DynamicSphere *sphere2 = new DynamicSphere(2, btScalar(10.0));
	sphere2->setPosition(0, 3, 5);
	world.addDynamicObject("sphere2", sphere2);

	DynamicSphere *sphere3 = new DynamicSphere(1.4, btScalar(10.0));
	sphere3->setPosition(3, 0, 6);
	world.addDynamicObject("sphere3", sphere3);

	DynamicModel *model1 = new DynamicModel("4wd.osga/models/wheel.ive", btScalar(12));
	/* model1->setRotation(90.0, 1.0, 0.0, 0.0); */
	model1->setPosition(0, 0, 3);
	world.addDynamicObject("model1", model1);

#if 0
	DynamicVehicle *vehicle1 = new DynamicVehicle();
	vehicle1->setPosition(10, 0, 80);
	world.addDynamicObject("vehicle1", vehicle1);
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
