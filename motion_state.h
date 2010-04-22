#ifndef __MOTION_STATE_H__
#define __MOTION_STATE_H__

#include <btBulletDynamicsCommon.h>
#include <osg/MatrixTransform>

/** Syncs movements between open scene graph and bullet world. */
class MotionState : public btMotionState {
public:
	MotionState(osg::MatrixTransform* transform);

	/** Returns the initial position and rotation of an object. */
	virtual void getWorldTransform(btTransform &worldTrans) const;

	/** Alters the position and rotation of an object. */
	virtual void setWorldTransform(const btTransform &worldTrans);

private:
	osg::ref_ptr<osg::MatrixTransform> m_transform;
};

#endif
