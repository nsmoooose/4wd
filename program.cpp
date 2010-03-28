#include <btBulletDynamicsCommon.h>
#include <osgViewer/Viewer>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
	std::cout << "Starting 4WD" << std::endl;
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(10, 30, 600, 500);
	viewer.run();
	return 0;
}
