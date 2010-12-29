CXXFLAGS = -g -Os -Wall -pedantic -Werror
LIBS = -Llib -l4wd -lstdc++ -lBulletDynamics -lBulletCollision -losgViewer -losgShadow -lm -lLinearMath -losgGA -losgDB -losg -lOpenThreads

all: 4wd demo1 4wd.osga

4wd: 4wd.o
	$(MAKE) -C lib
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIBS)

demo1: demo1.o
	$(MAKE) -C lib
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIBS)

4wd.osga: $(patsubst %.blend,%.ive,$(wildcard models/*.blend)) \
	$(wildcard models/*.png)
	$(RM) 4wd.osga
	osgarchive -a $@ -i $^

clean:
	$(MAKE) -C lib clean
	$(RM) 4wd *.o 4wd.osga models/*.ive models/*.osg models/*.log models/*.blend1

%.osg: %.blend
	blender -b $< -P models/blender_export.py -- $@

%.ive: %.osg
	osgconv $< $@

install:
	cp 4wd /usr/bin

ctags:
	@ctags -e --recurse=yes --exclude=analysis/* --exclude=.doxygen/*
