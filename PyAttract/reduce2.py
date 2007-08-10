#!/usr/bin/env python2.4


import sys
import copy

from ptools import *



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



defaultBB=[ [ 'N',    ['N']  , 30 , 0.0 ],
            ['CA',    ['CA'] ,  1 , 0.0 ],
            ['C' ,    ['C']  ,  1 , 0.0  ],
            ['O',     ['O']  ,  31, 0.0  ]
           ]

beadCorresp={}
beadCorresp["ARG"] = defaultBB + [[  'CG',        ['CG']        ,   3 ,  0.0  ],
                                 [  'NEC',      ['NE','CZ']     ,   4 ,  1.0   ]]

beadCorresp["GLU"] = defaultBB + [
                                  [   'CB',       ['CG']        ,   10  , 0.0  ] ,
                                  [ 'CO1',  ['CD', 'OE1', 'OE2'],   11  , -1.0 ]
                                  ]

beadCorresp["GLN"] = defaultBB + [
                                   [ 'CB',        ['CG']        ,   8,    0.0 ],
                                   [ 'CN1', ['CD', 'OE1', 'NE2'],   9,    0.0 ]
                                  ]


beadCorresp["LYS"] = defaultBB + [
                                   ['CB', ['CG'], 16, 0.0],
                                   ['CE', ['CE'], 17, 1.0]
                                 ]

beadCorresp["TRP"] = defaultBB + [
                                   ['CG', ['CG'], 25, 0.0],
                                   ['CSE', ['CD2','CE2','CE3','CH2','CZ3','CZ2'], 26, 0.0]
                                 ]

beadCorresp["MET"] =  defaultBB + [
                                    ['CSE', ['CB','CG'], 18, 0.0],
                                    ['CSE', ['SD','CE'], 19, 0.0]
                                  ]

beadCorresp["PHE"] = defaultBB + [
                                   ['CSE', ['CB','CG'], 20, 0.0],
                                   ['CSE', ['CD1','CD2','CE1','CE2','CZ'], 21, 0.0 ]
                                 ]

beadCorresp["TYR"] = defaultBB + [  ['CSE', ['CB','CG'], 27, 0.0],
                                    ['CSE', ['CD1','CD2','CE1','CE2','CZ','OH'],28,0.0]  ]

beadCorresp["HIS"] = defaultBB + [  ['CSE', ['CB','CG'], 12, 0.0],
                                    ['CSE', ['ND1','CD2','NE2','CE1'], 13, 0.0]  ]

beadCorresp["GLY"] = defaultBB



beadCorresp['ASN'] = defaultBB + [  ['CSE', ['CB', 'CG', 'OD1', 'ND2'] , 5, 0.0]  ]
beadCorresp["ALA"] = defaultBB + [  ['CSE',      ['CB'],                 2, 0.0]]
beadCorresp['ASP'] = defaultBB + [  ['CSE', ['CB', 'CG', 'OD1', 'OD2'] , 6 ,-1.0] ]
beadCorresp['CYS'] = defaultBB + [  ['CSE',    ['CB', 'SG']            , 7 , 0.]  ]
beadCorresp['ILE'] = defaultBB + [  ['CSE', ['CB', 'CG1', 'CG2', 'CD1'], 14, 0.]  ]
beadCorresp['LEU'] = defaultBB + [  ['CSE', ['CB', 'CG', 'CD1', 'CD2'] , 15, 0.]  ]
beadCorresp['PRO'] = defaultBB + [  ['CSE',     ['CB', 'CG', 'CD']     , 22, 0.]  ]
beadCorresp['SER'] = defaultBB + [  ['CSE',     ['CB', 'OG']           , 23, 0.]  ]
beadCorresp['THR'] = defaultBB + [  ['CSE',     ['CB', 'OG1', 'CG2']   , 24, 0.]  ]
beadCorresp['VAL'] = defaultBB + [  ['CSE',     ['CB', 'CG1', 'CG2']   , 29, 0.]  ]





allAtom=Rigidbody(sys.argv[1])
print "Number of atoms: " ,allAtom.Size()

#extract all 'atoms' objects
atoms=[]
for i in xrange(allAtom.Size()):
      atoms.append(allAtom.GetAtom(i))



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

for residKey, atomList in zip(residulist,orderedresid):
      residType=residKey[:3]
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
            print bead.ToPdbString(),  # ',' because of the extra \n caracter from the ptools C++ library
