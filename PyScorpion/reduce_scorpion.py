
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


for p in parameters:
  lspl = p.split() 
  try:  
    if lspl[0] != '#':
        listofgrains = residNames.get(lspl[0], {})
        listofgrains[lspl[5]] = 1    #add coarse grain name    
        residNames[lspl[0]] = listofgrains

        grainMap["%s:%s"%(lspl[0], lspl[5])] = dict(atomname=lspl[1],  
                                                    charge=float(lspl[2]),
                                                    atomradius=float(lspl[3]),
                                                    weight=float(lspl[4]),
                                                    cgname=lspl[5],
                                                    atomid=lspl[6],
                                                    beadradius=float(lspl[7]),
            )   
  except IndexError:
    pass


for i,j in grainMap.items():
    print i, j

