#include <osg/Transform>
#include <osg/Drawable>
#include <osg/Geode>
#include <osg/PrimitiveSet>
#include <osg/TriangleFunctor>

#include <osgbBullet/ComputeTriMeshVisitor.h>

#include <iostream>

#include "tools.h"

class ComputeTriMeshVisitor : public osg::NodeVisitor {
public:
    ComputeTriMeshVisitor( osg::NodeVisitor::TraversalMode traversalMode = TRAVERSE_ALL_CHILDREN );

#if( ( OPENSCENEGRAPH_MAJOR_VERSION >= 2) && (OPENSCENEGRAPH_MINOR_VERSION >= 8) )
    META_NodeVisitor(osgbBullet,ComputeTriMeshVisitor)
#endif

    virtual void reset();

    osg::Vec3Array * getTriMesh() {
        return( mesh.get() );
    }

    void apply( osg::Node & node );
    void apply( osg::Transform & transform );
    void apply( osg::Geode & geode );

    inline void pushMatrix( osg::Matrix & matrix ) {
        stack.push_back( matrix );
    }

    inline void popMatrix() {
        stack.pop_back();
    }

    void applyDrawable( osg::Drawable * drawable );

protected:
    typedef std::vector< osg::Matrix >   MatrixStack;

    MatrixStack stack;
    osg::ref_ptr< osg::Vec3Array > mesh;
};

struct ComputeTriMeshFunc {
    ComputeTriMeshFunc() {
        vertices = new osg::Vec3Array;
        vertices->clear();
    }

    void inline operator()( const osg::Vec3 v1, const osg::Vec3 v2, const osg::Vec3 v3, bool _temp ) {
        vertices->push_back( v1 );
        vertices->push_back( v2 );
        vertices->push_back( v3 );
    }

    osg::ref_ptr< osg::Vec3Array > vertices;
};

ComputeTriMeshVisitor::ComputeTriMeshVisitor( osg::NodeVisitor::TraversalMode traversalMode )
    : osg::NodeVisitor( traversalMode ) {
    stack.push_back( osg::Matrix::identity() );
    mesh = new osg::Vec3Array;
}

void ComputeTriMeshVisitor::reset() {
    stack.clear();
    stack.push_back( osg::Matrix::identity() );
    mesh->clear();
}

void ComputeTriMeshVisitor::apply( osg::Node & node ) {
    traverse( node );
}

void ComputeTriMeshVisitor::apply( osg::Transform & transform ) {
    osg::Matrix matrix;
    matrix = stack.back();
    transform.computeLocalToWorldMatrix( matrix, this );
    pushMatrix( matrix );
    traverse( transform );
    popMatrix();
}

void ComputeTriMeshVisitor::apply( osg::Geode & geode ) {
    for( unsigned int i = 0; i < geode.getNumDrawables(); ++i ) {
        applyDrawable( geode.getDrawable( i ) );
    }
}

void ComputeTriMeshVisitor::applyDrawable( osg::Drawable * drawable ) {
    osg::TriangleFunctor< ComputeTriMeshFunc > functor;
    drawable->accept( functor );

    const osg::Matrix& matrix = stack.back();
    for( osg::Vec3Array::iterator iter = functor.vertices->begin();
         iter != functor.vertices->end(); ++iter ) {
        mesh->push_back( *iter * matrix );
    }
}


btTriangleMeshShape* btTriMeshCollisionShapeFromOSG( osg::Node* node ) {
    ComputeTriMeshVisitor visitor;
    node->accept( visitor );

    osg::Vec3Array* vertices = visitor.getTriMesh();
    if( vertices->size() < 3 ) {
        osg::notify( osg::WARN ) << "osgbBullet::btTriMeshCollisionShapeFromOSG, no triangles found" << std::endl;
        return( NULL );
    }

    btTriangleMesh* mesh = new btTriangleMesh;
    for( size_t i = 0; i + 2 < vertices->size(); i += 3 ) {
        osg::Vec3& p1 = ( *vertices )[ i ];
        osg::Vec3& p2 = ( *vertices )[ i + 1 ];
        osg::Vec3& p3 = ( *vertices )[ i + 2 ];
        mesh->addTriangle( btVector3( p1.x(), p1.y(), p1.z() ),
                          btVector3( p2.x(), p2.y(), p2.z() ),
                          btVector3( p3.x(), p3.y(), p3.z() ) );
    }

    btBvhTriangleMeshShape* meshShape = new btBvhTriangleMeshShape( mesh, true );
    return( meshShape );
}
