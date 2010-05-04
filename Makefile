OBJECTS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
#CFLAGS = $(shell pkg-config --cflags bullet)
CFLAGS = -g -Os -Wall -pedantic -Werror
LIBS = -lstdc++ -lBulletDynamics -lBulletCollision -losgViewer

MODELS = $(wildcard models/*.ive) $(wildcard models/*.png)

all: 4wd 4wd.osga

4wd: $(OBJECTS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $^

4wd.osga: $(MODELS)
	$(RM) 4wd.osga
	osgarchive -a $@ -i $^

clean:
	$(RM) 4wd *.o
	$(RM) 4wd.osga
