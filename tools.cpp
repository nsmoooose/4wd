#include <osg/Transform>
#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osg/TriangleFunctor>
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


class CollectVerticesVisitor : public osg::NodeVisitor
{
public:
    CollectVerticesVisitor( osg::NodeVisitor::TraversalMode traversalMode = osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );

#if( ( OPENSCENEGRAPH_MAJOR_VERSION >= 2) && (OPENSCENEGRAPH_MINOR_VERSION >= 8) )
    META_NodeVisitor(osgbBullet,CollectVerticesVisitor)
#endif

    virtual void reset();


    osg::Vec3Array* getVertices()
    {
        return( verts_.get() );
    }

    void apply( osg::Transform& transform );
    void apply( osg::Geode& geode );

    inline void pushMatrix( osg::Matrix & matrix )
    {
        stack_.push_back( matrix );
    }

    inline void popMatrix()
    {
        stack_.pop_back();
    }

    void applyDrawable( osg::Drawable* drawable );

protected:
    typedef std::vector< osg::Matrix >   MatrixStack;

    MatrixStack stack_;
    osg::ref_ptr< osg::Vec3Array > verts_;
};




CollectVerticesVisitor::CollectVerticesVisitor( osg::NodeVisitor::TraversalMode traversalMode )
    : osg::NodeVisitor( traversalMode )
{
    verts_ = new osg::Vec3Array;
    reset();
}

void CollectVerticesVisitor::reset()
{
    stack_.clear();
    stack_.push_back( osg::Matrix::identity() );
    verts_->clear();
}

void CollectVerticesVisitor::apply( osg::Transform& transform )
{
    osg::Matrix matrix;
    matrix = stack_.back();
    transform.computeLocalToWorldMatrix( matrix, this );
    pushMatrix( matrix );

    traverse( transform );

    popMatrix();
}

void CollectVerticesVisitor::apply( osg::Geode& geode )
{
    unsigned int idx;
    for( idx = 0; idx < geode.getNumDrawables(); idx++ )
        applyDrawable( geode.getDrawable( idx ) );
}

void CollectVerticesVisitor::applyDrawable( osg::Drawable* drawable )
{
    osg::Geometry* geom = dynamic_cast< osg::Geometry* >( drawable );
    if( geom == NULL )
        return;

    const osg::Vec3Array* in = dynamic_cast< const osg::Vec3Array* >( geom->getVertexArray() );
    if( in == NULL )
    {
        osg::notify( osg::WARN ) << "CollectVerticesVisitor: Non-Vec3 vertex array encountered." << std::endl;
        return;
    }

    const osg::Matrix& m( stack_.back() );
    osg::Vec3Array::const_iterator iter;
    for( iter = in->begin(); iter != in->end(); iter++ )
    {
        verts_->push_back( *iter * m );
    }
}



btConvexHullShape* btConvexHullCollisionShapeFromOSG(osg::Node* node) {
    CollectVerticesVisitor cvv;
    node->accept( cvv );
    osg::Vec3Array* v = cvv.getVertices();
    osg::notify( osg::INFO ) << "CollectVerticesVisitor: " << v->size() << std::endl;

    btScalar* btverts = new btScalar[ v->size() * 3 ];
    if( btverts == NULL ) {
        osg::notify( osg::FATAL ) << "NULL btverts" << std::endl;
        return( NULL );
    }
    btScalar* btvp = btverts;

	float x_min=10000, x_max=-10000;
	float y_min=10000, y_max=-10000;
	float z_min=10000, z_max=-10000;

    osg::Vec3Array::const_iterator itr;
    for( itr = v->begin(); itr != v->end(); itr++ ) {
        const osg::Vec3& s( *itr );
        *btvp++ = (btScalar)( s[0] );
        *btvp++ = (btScalar)( s[1] );
        *btvp++ = (btScalar)( s[2] );

		x_min = std::min(x_min, s[0]);
		x_max = std::max(x_max, s[0]);
		y_min = std::min(y_min, s[1]);
		y_max = std::max(y_max, s[1]);
		z_min = std::min(z_min, s[2]);
		z_max = std::max(z_max, s[2]);
    }
	std::cout << "x: " << x_min << ", " << x_max << std::endl;
	std::cout << "y: " << y_min << ", " << y_max << std::endl;
	std::cout << "z: " << z_min << ", " << z_max << std::endl;
    btConvexHullShape* chs = new btConvexHullShape( btverts, v->size(), sizeof( btScalar ) * 3 );
    delete[] btverts;
    return( chs );
}

