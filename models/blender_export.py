#!/usr/bin/python
import Blender
import sys

export_file = sys.argv[len(sys.argv) - 1]
print("Exporting to: %s" % export_file)
Blender.Save(export_file, True)
