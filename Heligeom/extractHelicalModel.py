#!/usr/bin/env python

#exemple:
#python separateEnerInDockingResult.py ../cgPyAttract/1A74_bound-prot.red ../shortDNA/dna.3pb.red ../dockingPyAttract/1A74_3pb.att


import sys
import re
from ptools import *
import math
import string

def changeChain(rig,letter):
    rsize = rig.Size()
    for i in xrange(0,rsize):
        at=rig.GetAtomProperty (i)
        at.SetChainId(letter)
        rig.SetAtomProperty (i,at)
    return rig

def extend(hp,mono1,nb,Z=False):
    final = Rigidbody()
    monoTest = mono1.SelectAllAtoms ().CreateRigid()
    i=0
    O = hp.point
    axe =hp.unitVector
    if Z == True:
        #align on Z
        #1 make Z axis, unit prot axis
        at = Atomproperty()
        
        Zaxis = Rigidbody()
        O=Coord3D(0,0,0)
        Zaxis.AddAtom (at,O)
        axe = Coord3D(0,0,1)
        Zaxis.AddAtom (at,Coord3D(0,0,1))
        
        Protaxis = Rigidbody()
        Protaxis.AddAtom (at,hp.point)
        Protaxis.AddAtom (at,hp.point+hp.unitVector.Normalize()) 
        #2 superpose and get matrix
        m = superpose (Zaxis,Protaxis).matrix
        #3 apply matrix to rigidbody
        monoTest.ApplyMatrix (m)
    #4 modify axis
    #etend la structure pdb.
    monoTest = changeChain(monoTest,string.ascii_uppercase[i%26])
    i+=1
    final = final + monoTest
    for j in xrange(nb-1):
        monoTest.ABrotate ( O, O+ axe, hp.angle )
        monoTest.Translate( axe * hp.normtranslation )
        monoTest = changeChain(monoTest,string.ascii_uppercase[i%26])
        final = final + monoTest
        i+=1
    return final

def rigidXMat44(rigid, mat):
    assert(isinstance(rigid,AttractRigidbody))
    out=AttractRigidbody(rigid)
    for i in range(rigid.Size()):
        coords=rigid.GetCoords(i)
        coords2=Coord3D()
        coords2.x = mat[0][0]*coords.x + mat[0][1]*coords.y + mat[0][2]*coords.z + mat[0][3]
        coords2.y = mat[1][0]*coords.x + mat[1][1]*coords.y + mat[1][2]*coords.z + mat[1][3]
        coords2.z = mat[2][0]*coords.x + mat[2][1]*coords.y + mat[2][2]*coords.z + mat[2][3]
        out.SetCoords(i, coords2)
    return out


nargs = len(sys.argv)
if nargs < 4:
    print "usage: separateEnerDockingResult.py  out.att receptor.red Trans Rot [nbTurn]"
    raise SystemExit

filin = open(sys.argv[1],"r")
rec = AttractRigidbody(sys.argv[2])
trans = sys.argv[3]
rot = sys.argv[4]

nbturn = 1
if 5 < nargs < 7:
    nbturn = int(sys.argv[5])

goodMatrix = False

for ligne in filin :
	if ligne.startswith("==") :
                l = ligne.split()
                if l[1] == trans and l[2] == rot: 
                    pilot= float(ligne.split()[3])		
                    liste = []
                    liste.append(ligne)
                    matrix=[]
                    goodMatrix =True

 	elif goodMatrix and ligne.startswith("MAT") :
                liste.append(ligne)
		lspl=ligne.split( )
                matrix.append( [float(lspl[i]) for i in range(1,5) ]  )
		
			
	elif goodMatrix and "MAT END" in ligne :
		#utiliser matrix
		templig = rigidXMat44(rec, matrix)
                hp = MatTrans2screw(superpose (rec,templig).matrix)
                print extend(hp,rec,nbturn*int(360./abs(math.degrees(hp.angle))),Z=False).PrintPDB()
                break
filin.close()
