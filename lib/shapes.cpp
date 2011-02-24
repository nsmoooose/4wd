#include <osg/Geode>
#include <osg/Quat>
#include <osg/ShapeDrawable>
#include "shapes.h"

osg::Node* create_cylinder(float radius, float height) {
	osg::ref_ptr<osg::Cylinder> osg_shape = new osg::Cylinder();
	osg_shape->setRadius(radius);
	osg_shape->setHeight(height);
	osg::ref_ptr<osg::ShapeDrawable> osg_drawable = new osg::ShapeDrawable(osg_shape);
	osg::ref_ptr<osg::Geode> osg_geode = new osg::Geode();
	osg_geode->addDrawable(osg_drawable.get());
	return osg_geode.release();
}
