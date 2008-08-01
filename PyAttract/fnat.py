#!/usr/bin/env python

from ptools import *
import math
import sys
import os
from stat import *

def contact(receptor, ligand):
    "return residues in interaction, use ptools::pairlist"
    assert(isinstance(receptor,Rigidbody))
    assert(isinstance(ligand,Rigidbody))
    
    
    resnblig = []
    for i in range(ligand.Size()):
        at = ligand.GetAtom(i)
        resnblig.append(at.GetResidId())
    resnbrec = []
    for j in range(receptor.Size()):
        at = receptor.GetAtom(j)
        resnbrec.append(at.GetResidId())
    
    
    pl = PairList(receptor,ligand,7)
    contactnat = {} # residue list in interaction

    for i in range(pl.Size()):
        ap = pl[i]
        contactnat[(resnbrec[ap.atrec], resnblig[ap.atlig])] = True
    
    return contactnat.keys()

def fnat(receptor, ligcrist, ligprobe):
    "return native fraction (fnat)"
    resid= {}  # residue number of the ith atom
    corig = contact(receptor,ligcrist)
    cnew = contact(receptor,ligprobe)
    intersect  = [ i for i in corig if i in cnew ]
    f = float(len(intersect))/float(len(corig))
    return f

def main():

    recname = sys.argv[1]
    ligname = sys.argv[2]
    ligname2 = sys.argv[3]
    lig = Rigidbody(ligname)
    lig2 = Rigidbody(ligname2)
    rec = Rigidbody(recname)

    FNAT=fnat(rec,lig,lig2)
    print FNAT

if __name__ == "__main__":
    main()








