#include <iostream>
#include "dynamic_vehicle.h"
#include "vehicle_keyboard_handler.h"
#include "world.h"

bool VehicleKeyboardHandler::handle(
	const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa,
	osg::Object*, osg::NodeVisitor*) {

	switch(ea.getEventType()) {
	case osgGA::GUIEventAdapter::KEYDOWN:
		std::cout << "keydown" << std::endl;

		switch(ea.getKey()) {
		case osgGA::GUIEventAdapter::KEY_Space:
		{
		}

		case osgGA::GUIEventAdapter::KEY_Down:
		{
			std::cout << "down" << std::endl;
			DynamicVehicle *vehicle = dynamic_cast<DynamicVehicle*>(m_world->getDynamicObject("vehicle"));
			if(vehicle) {
				std::cout << "remove torque" << std::endl;
				vehicle->removeTorque();
			}
			return true;
		}

		case osgGA::GUIEventAdapter::KEY_Up:
		{
			std::cout << "up" << std::endl;
			DynamicVehicle *vehicle = dynamic_cast<DynamicVehicle*>(m_world->getDynamicObject("vehicle"));
			if(vehicle) {
				std::cout << "add torque" << std::endl;
				vehicle->addTorque();
			}
			return true;
		}

		case osgGA::GUIEventAdapter::KEY_Left:
		{
			std::cout << "left" << std::endl;
			DynamicVehicle *vehicle = dynamic_cast<DynamicVehicle*>(m_world->getDynamicObject("vehicle"));
			if(vehicle) {
				std::cout << "turn left" << std::endl;
				vehicle->turnLeft();
			}
			return true;
		}

		case osgGA::GUIEventAdapter::KEY_Right:
		{
			std::cout << "right" << std::endl;
			DynamicVehicle *vehicle = dynamic_cast<DynamicVehicle*>(m_world->getDynamicObject("vehicle"));
			if(vehicle) {
				std::cout << "turn left" << std::endl;
				vehicle->turnRight();
			}
			return true;
		}

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
