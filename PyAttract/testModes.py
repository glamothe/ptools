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
print [len(i) for i in modes]
print eigens
print len(eigens), len(modes)



#loads rigidbody:
rig=Rigidbody('1FIN_c_r.red2')
assert( rig.Size()*3 == len(modes[0]))
rig=AttractRigidbody(rig)



arrayOfModes = []

for mode in modes:
    vcmode = VCoord3D()
    dmode = deque(mode)
    try:
        while(True):
            co=Coord3D()
            co.x = dmode.popleft()
            co.y = dmode.popleft()
            co.z = dmode.popleft()
            vcmode.append(co)
    except IndexError:
            #print type(vcmode)
            #print len(vcmode), rig.Size()
            arrayOfModes.append(vcmode)


for vcmode,eigen in zip(arrayOfModes,eigens):
    rig.addMode(vcmode,float(eigen) )
    print "eigen: " , eigen


for l in arange(-3,3, 0.5):
    rig.applyMode(0, l)
    WritePDB(rig, "fich%s.red"%(l+3))


#start a minimization:
lig = Rigidbody('1FIN_c_l.red2')
lig = AttractRigidbody(lig)
lig.Translate(Coord3D(-1, -5, -7))

ff2 = AttractForceField2('mbest1k.par',rig, lig, 50)
minim = Lbfgs(ff2)
minim.minimize(10)

