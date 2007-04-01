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
        return [self.i,coord]
        


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




def rigidXstd_vector(rigid, mat_std):
    
    #create a 4x4 matrix from a linear std_vector_double
    mat=[]
    for iline in range(4):
        line=[]
        for icol in range(4):
            line.append(mat_std[iline*4+icol])
        mat.append(line)

    out=Rigidbody(rigid)
    for i in range(rigid.Size()):
        coords=rigid.GetCoords(i)
        coords2=Coord3D()
        coords2.x = mat[0][0]*coords.x + mat[0][1]*coords.y + mat[0][2]*coords.z + mat[0][3]
        coords2.y = mat[1][0]*coords.x + mat[1][1]*coords.y + mat[1][2]*coords.z + mat[1][3]
        coords2.z = mat[2][0]*coords.x + mat[2][1]*coords.y + mat[2][2]*coords.z + mat[2][3]
        out.SetCoords(i, coords2)
    return out






def main():


    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option("-s", "--single", action="store_true", dest="single",default=False,help="single minimization mode")
    parser.add_option("--ref", action="store", type="string", dest="reffile", help="reference ligand for rmsd" )
    parser.add_option("-t", "--translation", action="store", type="int", dest="transnb", help="translation number (distributed mode) starting from 0 for the first one!")
    (options, args) = parser.parse_args()



    receptor_name=args[0]
    ligand_name=args[1]

    print """
    **********************************************************************
    **                ATTRACT  (Python edition)                         **
    **                version: 0.3                                      **
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
    
    if (options.single and options.transnb):
        parser.error("options -s and -t are mutually exclusive")
    
    
    if (options.reffile):
        print "using reference file: %s"%options.reffile
        ref=Rigidbody(options.reffile)


    if (not options.single):
        translations=Translation()
        rotations=Rotation()
    else:
        translations=[[1,lig.FindCenter()]]
        rotations=[(0,0,0)]
        print "Single mode"

    transnb=0
    if (options.transnb!=None):
        trans=Rigidbody("translat.dat")
        co=trans.GetCoords(options.transnb)
        translations=[[options.transnb+1,co]]
        transnb=options.transnb

    
    for trans in translations:
        transnb+=1
        print "@@@@@@@ Translation nb %i @@@@@@@" %(transnb)
        rotnb=0
        for rot in rotations:
            rotnb+=1
            print "----- Rotation nb %i -----"%rotnb
            minimcounter=0
            ligand=Rigidbody(lig)

            center=ligand.FindCenter()
            ligand.Translate(Coord3D()-center)
            AttractEuler(ligand,ligand,rot[0],rot[1],rot[2])
            ligand.Translate(trans[1])

            for minim in minimlist:
                minimcounter+=1
                cutoff=math.sqrt(minim[1])
                niter=minim[0]
                print "{{ minimization nb %i of %i ; cutoff=%.2f(A) ; maxiter=%d"%(minimcounter,nbminim,cutoff,niter)

                X=SingleMinim(rec,ligand,cutoff,niter)

                #PrintVect(X)

                output=Rigidbody(ligand)
                center=output.FindCenter()
                output.Translate(Coord3D()-center)
                AttractEuler(output ,output, X[0], X[1], X[2])
                output.Translate(Coord3D(X[3],X[4],X[5]))
                output.Translate(center)

                
                ligand=Rigidbody(output)
                #WritePDB(output, "out.pdb")
                #vec=output.GetMatrix()


                #testout=rigidXstd_vector(lig, vec)


            if (options.reffile):
                rms=Rmsd(ref.CA(), output.CA())
            else:
                rms="XXXX"


            #calculates true energy, and rmsd if possible
            FF=AttractForceField(ligand, rec, 500)
            print "%4s %6s %6s %13s %13s"  %(" ","Trans", "Rot", "Ener", "RmsdCA_ref")
            print "%-4s %6d %6d %13.7f %13s" %("==", transnb, rotnb, FF.Energy(), str(rms))
            output.PrintMatrix()


            


    now = datetime.datetime.now()
    print "Finished at: ",now.strftime("%A %B %d %Y, %H:%M")


if __name__=="__main__":
    main()

