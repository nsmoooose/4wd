#include <osgGA/NodeTrackerManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgShadow/ShadowedScene>
#include <osgViewer/CompositeViewer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include "lib/dynamic_box.h"
#include "lib/dynamic_cylinder.h"
#include "lib/dynamic_model.h"
#include "lib/dynamic_sphere.h"
#include "lib/dynamic_vehicle.h"
#include "lib/vehicle_keyboard_handler.h"
#include "lib/world.h"

void configureDisplay(osgViewer::CompositeViewer& viewer, osg::Group *scene) {
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
	view->addEventHandler( new osgViewer::StatsHandler );
	view->addEventHandler( new osgViewer::HelpHandler );
	view->addEventHandler( new osgViewer::WindowSizeHandler );
	osg::ref_ptr<osgGA::StateSetManipulator> statesetManipulator = new osgGA::StateSetManipulator;
	statesetManipulator->setStateSet(view->getCamera()->getOrCreateStateSet());
	view->addEventHandler( statesetManipulator.get() );
}

void createWorld(World &world, osg::Group *worldNode, btDynamicsWorld *dynamicsWorld) {
	world.addDynamicObject(
		"ground",
		new DynamicBox(osg::Vec3(200.0, 200.0, 1.0), btScalar(0)));

	DynamicVehicle* vehicle = new DynamicVehicle();
	vehicle->setPosition(0, 0, 80);
	world.addDynamicObject("vehicle", vehicle);
}

int main(int argc, char *argv[]) {
	osg::ref_ptr<osg::Group> root = new osg::Group();

	osgViewer::CompositeViewer viewer;

	osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
	root->addChild(shadowedScene.get());

	World world;
	world.setRoot(shadowedScene.get());
	createWorld(world, shadowedScene.get(), world.getDynamics());

	configureDisplay(viewer, root.get());

    double prevSimTime = viewer.getFrameStamp()->getSimulationTime();
    while( !viewer.done() )
    {
        double currSimTime = viewer.getFrameStamp()->getSimulationTime();
        world.getDynamics()->stepSimulation( currSimTime - prevSimTime );
        prevSimTime = currSimTime;
        viewer.frame();
	}

	return 0;
}
