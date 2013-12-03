# -*- coding: utf-8 -*-
from _ptools import *

one_letter_residue_dict = {
   "ALA": "A",
   "CYS": "C",
   "ASP": "D",
   "GLU": "E",
   "PHE": "F",
   "GLY": "G",
   "HIS": "H",
   "HIE": "H",
   "HSP": "H",
   "ILE": "I",
   "LYS": "K",
   "LEU": "L",
   "MET": "M",
   "ASN": "N",
   "PRO": "P",
   "GLN": "Q",
   "ARG": "R",
   "SER": "S",
   "THR": "T",
   "VAL": "V",
   "TRP": "W",
   "TYR": "Y",
   "---": "-",  
}


def pdbToSeq(rigid):
    """use residu names from the structure to extract the sequence
       This function needs CA atoms to be present. A missing CA atom will result in
       a missing letter in the sequence.
    """
    
    rca = rigid.CA().CreateRigid()  #restrict to the CA atoms.
    seq = []
    for i in range(len(rca)):
        at = rca.CopyAtom(i)
        seq.append(one_letter_residue_dict[at.residType])
    
    return "".join(seq)
    




def getPDB(pdbname):
    import urllib2
    pdb = urllib2.urlopen("http://www.rcsb.org/pdb/files/%s.pdb"%pdbname)
    #content = pdb.read()
    
    rigid = Rigidbody(pdb)
    
    return rigid


