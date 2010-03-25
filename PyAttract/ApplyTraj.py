#! /usr/bin/env python

# reads a pdf file and a .trj (output from attract only) file and
# creates pdb file from trajectory


import sys
from ptools import *

if len(sys.argv) < 3:
    print "Usage: applyTraj.py trajectory_file ligand"
    sys.exit(0)


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
        outname = "lig_%05i.pdb"%counter
        print "writing file", outname
        WritePDB(output, outname)
