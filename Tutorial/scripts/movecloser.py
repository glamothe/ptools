#!/usr/bin/env python

import sys

from ptools import *


# this small script takes 2 arguments: receptor filename and ligand filename
# and will move the ligand in such way that center to center distance equal 30 Angstroms

#read command line parameters:
args = sys.argv

#args[0] contains program path name
receptorname = args[1]
ligandname = args[2]

#load receptor and ligand:
rec = Rigidbody(receptorname) #receptor
lig = Rigidbody(ligandname)  #ligand

#calculates center of mass of receptor:
cenR = rec.FindCenter()

#center of ligand:
cenL = lig.FindCenter()

#distance vector:
RL = cenL - cenR

#print their acutal distance:
distance = Norm(RL)
print "actual distance is ", distance, " Angstroms"

#if this distance is < 30A: do nothing and exit:
if (distance < 30.0):
	sys.exit()


#now calculates the unit vector along R-L axis:
uniV = RL.Normalize()

#so now we have to translate the ligand along the R-L axis
# by a vector equal (distance-30)(-uniV)

#creates -uniV:
min_uniV = Coord3D() - uniV
#this is tricky. Here is the explanation:
#-uniV alone is not allowed because the '-' operator doesn't work like that
# but Coord3D() alone creates a null vector.
# if we substract uniV to the null vector, we get what we want (ie -uniV)

#now we create the translation vector:
tvect = (distance-30.0)*min_uniV
print "we are going to move ligand by the vector", tvect

#do the translation:
lig.Translate(tvect)


#just to see if everything went fine:
cenL=lig.FindCenter()
RL= cenL-cenR
print "new distance: ", Norm(RL)


#write the new coordinates into "ligandname.translated":
WritePDB(lig, ligandname+".translated")