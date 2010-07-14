CXXFLAGS = -g -Os -Wall -pedantic -Werror
LIBS = -lstdc++ -lBulletDynamics -lBulletCollision -losgViewer -lm -lLinearMath -losgGA -losgDB -losg -lOpenThreads

all: 4wd 4wd.osga

4wd: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(CC) $(LIBS) $(CFLAGS) -o $@ $^

4wd.osga: $(patsubst %.blend,%.ive,$(wildcard models/*.blend)) \
	$(wildcard models/*.png)
	$(RM) 4wd.osga
	osgarchive -a $@ -i $^

clean:
	$(RM) 4wd *.o 4wd.osga models/*.ive models/*.dxf

%.dxf: %.blend
	blender -b $< -P models/blender_export.py -- $@ > /dev/null

%.ive: %.dxf
	osgconv $< $@

install:
	cp 4wd /usr/libexec/gnome-screensaver