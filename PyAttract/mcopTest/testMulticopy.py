from ptools import *

#creation recepteur multicopy:

m=Rigidbody("mainR.pdb")
am = AttractRigidbody(m)
c1 = Rigidbody("copy1R.pdb")
c2 = Rigidbody("copy2R.pdb")

ac1 = AttractRigidbody(c1)
ac2 = AttractRigidbody(c2)


reg = Region()
reg.addCopy(ac1)
reg.addCopy(ac2)


mcrec = Mcoprigid()
mcrec.setMain(am)
mcrec.addEnsemble(reg)




#ligand
lig = Rigidbody("mainL.pdb")
alig = AttractRigidbody(lig)

mclig = Mcoprigid()
mclig.setMain(alig)

#forcefield:

ff = AttractForceField1("aminon.par", 12.)
FF = McopForceField(ff, 12.)

FF.setLigand(mclig)
FF.setReceptor(mcrec)

v=Vdouble()
for i in range(6):
    v.append(surreal(0))


FF.calculate_weights(mclig, True )
print FF.Function(v)



