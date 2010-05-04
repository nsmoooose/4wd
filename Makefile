CXXFLAGS = -g -Os -Wall -pedantic -Werror
LIBS = -lstdc++ -lBulletDynamics -lBulletCollision -losgViewer

all: 4wd 4wd.osga

4wd: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(CC) $(LIBS) $(CFLAGS) -o $@ $^

4wd.osga: $(wildcard models/*.ive) $(wildcard models/*.png)
	$(RM) 4wd.osga
	osgarchive -a $@ -i $^

clean:
	$(RM) 4wd *.o
	$(RM) 4wd.osga
