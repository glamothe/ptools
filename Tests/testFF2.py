from ptools import *

a = Rigidbody("pk6a.red")
a = AttractRigidbody(a)
b = Rigidbody("pk6c.red")
b = AttractRigidbody(b)

ff = AttractForceField2("mbest1k.par", 20.0)
ff.AddLigand(a)
ff.AddLigand(b)

x = Vdouble()
for i in range(6):
    x.append(0)
print ff.Function(x)
