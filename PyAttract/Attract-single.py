#!/usr/bin/python2.4

from ptools import *
import sys
import os
import time
import datetime




def PrintVect(vect):
    for i in range(len(vect)):
        print vect[i], " | ",
    print ''








def main():
    
    receptor_name=sys.argv[1]
    ligand_name=sys.argv[2]
    
    print """
    **********************************************************************
    **                ATTRACT SINGLE (Python edition)                   **
    **                version: 0.1                                      **
    ********************************************************************** """
    
    import locale
    import datetime
    

    #locale.setlocale(locale.LC_ALL, 'fr_FR')
    now = datetime.datetime.now()
    print now,"(",now.strftime("%A %B %d %Y, %H:%M"),")"

    cutoff=10
    
    rec=Rigidbody(receptor_name)
    lig=Rigidbody(ligand_name)
    print "Receptor (fixed) %s  has %d atoms" %(receptor_name,rec.Size())
    print "Ligand (mobile) %s  has %d atoms" %(ligand_name,lig.Size())
    
    print "initializing Force Field"
    FF=AttractForceField(rec,lig,cutoff)
    
    lbfgs=Lbfgs(FF)
    lbfgs.minimize(500)
    X=lbfgs.GetX()
    PrintVect(X)

    output=Rigidbody(lig)
    AttractEuler(lig ,output, X[0], X[1], X[2])
    output.Translate(Coord3D(X[3],X[4],X[5]))
    WritePDB(output, "out.pdb")




if __name__=="__main__":
    main()







