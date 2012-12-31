from ptools import *
import math
import sys
import random
import string

def ScalProd(a,b):
    return a.x * b.x + a.y * b.y + a.z * b.z

def VectProd(u,v):
    UvectV = Coord3D()
    UvectV.x = u.y * v.z - u.z * v.y 
    UvectV.y = u.z * v.x - u.x * v.z 
    UvectV.z = u.x * v.y - u.y * v.x
    return UvectV 

def residSize(rig):
    rsize = 0
    temp = -1
    for i in xrange(rig.Size()):
        rid = rig.GetAtomProperty (i).GetResidId ()
        if rid != temp:
            temp = rid
            rsize +=1
    return rsize
        
        
def test(hp,mono1,mono2):

        monoTest = mono1.SelectAllAtoms ().CreateRigid()
        monoTest.ABrotate ( hp.point, hp.point + hp.unitVector, hp.angle )
        monoTest.Translate( hp.unitVector * hp.normtranslation )
        
        monoTrue = mono1.SelectAllAtoms ().CreateRigid()
        m = superpose (mono2,mono1).matrix
        monoTrue.ApplyMatrix (m)
        rmsd = Rmsd (monoTest.SelectAllAtoms (), monoTrue.SelectAllAtoms ())
        
        return rmsd

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
    


    
def main():
    nargs = len(sys.argv)
    if nargs < 3:
        print "usage: heligeom.py monomer1.pdb monomer2.pdb [numberOfNewMonomer] [-Z]"
        print ""
        print ""
        print "where  monomer1.pdb and monomer2.pdb are the name of the pdb file of your monomer"
        print "and numberOfNewMonomer is an optional argument for the number of new monomer you want to add to make a helicoidal structure"
        print "the new (optional) pdb file is printed on the standar output and the parameter of the helice and the estimated quality are redirected on the error output"
        print "with the -Z option, the generated pdb file is aligned on the Z axis"
        raise SystemExit

    mono1 = Rigidbody( sys.argv[1])
    mono2 = Rigidbody( sys.argv[2])
    hp = MatTrans2screw(superpose (mono2,mono1).matrix)
    #hp,succes = computeParam(mono1,mono2,a,b)
    #print >> sys.stderr, "resid number :",a,b

    rmsd=test(hp,mono1,mono2)
    #print >> sys.stderr,"quality (Rmsd):",rmsd
    #print >> sys.stderr," "
    print >> sys.stderr,""
    print >> sys.stderr,"P:\t%0.2f\t%0.2f\t%0.2f\n"%(hp.point.x,hp.point.y,hp.point.z)+"omega:\t%0.2f\t%0.2f\t%0.2f\n"%(hp.unitVector.x,hp.unitVector.y,hp.unitVector.z)+"angle theta:\t radian: %0.2f"%(hp.angle)+"\t degree: %0.2f"%(math.degrees(hp.angle))+"\ntrans\t\t\t%0.2f"%(hp.normtranslation)
    print >> sys.stderr,""
    print >> sys.stderr,"monomer per turn:\t%0.2f"%( 360./abs(math.degrees(hp.angle)))
    print >> sys.stderr,"pitch:\t\t\t%0.2f"%(hp.normtranslation*(360./abs(math.degrees(hp.angle))))#"%0.2f"%(43210.1234567)
    
    if hp.angle * hp.normtranslation > 0:
        sens = "right-handed"
    else: sens = "left-handed"
    print >> sys.stderr,"Helix sense : \t\t"+sens
    print >> sys.stderr,""
    if nargs >= 4:
        Z=False
        if nargs >= 5:
            Z = True
        final = extend(hp,mono1,int (sys.argv[3]),Z)
        print final.PrintPDB()
if __name__ == "__main__":
    main()
