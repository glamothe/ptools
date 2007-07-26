#!/usr/bin/python2.4

from ptools import *
import sys


#rig = Rigidbody("1FIN_c_r.red")
#rig2 = Rigidbody("1FIN_c_l.red")

#FF=AttractForceField(rig2,rig,10)

#FF=TestForceField()

def printvec(X):
	for i in X:
		print i,'|',
	print ""


class PyForceField(ForceField):
    def __init__(self):
        ForceField.__init__(self)
    def Function(self, vec):
	print "============="
        print "len(vec): ", len(vec)
        sol=(vec[0]-3)**2
        print "fonction: ", sol
	print "vector: ", list(vec)
        return sol
    def Derivatives(self, vec1, grad):
        #grad[0]=2*vec1[0]-6
        grad[0]=2*vec1[0]-6
        print "X: ", 
        printvec(vec1)
        print "Grad: ",
        printvec(grad)
    def ProblemSize(self):
        return 1

FF=PyForceField()



#print FF.Electrostatic()
#print FF.LennardJones()
#print FF.Energy()

minim=Lbfgs(FF)
minim.minimize(500)

X=Vdouble()
G=Vdouble()

def initvec(vec):
	for i in range(6):
		vec.append(0)
initvec(X)
initvec(G)



printvec(X)
printvec(G)


#FF.Function(X)
#FF.Derivatives(X,G)

#printvec(G)

