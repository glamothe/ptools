#!/usr/bin/env python


import sys
import copy

from ptools import *
from ordereddict import OrderedDict


class IncompleteBead:
      pass

class BeadCreator:

      def __init__(self, reducedname, reducedtypenb, reducedcharge,lstofAtoms):
            self._reducedname=reducedname
            self._reducedtypenb=reducedtypenb
            self._reducedcharge=reducedcharge
            self._lstofAtoms=copy.deepcopy(lstofAtoms)
            self._CoM=Coord3D()  #from ptools
            self.size=0

            atProp=Atomproperty()
            #print "*******reducedname: ",reducedname
            atProp.SetType(reducedname)
            atProp.SetAtomCharge(reducedcharge)
            atProp.SetChainId('')
            self.atProp=atProp

      def submit(self, atom):
            "try to add an atom to the bead"
            atomtype=atom.GetType()
            #trick to handle 'OTn' instead of 'O' for last pdb atom:
            if atomtype[:2]=='OT': atomtype='O'
            if atomtype in self._lstofAtoms:
                  self._CoM+=atom.GetCoords()
                  self._lstofAtoms.remove(atomtype)
                  self.size+=1
                  #print self.size
      def create(self):
            "creates a new atom bead"
            if len(self._lstofAtoms)!=0:
                  raise IncompleteBead
            CoM=self._CoM*(1.0/float(self.size))
            at=Atom(self.atProp,CoM)
            return at

###insert load parameter file here


#read parameter file:
parameters = open("at2cg.scorpion.dat", 'r').readlines()

beadCorresp = {}

#list of all residues names
#keys are residues names
#values are coarse grain names 
#ie: residNames['ALA'] == {'CA':1 ,'CB':1 }
residNames = {}

#grainMap:
#keys are like this: 'ALA:CA', 
grainMap = {}

initalBeadCharge= {
      'ASP:CB': -1,
      'GLU:CG': -1,
      'ARG:CG': +1, 
      'LYS:CG': 1
}



allAtomRadius = {}
allAtomCharges = {}
beadRadius = {}


for p in parameters:
  lspl = p.split() 
  try:  
    if lspl[0] != '#':
        listofgrains = residNames.get(lspl[0], OrderedDict() )
        listofgrains[lspl[5]] = 1    #add coarse grain name    
        residNames[lspl[0]] = listofgrains

        
        listOfAtomsInAGrain = grainMap.get("%s:%s"%(lspl[0], lspl[5]), [] ) 

        listOfAtomsInAGrain.append(dict(  atomname=lspl[1],  
                                          atomcharge=float(lspl[2]),
                                          atomradius=float(lspl[3]),
                                          weight=float(lspl[4]),
                                          beadname=lspl[5],
                                          beadid=int(lspl[6]),
                                          beadradius=float(lspl[7]),
                                        ) 
                                   )

        allAtomRadius["%s:%s"%(lspl[0], lspl[1]) ] = float(lspl[3]) 
        allAtomCharges["%s:%s"%(lspl[0], lspl[1]) ] = float(lspl[2])
        beadRadius["%s:%s"%(lspl[0], lspl[5])  ] = float(lspl[7]) 

        grainMap["%s:%s"%(lspl[0], lspl[5]) ] = listOfAtomsInAGrain

 
  except IndexError:
    pass


#for i,j in grainMap.items():
#    print i, j


beadCorresp = {}

for residname, cgnames in residNames.items():
    for cgname in cgnames:
        key = "%s:%s"%(residname, cgname)
        lstOfAllAtoms = [i['atomname'] for i in grainMap[key] if i['weight']!=0 ]
        beadid = grainMap[key][0]['beadid']
        beadcharge = initalBeadCharge.get(key, 0.0)
        beadname = grainMap[key][0]['beadname']

        beadDescription = [beadname, lstOfAllAtoms, beadid, beadcharge]
    
        descriptions =  beadCorresp.get(residname, [])
        descriptions.append(beadDescription)
        beadCorresp[residname] = descriptions

print beadCorresp
####



allAtom=Rigidbody(sys.argv[1])
sys.stderr.write("Number of atoms: %d\n" %(len(allAtom) ))

