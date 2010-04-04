#include <btBulletDynamicsCommon.h>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <iostream>
#include <string>

/** Syncs movements between open scene graph and bullet world. */
class MotionState : public btMotionState {
public:
	MotionState(osg::MatrixTransform* transform) {
		m_transform = transform;
	}

	/** Returns the initial position and rotation of an object. */
	virtual void getWorldTransform(btTransform &worldTrans) const {
		osg::Matrix matrix = m_transform->getMatrix();
		osg::Vec3 trans = matrix.getTrans();
		osg::Quat quat = matrix.getRotate();
		worldTrans.setRotation(btQuaternion(quat._v[0], quat._v[1], quat._v[2], quat._v[3]));
		worldTrans.setOrigin(btVector3(trans._v[0], trans._v[1], trans._v[2]));
	}

	/** Alters the position and rotation of an object. */
	virtual void setWorldTransform(const btTransform &worldTrans) {
		btScalar ogl[16];
		worldTrans.getOpenGLMatrix(ogl);
		m_transform->setMatrix(osg::Matrix(ogl));
	}

private:
	osg::ref_ptr<osg::MatrixTransform> m_transform;
};

osg::MatrixTransform *create_box(osg::Vec3 size) {
	osg::ref_ptr<osg::Box> box = new osg::Box();
    box->setHalfLengths(size);

	osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(shape.get());

	osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform();
    transform->addChild(geode.get());
    return transform.release();
}

void createWorld(osg::Group *parent, btDynamicsWorld *dynamicsWorld) {
	osg::ref_ptr<osg::MatrixTransform> ground = create_box(osg::Vec3(10, 10, 0.1));
	parent->addChild(ground.get());

    btRigidBody::btRigidBodyConstructionInfo rb(
		btScalar(0.0), /* mass */
		new MotionState(ground.get()),
		new btBoxShape(btVector3(10, 10, 0.1)),
		btVector3(0, 0, 0)); /* inertia */
	dynamicsWorld->addRigidBody(new btRigidBody(rb));


	osg::ref_ptr<osg::MatrixTransform> box = create_box(osg::Vec3(3, 3, 3));
	osg::Matrix matrix;
	matrix.makeRotate(45.0, osg::Vec3(1.0, 0.0, 0.0));
	matrix.setTrans(0.0, 0.0, 14.0);
	box->setMatrix(matrix);
	parent->addChild(box.get());

	btBoxShape *shape = new btBoxShape(btVector3(3, 3, 3));
	btVector3 inertia(1, 1, 1);
    btRigidBody::btRigidBodyConstructionInfo box_rb(
		btScalar(1.0), /* mass */
		new MotionState(box.get()),
		shape,
		inertia); /* inertia */
	dynamicsWorld->addRigidBody(new btRigidBody(box_rb));



	box = create_box(osg::Vec3(7, 3, 3));
	matrix.makeRotate(5.0, osg::Vec3(1.0, 0.0, 0.0));
	matrix.setTrans(0.0, 0.0, 40.0);
	box->setMatrix(matrix);
	parent->addChild(box.get());

	shape = new btBoxShape(btVector3(7, 3, 3));
	inertia = btVector3(1, 1, 1);
    box_rb = btRigidBody::btRigidBodyConstructionInfo(
		btScalar(3.0), /* mass */
		new MotionState(box.get()),
		shape,
		inertia); /* inertia */
	dynamicsWorld->addRigidBody(new btRigidBody(box_rb));
}

int main(int argc, char *argv[]) {
	std::cout << "Starting 4WD" << std::endl;

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
    btVector3 worldAabbMin(-10000, -10000, -10000);
    btVector3 worldAabbMax(10000, 10000, 10000);
    /* btBroadphaseInterface *inter = new btAxisSweep3(worldAabbMin, worldAabbMax, 1000); */
    btBroadphaseInterface *inter = new btDbvtBroadphase();
    btDynamicsWorld *dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, inter, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, 0, -10));

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(10, 30, 600, 500);

	osg::ref_ptr<osg::Group> root = new osg::Group();
	createWorld(root.get(), dynamicsWorld);

	viewer.setSceneData(root.get());
    viewer.addEventHandler(new osgViewer::StatsHandler);
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());

    double prevSimTime = viewer.getFrameStamp()->getSimulationTime();
    viewer.realize();
    while( !viewer.done() )
    {
        double currSimTime = viewer.getFrameStamp()->getSimulationTime();
        dynamicsWorld->stepSimulation( currSimTime - prevSimTime );
        prevSimTime = currSimTime;
        viewer.frame();
    }
	return 0;
}
