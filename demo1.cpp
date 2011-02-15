#include <iostream>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osgViewer/CompositeViewer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include "lib/dynamic_box.h"
#include "lib/dynamic_cylinder.h"
#include "lib/dynamic_model.h"
#include "lib/dynamic_sphere.h"
#include "lib/dynamic_vehicle.h"
#include "lib/gldebugdrawer.h"
#include "lib/vehicle_keyboard_handler.h"
#include "lib/world.h"

void configureDisplay(World& world, osgViewer::CompositeViewer& viewer, osg::Group *scene) {
	osg::GraphicsContext::WindowingSystemInterface* wsi =
		osg::GraphicsContext::getWindowingSystemInterface();
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

	osgViewer::View* view = new osgViewer::View;
	view->setName("View one");
	viewer.addView(view);
	view->setSceneData(scene);
	view->getCamera()->setName("Cam one");
	view->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	view->getCamera()->setGraphicsContext(gc.get());
	view->setCameraManipulator(new osgGA::TrackballManipulator);
	view->addEventHandler(new osgViewer::StatsHandler);
	view->addEventHandler(new osgViewer::HelpHandler);
	view->addEventHandler(new osgViewer::WindowSizeHandler);
	view->addEventHandler(new VehicleKeyboardHandler(&world));
	osg::ref_ptr<osgGA::StateSetManipulator> statesetManipulator = new osgGA::StateSetManipulator;
	statesetManipulator->setStateSet(view->getCamera()->getOrCreateStateSet());
	view->addEventHandler( statesetManipulator.get() );
}

void createWorld(World &world, osg::Group *worldNode, btDynamicsWorld *dynamicsWorld) {
	DynamicObject* ground = new DynamicModel("4wd.osga/models/demo1_ground.ive", btScalar(0));
	ground->setPosition(0, 0, -3.5);
	world.addDynamicObject("ground", ground);

	DynamicVehicle* vehicle = new DynamicVehicle();
	// vehicle->setPosition(0, 0, 20);
	world.addDynamicObject("vehicle", vehicle);
}

int main(int argc, char *argv[]) {
	osg::ref_ptr<osg::Group> root = new osg::Group();

	osgViewer::CompositeViewer viewer;

	osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
	root->addChild(shadowedScene.get());

	/*
	const int ReceivesShadowTraversalMask = 0x1;
	const int CastsShadowTraversalMask = 0x2;
	shadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
	shadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);
	osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
	shadowedScene->setShadowTechnique(sm.get());
	int mapres = 1024;
	sm->setTextureSize(osg::Vec2s(mapres,mapres));
	*/

	osg::Group* lightGroup = new osg::Group;
	osg::Light* light = new osg::Light;
    light->setLightNum(0);
    light->setPosition(osg::Vec4(0, 0, 10, 1.0f));
    light->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    light->setSpotCutoff(90.0f);
    light->setSpotExponent(0.0f);
    light->setDirection(osg::Vec3(0.0f, 0.0f, -1.0f));
	light->setLinearAttenuation(0.1);
	osg::LightSource* light_source = new osg::LightSource;
    light_source->setLight(light);
    light_source->setLocalStateSetModes(osg::StateAttribute::ON);
    lightGroup->addChild(light_source);
	shadowedScene->addChild(lightGroup);

	World world;
	world.setRoot(shadowedScene.get());
	createWorld(world, shadowedScene.get(), world.getDynamics());

	btDynamicsWorld *dynamicsWorld = world.getDynamics();

	GLDebugDrawer* debug_drawer = new GLDebugDrawer();
	// debug_drawer->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
	debug_drawer->setDebugMode(btIDebugDraw::DBG_DrawConstraints|btIDebugDraw::DBG_DrawConstraintLimits|btIDebugDraw::DBG_FastWireframe);
	dynamicsWorld->setDebugDrawer(debug_drawer);
	root->addChild(debug_drawer->getSceneGraph());

	configureDisplay(world, viewer, root.get());

    double prevSimTime = world.getSimulationTime();
    while( !viewer.done() )
    {
		debug_drawer->BeginDraw();

        double currSimTime = world.getSimulationTime();
		if(!world.getPause()) {
			dynamicsWorld->stepSimulation( currSimTime - prevSimTime );
		}
		prevSimTime = currSimTime;

		DynamicVehicle *vehicle = dynamic_cast<DynamicVehicle*>(world.getDynamicObject("vehicle"));
		if(vehicle) {
			std::cout << vehicle->toString() << std::endl;
		}

		dynamicsWorld->debugDrawWorld();
		debug_drawer->EndDraw();
        viewer.frame(currSimTime);
	}

	return 0;
}
