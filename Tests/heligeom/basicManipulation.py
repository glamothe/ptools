from ptools import *


#translate the DNA in coarse grain representation
dna = DNA ("bp.red.pdb","generate_B_DNA.expected")


#add a base Pair
bp = BasePair (dna[0].GetRigidBody())
dna.Add(bp)


#add itself
new = DNA (dna)
dna.Add(new,BDNA())


#change the type of a base
dna.ChangeType (0,"A","bp.red.pdb")


#turn the center base 
dna.ApplyLocal (Roll(30), dna.Size()/2)


#trim the extremities
dna = dna.SubDNA (2, dna.Size()-3)

#change to a All Atom representation
dna.ChangeRepresentation ("bp.ato.pdb")


print dna.PrintPDB() 

