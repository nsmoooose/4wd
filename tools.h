#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <osg/Node>
#include <btBulletDynamicsCommon.h>

btTriangleMeshShape* btTriMeshCollisionShapeFromOSG(osg::Node* node);

#endif
