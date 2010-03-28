OBJECTS = $(patsubst %.c,%.o,$(wildcard *.cpp))
#CFLAGS = $(shell pkg-config --cflags bullet)
LIBS = -lstdc++ -lBulletDynamics -lBulletCollision -losgViewer

all: 4wd

4wd: $(OBJECTS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $(OBJECTS)

clean:
	$(RM) 4wd *.o
