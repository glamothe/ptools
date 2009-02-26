from ptools import *
from mcoploader import *



m=Rigidbody("1FIN_r_main.red")
am =AttractRigidbody(m)
c1 = Rigidbody ("1FIN_r_1_2.red")
c2 = Rigidbody ("1FIN_r_1_1.red")

ac1 =AttractRigidbody (c1)
ac2 =AttractRigidbody (c2)


reg=Region()
reg.addCopy(ac1)
reg.addCopy(ac2)


mcrec = Mcoprigid()
mcrec.setMain(am)
mcrec.addEnsemble(reg)



lig=Rigidbody ("1FIN_c_l.red")
alig=AttractRigidbody(lig);

#mclig=Mcoprigid mclig ;
#mclig.setMain(alig);


myrec = mcopload("receptor.mcop")




ff = McopForceField(factoryCreator1(), "aminon.par", 15, myrec, lig)



X=Vdouble()
for i in range(6):
    X.append(0)
    
print ff.ProblemSize()

print ff.Function(X)