#extract all 'atoms' objects
atoms=[]
for i in xrange(len(allAtom) ):
      atoms.append(allAtom.CopyAtom(i))



#count residues:
residuMap={}
residulist=[]
for at in atoms:
      residueIdentifier = at.GetResidType() + str(at.GetChainId())  + str(at.GetResidId())
      #residueIdentifier is like "LEU296"
      residuMap.setdefault(residueIdentifier, []).append(at)
      if residueIdentifier not in residulist:
            residulist.append(residueIdentifier)

sys.stderr.write("Number of residues: %i\n" %(len(residuMap)))
sys.stderr.write("Start atom of each residue:\n")
orderedresid=[residuMap[i] for i in residulist ]
startatoms=[lat[0].GetAtomId() for lat in orderedresid ]
out = ""
for statom in startatoms:
      out+=(str(statom))+" "
sys.stderr.write(out+"\n")


#iterates through all the residues and create reduced beads:

totAtoms=0

protein = [] 

for residKey, atomList in zip(residulist,orderedresid):
      residType=residKey[:3]
      if (residType)=="HIE": residType="HIS" #fix for an amber output file
      residNumber=int(residKey[4:])
      #print "reducing residue %s of type %s" % (residKey, residType)
      correspList=beadCorresp[residType]
      #print correspList
      for correspUnit in correspList:
            atomTypeName=correspUnit[0]
            lstToReduce=correspUnit[1]
            atomTypeNumber=correspUnit[2]
            atomCharge=correspUnit[3]
            beadcreator=BeadCreator(atomTypeName,atomTypeNumber, atomCharge, lstToReduce)
            for atom in atomList:
                  beadcreator.submit(atom)
            try:
                  bead = beadcreator.create()
            except IncompleteBead:
                  print "The bead %i of residue %s is incomplete. Please check your pdb!"\
                      %(totAtoms+1,residKey)
                  raise 
            totAtoms+=1
            #now we must modify the bead: change the residue type and set the "extra" field correctly
            bead.SetResidType(residType)
            extra = ('%5i'+'%8.3f'+'%2i'*2) %(atomTypeNumber,atomCharge,0, 0)
            bead.SetExtra(extra)
            bead.SetAtomId(totAtoms)
            bead.SetResidId(residNumber)
            protein.append(bead)
            print bead.ToPdbString(),  # ',' because of the extra \n caracter from the ptools C++ library




from cgopt import optimize

charge = []
radius = []
cx = []
cy = []
cz = []

cgch = []
cgr = []
cgx = []
cgy = []
cgz = []



for i in range(len(allAtom)):
   atom = allAtom.CopyAtom(i)
  # print dir(atom)
#   residu_name = atom. 
   residu_type= atom.GetResidType() 
   atomtype = atom.GetType()
   key = "%s:%s"%(residu_type, atomtype) 
   radius.append(  allAtomRadius[key] ) 
   charge.append ( allAtomCharges[key]   )
   cx.append( atom.GetCoords().x)
   cy.append( atom.GetCoords().y)
   cz.append( atom.GetCoords().z)
   print  key ,  charge[i], radius[i], cx[i], cy[i], cz[i]
   
  
for i, atom in enumerate(protein):
   
   cgch.append( atom.GetAtomCharge() ) 
   
   residu_type= atom.GetResidType() 
   atomtype = atom.GetType()
   key = "%s:%s"%(residu_type, atomtype) 

   cgr.append( beadRadius[key] )  
   cgx.append( atom.GetCoords().x)
   cgy.append( atom.GetCoords().y)
   cgz.append( atom.GetCoords().z)

   print key, cgch[i], cgr[i], cgx[i], cgy[i], cgz[i]

optimized = optimize(len(allAtom), charge, radius, cx, cy, cz, len(protein), cgch, cgr, cgx, cgy, cgz )

print "optimized charges: ", optimized
#rig = AttractRigidbody()

for i, bead in enumerate(protein):
    #rig.AddAtom(bead)
    bead.SetAtomCharge(optimized[i])
    print bead.ToPdbString(),

