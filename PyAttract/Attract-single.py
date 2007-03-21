#!/usr/bin/python

from ptools import *
import sys
import os
import time
import datetime
import math
import string







def PrintVect(vect):
    for i in range(len(vect)):
        print vect[i], " | ",
    print ''




def SingleMinim(rec, lig,cutoff,niter):
    FF=AttractForceField(rec,lig,cutoff)
    lbfgs=Lbfgs(FF)
    lbfgs.minimize(niter)
    X=lbfgs.GetX()
    return X




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
        rotdat=open('rotation.dat','r')
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
                phiii=jjj*phii
                for iii in range(self.nrot):
                    roti=iii*self.zwopi/self.nrot
                    self._rot.append((phii, ssii, roti))

    def __init__(self):
        self.read_rotdat()
    
    def __iter__(self):
        return self._rot.__iter__()



class Translation:
    def __init__(self):
        self.translat_dat=Rigidbody("translat.dat")
        print "Reading %i translations from translat.dat"%self.translat_dat.Size()

    def __iter__(self):
        self.i=0
        return self
    def next(self):
        if (self.i == self.translat_dat.Size()): raise StopIteration
        coord=self.translat_dat.GetCoords(self.i)
        self.i+=1
        return coord
        


def skipComments(attach):
    result=[]
    for line in attach.readlines():
        if line[0] not in ["#","!"]:
            result.append(line)
    return result



def readParams(filename):
    fich=open(filename,"r")
    clean=skipComments(fich)
    
    #nb of minimizations to perform is first integer of cleaned lines
    nbminim=int(clean.pop(0).split()[0])
    print "%i series of minimizations"%nbminim
    
    
    lignames=[]
    while(True):
        line=clean.pop(0).split()
        if line[0]=="Lig":
            lignames.append(line[2])
        else:
            break

    ignored=line #one more line is read in the loop before.
                 # (the xrst line which is ignored now)
    
    minimlist=[]
    for i in range(nbminim):
        line=clean.pop(0).split()
        minim=(int(line[0]),float(line[-1]))
        minimlist.append(minim)  #return a list of type (iter,squarecutoff)
                                 #other parameters are ignored
    return (nbminim, lignames, minimlist)








def main():
    
    
    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option("-s", action="store_true", dest="single",default=False)
    (options, args) = parser.parse_args()

    

    receptor_name=args[0]
    ligand_name=args[1]
    
    print """
    **********************************************************************
    **                ATTRACT  (Python edition)                         **
    **                version: 0.2                                      **
    ********************************************************************** """
    
    import locale
    import datetime
    

    #locale.setlocale(locale.LC_ALL, 'fr_FR')
    now = datetime.datetime.now()
    print now,"(",now.strftime("%A %B %d %Y, %H:%M"),")"


    (nbminim,lignames,minimlist) = readParams("attract.inp")
    



    rec=Rigidbody(receptor_name)
    lig=Rigidbody(ligand_name)
    print "Receptor (fixed) %s  has %d atoms" %(receptor_name,rec.Size())
    print "Ligand (mobile) %s  has %d atoms" %(ligand_name,lig.Size())
    
    
    
    if (not options.single):
        translations=Translation()
        rotations=Rotation()
    else:
        translations=[lig.FindCenter()]
        rotations=[(0,0,0)]
    
    transnb=0
    for trans in translations:
        transnb+=1
        print "###Performing translation nb###", transnb
        rotnb=0
        for rot in rotations:
            rotnb+=1
            print "Rotation nb %i"%rotnb
            minimcounter=0
            ligand=Rigidbody(lig)
            for minim in minimlist:
                minimcounter+=1
                print "minimization nb %i of %i"%(minimcounter,nbminim)
                cutoff=math.sqrt(minim[1])
                niter=minim[0]

                
                center=ligand.FindCenter()
                ligand.Translate(Coord3D()-center)
                AttractEuler(ligand,ligand,rot[0],rot[1],rot[2])
                ligand.Translate(trans)

                X=SingleMinim(rec,ligand,cutoff,niter)

                PrintVect(X)

                output=Rigidbody(ligand)
                center=output.FindCenter()
                output.Translate(Coord3D()-center)
                AttractEuler(output ,output, X[0], X[1], X[2])
                output.Translate(Coord3D(X[3],X[4],X[5]))
                output.Translate(center)

                output.PrintMatrix()
                ligand=Rigidbody(output)
                #WritePDB(output, "out.pdb")


            #check the rot/trans matrix:
            

    now = datetime.datetime.now()
    print "Finished at: ",now.strftime("%A %B %d %Y, %H:%M")


if __name__=="__main__":
    main()

