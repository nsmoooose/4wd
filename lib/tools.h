#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <osg/Matrix>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Node>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btTransform.h>

/** Create a triangle mesh from a open scenegraph node. */
btTriangleMeshShape *btTriMeshCollisionShapeFromOSG(osg::Node *node);

/** Create a hull aproximation from an open scenegraph node. */
btConvexHullShape *btConvexHullCollisionShapeFromOSG(osg::Node *node);

osg::Matrix asOsgMatrix( const btTransform& t );
btTransform asBtTransform( const osg::Matrix& m );

osg::Matrix asOsgMatrix( const btMatrix3x3& m );
btMatrix3x3 asBtMatrix3x3( const osg::Matrix& m );

osg::Vec3 asOsgVec3( const btVector3& v );
btVector3 asBtVector3( const osg::Vec3& v );

osg::Vec4 asOsgVec4( const btVector3& v, const double w );
osg::Vec4 asOsgVec4( const btVector4& v );
btVector4 asBtVector4( const osg::Vec4& v );

#endif
