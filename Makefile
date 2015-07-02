CXXFLAGS = -g -Os -Wall -pedantic -Werror $(shell pkg-config --cflags bullet)
LIBS = -Llib -l4wd -lstdc++ -lBulletDynamics -lBulletCollision -losgViewer -losgShadow -lm -lLinearMath -losgGA -losgDB -losg -lOpenThreads -losgText

all: lib 4wd demo1 4wd.osga

lib:
	$(MAKE) -C $@

4wd: lib/lib4wd.a 4wd.o
	$(CC) $(CXXFLAGS) -o $@ $(filter %.o, $^) $(LIBS)

demo1: lib/lib4wd.a demo1.o
	$(CC) $(CXXFLAGS) -o $@ $(filter %.o, $^) $(LIBS)

4wd.osga: $(patsubst %.blend,%.ive,$(wildcard models/*.blend)) \
	$(wildcard models/*.png)
	$(RM) 4wd.osga
	osgarchive -a $@ -i $^

clean:
	$(MAKE) -C lib $@
	$(RM) 4wd *.o 4wd.osga models/*.ive models/*.osg models/*.log models/*.blend1

%.osgt: %.blend
	blender -b $< -P models/blender_export.py -- $@

%.ive: %.osgt
	osgconv $< $@

install:
	cp 4wd /usr/bin

ctags:
	@ctags -e --recurse=yes --exclude=analysis/* --exclude=.doxygen/*

.PHONY: clean all lib
