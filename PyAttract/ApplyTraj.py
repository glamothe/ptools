#! /usr/bin/env python

# reads a pdf file and a .trj (output from attract only) file and
# creates pdb file from trajectory
# note: trj file are created by Attract_ff2_u.py (version ff2_u only for the moment)
# if option -s is used (for single minimization only)

import sys
from ptools import *

trjfile = open(sys.argv[1])
ligand = Rigidbody(sys.argv[2])

#reads trj file:

lines = trjfile.readlines()

counter = 0

for l in lines:
    if "~~~" in l:
        ligand = output
    else:
        counter+=1
        output=AttractRigidbody(ligand)
        center=output.FindCenter()
        output.Translate(Coord3D()-center)
        X = l.split()
        X = [float(i) for i in X]
        output.AttractEulerRotate(surreal(X[0]), surreal(X[1]), surreal(X[2]))
        output.Translate(Coord3D(surreal(X[3]),surreal(X[4]),surreal(X[5])))
        output.Translate(center)
        outname = "lig_%i.pdb"%counter
        print "writing file", outname
        WritePDB(output, outname)
