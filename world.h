#ifndef __WORLD_H_
#define __WORLD_H__

#include <map>
#include <string>
#include <osg/Group>
#include "dynamic_object.h"

class World {
public:
	World();

	osg::Group *getRoot();
	void setRoot(osg::Group* root) { m_root = root; }
	btDynamicsWorld *getDynamics();

	void addDynamicObject(const std::string &id, DynamicObject *object);

private:
	typedef std::map<std::string, DynamicObject*> dynamic_map;
	dynamic_map m_dynamic_objects;

	osg::ref_ptr<osg::Group> m_root;
	btDynamicsWorld *m_dynamics;
};

#endif
