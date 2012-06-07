
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



for p in parameters:
  lspl = p.split() 
  try:  
    if lspl[0] != '#':
        listofgrains = residNames.get(lspl[0], {})
        listofgrains[lspl[5]] = 1    #add coarse grain name    
        residNames[lspl[0]] = listofgrains

        
        listOfAtomsInAGrain = grainMap.get("%s:%s"%(lspl[0], lspl[5]), [] ) 

        listOfAtomsInAGrain.append(dict(  atomname=lspl[1],  
                                          atomcharge=float(lspl[2]),
                                          atomradius=float(lspl[3]),
                                          weight=float(lspl[4]),
                                          beadname=lspl[5],
                                          beadid=lspl[6],
                                          beadradius=float(lspl[7]),
                                        ) 
                                   )
        grainMap["%s:%s"%(lspl[0], lspl[5]) ] = listOfAtomsInAGrain

 
  except IndexError:
    pass


for i,j in grainMap.items():
    print i, j


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
