#!/usr/bin/env python
# -*- coding: utf-8 -*-

##
## translate script
## generate starting point
## usage : translate arg1 arg2 [options]
## arg1 : receptor file in the reduce model
## arg2 : ligand file in the reduce model
## to modify the density, use the -d option (the value must be > 1.0), default=100.0
##

from ptools import *
import sys
import os
from math import *
from optparse import OptionParser

parser = OptionParser()
parser.usage = 'translate.py <receptor_file> <ligand_file> [options]'
parser.add_option("-d", "--density", action="store", type="float", dest="density",help="distance in angstroem between starting points (the value must be > 1.0), default is 10.0 angstroem")
(options, args) = parser.parse_args()

rec = Rigidbody(sys.argv[1])
lig = Rigidbody(sys.argv[2])

# search solvation parameters file
completePath=sys.argv[0]
scriptdir,scriptname = os.path.split(completePath)
solvname = os.path.join(scriptdir,"aminon.par")

# initialize some parameters
surf=Surface(30,30,solvname)
center_rec=rec.FindCenter()
center_lig=lig.FindCenter()
rad=lig.Radius()
rad2=rad*rad
surf.surfpointParams(5000,rad)

# grid points generation
grid=surf.surfpoint(rec,1.4)

# remove points too close from the receptor
outergrid=surf.outergrid(grid,rec,rad2)

# remove closest points...
d=10.0
if (options.density):
	d=options.density
outergrid=surf.removeclosest(outergrid,d)

# output starting positions
nb_startingpoint=0
startingpoint=[]

for i in range(len(outergrid)):
	coord=outergrid.getCoords(i)
	nb_startingpoint+=1
	startingpoint.append("%4s %6i %5s %3s %4i    %8.3f%8.3f%8.3f"  %("ATOM",nb_startingpoint, "POSI", "PRO",nb_startingpoint ,float(coord.x),float(coord.y),float(coord.z)))

print "  ",len(startingpoint)
for i in range(len(startingpoint)):
	print startingpoint[i]
