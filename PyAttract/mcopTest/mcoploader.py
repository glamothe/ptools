
from ptools import *

def arbload(name):
    a = Rigidbody(name)
    return AttractRigidbody(a)


def mcopload(filename):
    f=open(filename,'r')
    mcobj= Mcoprigid()
    for l in f.readlines():
        if l[0]=="#": break
        lspl = l.split()
        if lspl[0]=="main":
            main = arbload(lspl[1])
            mcobj.setMain(main)
        if lspl[0]=="copy":
            reg = Region()
            filelist = lspl[1:]
            for f in filelist:
                c = arbload(f)
                reg.addCopy(c)
            mcobj.addEnsemble(reg)
    return mcobj









if __name__=="__main__":
    print mcopload("receptor.mcop")