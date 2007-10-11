from ptools import *
import copy
from collections import *

from Numeric import *

def readModesVectors(filename='eignew.out'):
    
    start=0
    
    #reads the file:
    modefile=open(filename, 'r')
    filelines=modefile.readlines()
    splittedlines = [i.split() for i in filelines ]
    print splittedlines[0:2]
    
    
    i=0
    
    modes=[]
    eigens=[]
    
    try:
        while(True):
            mode=[]
            
            while(len(splittedlines[i])!=2):
                mode.extend(  [ float(a) for a in  splittedlines[i]])
                i+=1
            #print len(mode)
            if len(mode)!=0:
                modes.append(copy.deepcopy(mode))
            else:
                eigens.append(splittedlines[i][1])
            i=i+1
    except IndexError:
            pass

    return modes, eigens





#################################
## main program starts here     #
#################################

modes,eigens=readModesVectors()
print 'h', [len(i) for i in modes]
print eigens
print len(eigens), len(modes)



#loads rigidbody:
rig=Rigidbody('1FIN_c_r.red2')
assert( rig.Size()*3 == len(modes[0]))
rig=AttractRigidbody(rig)
rig.setTranslation(False)
rig.setRotation(False)



arrayOfModes = []

imode = 0
for mode in modes:
    imode+=1
    print "we have %i modes so far" %(imode)
    vcmode = VCoord3D()
    dmode = deque(mode)
    try:
        while(True):
            co=Coord3D()
            co.x = surreal(dmode.popleft())
            co.y = surreal(dmode.popleft())
            co.z = surreal(dmode.popleft())
            vcmode.append(co)
    except IndexError:
            #print type(vcmode)
            #print len(vcmode), rig.Size()
            arrayOfModes.append(vcmode)


for vcmode,eigen in zip(arrayOfModes,eigens):
    rig.addMode(vcmode,surreal(float(eigen)) )
    print "surreal(float(eigen)): ", surreal(float(eigen))
    print "eigen: " , eigen


for l in arange(-3,3, 0.5):
    rig.applyMode(0, surreal(l))
    WritePDB(rig, "fich%s.red"%(l+3))


#start a minimization:
lig = Rigidbody('1FIN_c_l.red2')
lig = AttractRigidbody(lig)
#lig.Translate(Coord3D(-1, -5, -7))

#lig.setTranslation(False)
#lig.setRotation(False)

ff2 = AttractForceField2('mbest1k.par', surreal(8))
ff2.AddLigand(rig)

ff2.AddLigand(lig)
minim = Lbfgs(ff2)
minim.minimize(500)

raise SystemExit

nminim = minim.GetNumberIter()
for i in range(nminim):
    v=minim.GetMinimizedVarsAtIter(i)
    rig.applyMode(0, surreal(v[0]))
    WritePDB(rig, "minim%s.red"%(i))



