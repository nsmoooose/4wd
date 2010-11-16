#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <osg/Node>
#include <btBulletDynamicsCommon.h>

/** Create a triangle mesh from a open scenegraph node. */
btTriangleMeshShape *btTriMeshCollisionShapeFromOSG(osg::Node *node);

/** Create a hull aproximation from an open scenegraph node. */
btConvexHullShape *btConvexHullCollisionShapeFromOSG(osg::Node *node);

#endif
