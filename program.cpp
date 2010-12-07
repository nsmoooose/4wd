#include <btBulletDynamicsCommon.h>
#include <cstdlib>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgShadow/ParallelSplitShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ShadowedScene>
#include <osgViewer/CompositeViewer>
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

class VehicleKeyboardHandler : public osgGA::GUIEventHandler {
public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*) {
		switch(ea.getEventType()) {
		case osgGA::GUIEventAdapter::KEYDOWN:
			std::cout << "keydown" << std::endl;

			switch(ea.getKey()) {
			case osgGA::GUIEventAdapter::KEY_Down:
				std::cout << "down" << std::endl;
				return true;
			case osgGA::GUIEventAdapter::KEY_Up:
				std::cout << "up" << std::endl;
				return true;
			case osgGA::GUIEventAdapter::KEY_Left:
				std::cout << "left" << std::endl;
				return true;
			case osgGA::GUIEventAdapter::KEY_Right:
				std::cout << "right" << std::endl;
				return true;
			default:
				return false;
			}

		case osgGA::GUIEventAdapter::KEYUP:
			std::cout << "keyup" << std::endl;

			switch(ea.getKey()) {
			case osgGA::GUIEventAdapter::KEY_Down:
				std::cout << "down" << std::endl;
				return true;
			case osgGA::GUIEventAdapter::KEY_Up:
				std::cout << "up" << std::endl;
				return true;
			case osgGA::GUIEventAdapter::KEY_Left:
				std::cout << "left" << std::endl;
				return true;
			case osgGA::GUIEventAdapter::KEY_Right:
				std::cout << "right" << std::endl;
				return true;
			default:
				return false;
			}

		default:
			return false;
		}
		return false;
	}
};

