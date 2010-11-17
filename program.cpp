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
		new DynamicModel("4wd.osga/models/ground.ive", btScalar(0)));

	DynamicModel* vehicle = new DynamicModel("4wd.osga/models/hmmwv.ive", btScalar(30));
	vehicle->setPosition(0, 0, 80);
	world.addDynamicObject("vehicle", vehicle);
}

int main(int argc, char *argv[]) {
	std::cout << "Starting 4WD" << std::endl;

	srand(time(NULL));

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(10, 30, 600, 500);

	World world;
	osg::ref_ptr<osg::Group> root = world.getRoot();
	createWorld(world, root.get(), world.getDynamics());

	/*
	osg::Group* lightGroup = new osg::Group;
	osg::Light* light = new osg::Light;
    light->setLightNum(0);
    light->setPosition(osg::Vec4(10, 10, 100, 1.0f));
    light->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    light->setSpotCutoff(90.0f);
    light->setSpotExponent(0.0f);
    light->setDirection(osg::Vec3(0.0f, 0.0f, -1.0f));
	light->setLinearAttenuation(0.1);
	osg::LightSource* light_source = new osg::LightSource;
    light_source->setLight(light);
    light_source->setLocalStateSetModes(osg::StateAttribute::ON);
    //light_source->setStateSetModes(*rootStateSet,osg::StateAttribute::ON);
    lightGroup->addChild(light_source);
	root->addChild(lightGroup);
	*/

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
