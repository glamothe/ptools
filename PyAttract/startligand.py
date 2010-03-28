#!/usr/bin/env python
#
#  This script takes a translation file, a rotation file and a ligand file
# and generates the ligand initial placement prior to the minimization step.


from ptools import *
import sys
import os
import math
import string



class Rotation:
    
    class _Rot:
        ssi=0.0
        phi=0.0
        rot=0.0
        def __init__(self, ssii, phii, roti):
            self.ssi=ssii
            self.phi=phii
            self.rot=roti
    
    def read_rotdat(self):
        self.zwopi=2.0*3.14159265
        self.NbRotByTrans=0
        self.theta=[]
        self.nphi=[]
        # read theta,phi,rot data
        rotdat=open( rotfile ,'r')
        line=rotdat.readline().split()
        self.ntheta=int(line[0])
        self.nrot=int(line[1])
        print "ntheta, nrot: %i %i" %(self.ntheta,self.nrot)
        for i in range(self.ntheta):
            line=rotdat.readline().split()
            self.theta.append(float(line[0]))
            self.nphi.append(int(line[1]))
            self.NbRotByTrans=self.NbRotByTrans+self.nphi[i]*self.nrot
            self.theta[i]=self.zwopi*self.theta[i]/360.0
            print self.theta[i],self.nphi[i]
        rotdat.close()
        self._rot=[]
        
        print "%i rotations by translation"%self.NbRotByTrans

        for kkk in range(self.ntheta):
            ssii=self.theta[kkk]
            phii=self.zwopi/self.nphi[kkk]
            for jjj in range(self.nphi[kkk]):
                phiii=(jjj+1)*phii
                for iii in range(self.nrot):
                    roti=(iii+1)*self.zwopi/self.nrot
                    self._rot.append((phiii, ssii, roti))


    def __init__(self):
        self.read_rotdat()
    
    def __iter__(self):
        return self._rot.__iter__()




###########################
##  MAIN   PROGRAM        #
###########################
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-t", "--translationfile", action="store", type="string", dest="transfile", help="translation file")
parser.add_option("-r", "--rotationfile", action="store", type="string", dest="rotfile", help="rotation file")
(options, args) = parser.parse_args()

if len(args) == 3:
    ligand_name=args[0]
    transnb = int(args[1])
    argrotnb = int(args[2])
else:
    sys.exit("""ERROR: missing argument
Usage: startligand.py ligand_file translation_number rotation_number """)

if (not options.transfile):
    transfile = "translation.dat"
if (not options.rotfile):
    rotfile = "rotation.dat"

print "Translation file:", transfile
print "Rotation file:", rotfile

lig=Rigidbody(ligand_name)
print "Ligand (mobile partner) %s  has %d particules" %(ligand_name,lig.Size())


trans=Rigidbody( transfile )
co=trans.GetCoords(transnb)
translations=[[transnb+1,co]]
rotations = Rotation()

for trans in translations:
    #transnb+=1
    print "@@@@@@@ Translation nb %i @@@@@@@" %(transnb)
    for rotnb,rot in enumerate(rotations):
        rotnb+=1
        if rotnb == argrotnb:
            print "----- Rotation nb %i -----"%rotnb
            minimcounter=0
            ligand=AttractRigidbody(lig)

            center=ligand.FindCenter()
            ligand.Translate(Coord3D()-center) #set ligand center of mass to 0,0,0
            ligand.AttractEulerRotate(surreal(rot[0]),surreal(rot[1]),surreal(rot[2]))
            ligand.Translate(trans[1])
            WritePDB(ligand, "ligand_%i_%i.red"%(transnb,argrotnb))