void configureDisplay(osgViewer::CompositeViewer& viewer, osg::Group *scene, DynamicObject* object) {
	osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi) {
		osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
		return;
	}

	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 100;
	traits->y = 100;
	traits->width = 1000;
	traits->height = 800;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	if (gc.valid()) {
		osg::notify(osg::INFO)<<"  GraphicsWindow has been created successfully."<<std::endl;
		gc->setClearColor(osg::Vec4f(0.2f,0.2f,0.6f,1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		osg::notify(osg::NOTICE)<<"  GraphicsWindow has not been created successfully."<<std::endl;
	}

	{
		osgViewer::View* view = new osgViewer::View;
		view->setName("View one");
		viewer.addView(view);
		view->setSceneData(scene);
		view->getCamera()->setName("Cam one");
		view->getCamera()->setViewport(new osg::Viewport(0,0, traits->width, traits->height/3 * 2 - 2));
		view->getCamera()->setGraphicsContext(gc.get());
		view->addEventHandler(new osgViewer::StatsHandler);
		view->addEventHandler(new osgViewer::HelpHandler);
		view->addEventHandler(new osgViewer::WindowSizeHandler);
		view->addEventHandler(new VehicleKeyboardHandler());
		osg::ref_ptr<osgGA::StateSetManipulator> statesetManipulator = new osgGA::StateSetManipulator;
		statesetManipulator->setStateSet(view->getCamera()->getOrCreateStateSet());
		view->addEventHandler( statesetManipulator.get() );
		osg::ref_ptr<osgGA::NodeTrackerManipulator> trackerManipulator = new osgGA::NodeTrackerManipulator();
		if(object != NULL) {
			osg::Node* trackingNode = object->getNode()->getChild(0);
			trackerManipulator->setTrackNode(trackingNode);
			trackerManipulator->setTrackerMode( osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION );
			trackerManipulator->setRotationMode( osgGA::NodeTrackerManipulator::TRACKBALL );
			view->setCameraManipulator(trackerManipulator.get());
		}
		else {
			view->setCameraManipulator(new osgGA::TrackballManipulator);
		}
	}

	{
		osgViewer::View* view = new osgViewer::View;
		view->setName("View one");
		viewer.addView(view);
		view->setSceneData(scene);
		view->getCamera()->setName("Cam two");
		view->getCamera()->setViewport(new osg::Viewport(0, traits->height/3 * 2 + 2, traits->width / 2, traits->height/3 - 1));
		view->getCamera()->setGraphicsContext(gc.get());
		view->setCameraManipulator(new osgGA::TrackballManipulator);
		view->addEventHandler( new osgViewer::StatsHandler );
		view->addEventHandler( new osgViewer::HelpHandler );
		view->addEventHandler( new osgViewer::WindowSizeHandler );
		osg::ref_ptr<osgGA::StateSetManipulator> statesetManipulator = new osgGA::StateSetManipulator;
		statesetManipulator->setStateSet(view->getCamera()->getOrCreateStateSet());
		view->addEventHandler( statesetManipulator.get() );
	}

	{
		osgViewer::View* view = new osgViewer::View;
		view->setName("View one");
		viewer.addView(view);
		view->setSceneData(scene);
		view->getCamera()->setName("Cam two");
		view->getCamera()->setViewport(new osg::Viewport(traits->width/2 + 2, traits->height/3 * 2 + 2, traits->width / 2, traits->height/3 - 1));
		view->getCamera()->setGraphicsContext(gc.get());
		view->setCameraManipulator(new osgGA::TrackballManipulator);
		view->addEventHandler( new osgViewer::StatsHandler );
		view->addEventHandler( new osgViewer::HelpHandler );
		view->addEventHandler( new osgViewer::WindowSizeHandler );
		osg::ref_ptr<osgGA::StateSetManipulator> statesetManipulator = new osgGA::StateSetManipulator;
		statesetManipulator->setStateSet(view->getCamera()->getOrCreateStateSet());
		view->addEventHandler( statesetManipulator.get() );
	}
}

void createWorld(World &world, osg::Group *worldNode, btDynamicsWorld *dynamicsWorld) {
	world.addDynamicObject(
		"ground",
		new DynamicModel("4wd.osga/models/ground.ive", btScalar(0)));

	DynamicModel* model = new DynamicModel("4wd.osga/models/hmmwv.ive", btScalar(30), true);
	model->setPosition(0, 0, 80);
	world.addDynamicObject("model", model);

	DynamicVehicle* vehicle = new DynamicVehicle(world.getDynamics());
	vehicle->setPosition(20, 20, 80);
	world.addDynamicObject("vehicle", vehicle);
}

int main(int argc, char *argv[]) {
	std::cout << "Starting 4WD" << std::endl;

	// osg::setNotifyLevel(osg::DEBUG_INFO);

	srand(time(NULL));

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osgViewer::CompositeViewer viewer;

	osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
	root->addChild(shadowedScene.get());

	/*
	const int ReceivesShadowTraversalMask = 0x1;
	const int CastsShadowTraversalMask = 0x2;
	shadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
	shadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);
	osg::ref_ptr<osgShadow::SoftShadowMap> sm = new osgShadow::SoftShadowMap;
	shadowedScene->setShadowTechnique(sm.get());
	*/

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

	World world;
	world.setRoot(shadowedScene.get());
	createWorld(world, shadowedScene.get(), world.getDynamics());

	configureDisplay(viewer, root.get(), world.getDynamicObject("vehicle"));

    double prevSimTime = viewer.getFrameStamp()->getSimulationTime();
	double lastEvent = prevSimTime;
    while( !viewer.done() )
    {
        double currSimTime = viewer.getFrameStamp()->getSimulationTime();
        world.getDynamics()->stepSimulation( currSimTime - prevSimTime );
        prevSimTime = currSimTime;
        viewer.frame();

		if(currSimTime > lastEvent + 1.0) {
			DynamicObject *object = NULL;
			switch(rand() % 3) {
			case 0:
				object = new DynamicBox(osg::Vec3(rand() % 2 + 1, rand() % 2 + 1, rand() % 2 + 1), btScalar(rand() % 10 + 1));
				object->setRotation(rand() % 100, 0.0, 1.0, 0.0);
				object->setPosition(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 + 50);
				break;
			case 1:
				object = new DynamicSphere(rand() % 2 + 1, btScalar(rand() % 10 + 1));
				object->setRotation(rand() % 100, 0.0, 1.0, 0.0);
				object->setPosition(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 + 50);
				break;
			case 2:
				object = new DynamicCylinder(rand() % 2 + 1, rand() % 8 + 1, btScalar(rand() % 1 + 1));
				object->setRotation(rand() % 100, 0.0, 1.0, 0.0);
				object->setPosition(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 + 50);
				break;
			default:
				return 0;
			}
			world.addDynamicObject("object", object);

			lastEvent = currSimTime;
		}
    }
	return 0;
}
