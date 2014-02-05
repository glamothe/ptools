#!/usr/bin/env python

import argparse

from ptools import *
import sys


def toString(rigid):
    out = []
    for i in range(len(rigid)):
       at = rigid.CopyAtom(i)
       out.append(at.ToPdbString())

    return "".join(out)



if __name__=="__main__":
    
    parser = argparse.ArgumentParser(description="quick and dirty filter for PDB atoms")
    parser.add_argument("--ca", dest="ca", action="store_true", default=False, help="filter C alpha atoms")
    parser.add_argument("--heavy", dest="heavy", action="store_true", default=False, help="filter heavy atoms (remove H)")
    #parser.add_argument("--side", dest="side", action="store_true", default=False, help="filter side-chain")
   
    parser.add_argument("--bb", dest="bb", action="store_true", default=False, help="filter backbone")

    parser.add_argument("filename", help="pdb file to filter. Use '-' for stdin")

    
    args = parser.parse_args()


    #read the PDB file:
    if args.filename=="-":
        #we must read from stdin
        file = sys.stdin
        r = Rigidbody(file)

    else:
        r = Rigidbody(args.filename)

    

    # filter CA if needed:
    if args.ca:
        r = r.CA().CreateRigid()

        
    # filter heavy atoms if needed:
    if args.heavy:
        r_sel = ~r.SelectAtomType('H*')  #select not H atoms...
        r = r_sel.CreateRigid()

    #filter backbone:
    if args.bb:
        r = r.Backbone().CreateRigid()

    print toString(r)
