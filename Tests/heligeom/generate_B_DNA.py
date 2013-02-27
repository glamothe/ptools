from ptools import *

dna = DNA ("bp.ato.pdb","AAAAAATCGATCTATC",ADNA()) #tout atom
#dna = DNA ("bp.red.pdb","AATCGAATCG",BDNA) #gros grain

print dna.PrintPDB()
