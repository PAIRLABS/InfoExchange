#!/usr/bin/python

# Python script to find STL dimensions
# Requrements: sudo pip install numpy-stl

import math
import stl
from stl import mesh
import numpy

import os
import sys

if len(sys.argv) < 3:
    sys.exit('Usage: %s [source stl file] scale' % sys.argv[0])

if not os.path.exists(sys.argv[1]):
    sys.exit('ERROR: file %s was not found!' % sys.argv[1])

# this stolen from numpy-stl documentation
# https://pypi.python.org/pypi/numpy-stl

def find_mins_maxs(obj):
    minx = maxx = miny = maxy = minz = maxz = None
    for p in obj.points:
        # p contains (x, y, z)
        if minx is None:
            minx = p[stl.Dimension.X]
            maxx = p[stl.Dimension.X]
            miny = p[stl.Dimension.Y]
            maxy = p[stl.Dimension.Y]
            minz = p[stl.Dimension.Z]
            maxz = p[stl.Dimension.Z]
        else:
            maxx = max(p[stl.Dimension.X], maxx)
            minx = min(p[stl.Dimension.X], minx)
            maxy = max(p[stl.Dimension.Y], maxy)
            miny = min(p[stl.Dimension.Y], miny)
            maxz = max(p[stl.Dimension.Z], maxz)
            minz = min(p[stl.Dimension.Z], minz)
    return minx, maxx, miny, maxy, minz, maxz


# find the max dimensions, so we can know the bounding box, getting the height,
# width, length (because these are the step size)...
def scaling(obj, scale):
    for p in obj.points:
        p[0] *= scale
        p[1] *= scale
        p[2] *= scale
        p[3] *= scale
        p[4] *= scale
        p[5] *= scale
        p[6] *= scale
        p[7] *= scale
        p[8] *= scale
    return

main_body = mesh.Mesh.from_file(sys.argv[1])
minx, maxx, miny, maxy, minz, maxz = find_mins_maxs(main_body)
# the logic is easy from there

print ("File:", sys.argv[1])
print ("X:", maxx - minx)
print ("Y:", maxy - miny)
print ("Z:", maxz - minz)

scaling(main_body, float(sys.argv[2]))
main_body.save(sys.argv[1])


new_body = mesh.Mesh.from_file(sys.argv[1])
minx, maxx, miny, maxy, minz, maxz = find_mins_maxs(new_body)

print ("File:", sys.argv[1])
print ("X:", maxx - minx)
print ("Y:", maxy - miny)
print ("Z:", maxz - minz)
