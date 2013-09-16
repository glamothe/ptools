#!/usr/bin env python

#return a list of contacted residues on the receptor

from ptools import *
import sys

import argparse


def main(args):
    receptor = AttractRigidbody(args.receptor)
    ligand = AttractRigidbody(args.ligand)
        
    resnbrec = []  #resnbrec[i] is the residue id of atom i
    for j in range(len(receptor)):
        at = receptor.CopyAtom(j)
        resnbrec.append(at.residId)
    
    
    pl = AttractPairList(receptor,ligand, args.cutoff)
    
    
    atomrec = [i.atrec for i in pl]  #list of contacted atoms on the receptor
    residusrec = sorted(list({resnbrec[i] for i in atomrec})) #list of contacted residues on the receptor
    
    for r in residusrec:
        print r,
        
    print ""



if __name__=="__main__":
    
    parser = argparse.ArgumentParser(description="calculate a list of contacted residues on the receptor")
    parser.add_argument("--receptor", dest="receptor")
    parser.add_argument("--ligand", dest="ligand")
    parser.add_argument("--cutoff", dest="cutoff", type=int, default=7, help="cutoff used to determine if two residues interact. Default to 7A (for coarse grained templates). You should set the cutoff to 5 when working on all atoms molecules")
    
    args = parser.parse_args()
    
    main(args)