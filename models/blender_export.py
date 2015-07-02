#!/usr/bin/python
import bpy
import osg
import sys
export_file = sys.argv[len(sys.argv) - 1]

config = osg.osgconf.Config()
config.initFilePaths(export_file)
config.export_anim = False
config.apply_modifiers = False
config.scene = bpy.context.scene
config.json_materials = False
config.json_shaders = False

export = osg.osgdata.Export(config)
print("Exporting to: %s" % config.filename)
export.process()
export.write()
