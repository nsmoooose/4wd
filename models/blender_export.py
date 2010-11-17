#!/usr/bin/python
import Blender
import sys

export_file = sys.argv[len(sys.argv) - 1]
print("Exporting to: %s" % export_file)


import osgExport
import osg.osgconf

# osg.osgconf.DEBUG = True
config = osg.osgconf.Config()
config.filename = export_file
osgExport.OpenSceneGraphExport(config)

# Blender.Save(export_file, True)
