#ifndef __SHAPES_H__
#define __SHAPES_H__

class Shape {
};

class BoxShape : public Shape {
public:
	osg::Node* getNode();

private:
	osg::ref_ptr<osg::Node> m_node;
};

class CylinderShape : public Shape {
public:
	CylinderShape(btCylinderShapeZ* shape);

protected:
	CylinderShape();
};

class SphereShape : public Shape {
public:
};

class ModelShape : public Shape {
public:
};

class CompundShape : public Shape {
public:
};

#endif
