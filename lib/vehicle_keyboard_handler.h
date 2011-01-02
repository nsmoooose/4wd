#ifndef __VEHICLEKEYBOARDHANDLER_H__
#define __VEHICLEKEYBOARDHANDLER_H__

#include <osgGA/GUIEventHandler>

class DynamicVehicle;
class World;

class VehicleKeyboardHandler : public osgGA::GUIEventHandler {
public:
VehicleKeyboardHandler(World *world) : m_world(world) {}

	virtual bool handle(const osgGA::GUIEventAdapter& ea,
						osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*);

	World *m_world;
};

#endif
