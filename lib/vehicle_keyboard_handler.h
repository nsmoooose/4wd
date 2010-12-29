#ifndef __VEHICLEKEYBOARDHANDLER_H__
#define __VEHICLEKEYBOARDHANDLER_H__

#include <osgGA/GUIEventHandler>

class VehicleKeyboardHandler : public osgGA::GUIEventHandler {
public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea,
						osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*);
};

#endif
