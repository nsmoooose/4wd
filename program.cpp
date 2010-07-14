#include <btBulletDynamicsCommon.h>
#include <cstdlib>
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
		new DynamicBox(osg::Vec3(180, 180, 0.1), btScalar(0)));

#if 0
	DynamicCylinder *cyl1 = new DynamicCylinder(2, 2, btScalar(3.0));
	// cyl1->setRotation(-45.0, 1.0, 0.0, 0.0);
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

	DynamicModel *model1 = new DynamicModel("4wd.osga/models/procera.ive", btScalar(12));
	model1->setRotation(90.0, 1.0, 0.0, 0.0);
	model1->setPosition(0, 0, 150);
	world.addDynamicObject("model1", model1);

	DynamicModel *model2 = new DynamicModel("4wd.osga/models/hollow_box.ive", btScalar(12));
	model2->setRotation(90.0, 1.0, 0.0, 0.0);
	model2->setPosition(0, 0, 100);
	world.addDynamicObject("model2", model2);

	DynamicVehicle *vehicle1 = new DynamicVehicle();
	vehicle1->setPosition(10, 0, 80);
	world.addDynamicObject("vehicle1", vehicle1);
#endif
}

int main(int argc, char *argv[]) {
	std::cout << "Starting 4WD" << std::endl;

	srand(time(NULL));

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
	double lastEvent = prevSimTime;
    viewer.realize();
    while( !viewer.done() )
    {
        double currSimTime = viewer.getFrameStamp()->getSimulationTime();
        world.getDynamics()->stepSimulation( currSimTime - prevSimTime );
        prevSimTime = currSimTime;
        viewer.frame();

		if(currSimTime > lastEvent + 1.0) {
			
			DynamicBox *box2 = new DynamicBox(osg::Vec3(rand() % 10 + 1, rand() % 10 + 1, rand() % 10 + 1), btScalar(rand() % 10 + 1));
			box2->setRotation(rand() % 100, 0.0, 1.0, 0.0);
			box2->setPosition(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 + 50);
			world.addDynamicObject("box2", box2);
			lastEvent = currSimTime;
		}
    }
	return 0;
}
