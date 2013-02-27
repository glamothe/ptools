from ptools import *



prot = AttractRigidbody ("1A74_prot.red")
dna = AttractRigidbody ("1A74_opt.red")

cutoff=1000

forcefield = AttractForceField1("aminon.par",surreal(cutoff))




ener=forcefield.nonbon8(prot,dna, AttractPairList (prot, dna,cutoff))



print "ener :", ener , ", size prot : ",prot.Size() ,", size dna : ",dna.Size(),", size tot : ", prot.Size() +dna.Size()