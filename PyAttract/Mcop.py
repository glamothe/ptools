from ptools import *
import copy
import math

#class MulticopyRegion:
    #" multicopy zone " 
    #RT=0.592 #at 298K
    #def __init__(self):
        #self._copies=[]

    #def AddCopy(self, copy):
        #self._copies.append(copy)
    #def Energy(self):
        #pass

    #def Translate(co):
        #for cop in self._copies:
            #cop.Translate(co)

    #def EnergyAndWeights(self, rigid, cutoff):
        #"calculates weights and energies between a multicopy region and a rigid region"
        #coeffs=[]
        #energies=[]
        #sumCoeffs=0.0
        #weights=[]
        #for copy in self._copies:
            #FF = AttractForceField(copy,rigid,cutoff)
            #energy=FF.Energy()
            #c = math.exp(-energy/self.RT)
            #coeffs.append(c)
            #sumCoeffs += c
            #energies.append(energy)
        ##normalize coeffs:
        #for i in xrange(len(coeffs)):
            #weights.append(coeffs/sumCoeffs)

        #return (energies, weigths)



def EnergyAndWeights(rigid,mcobject, ffobject, cutoff):
    "calculates weights and energies between a multicopy region and a rigid region"
    RT=0.592
    coeffs=[]
    energies=[]
    sumCoeffs=0.0
    weights=[]
    count=1
    for copy in mcobject:
        print count
        count+=1
        ffobject=AttractForceField2('mbest1k.par', rigid, copy, cutoff)
        energy=ffobject.nonbon8(rigid,copy,False)
        c = math.exp(-energy/RT)
        coeffs.append(c)
        sumCoeffs += c
        energies.append(energy)
    #normalize coeffs:
    for i in xrange(len(coeffs)):
        weights.append(coeffs[i]/sumCoeffs)

    return (energies, weights)





class MulticopyRigid:
    "Rigidbody that has multicopy regions"
    def __init__(self,fixed, copyRegions):
        assert(type(fixed) == AttractRigidbody)
        assert(type(copyRegions) == list )
        assert(len(copyRegions)>0)
        assert(type(copyRegions[0])==list)
        self._fixed=fixed
        self._mcopies=copy.copy(copyRegions)
    def Translate(self, T):
        self._fixed.Translate(T)
        for mcopregion in self._mcopies:
            for cop in mcopregion:
                cop.Translate(T)
    def AttractEuler(self, phi, ssi, rot):
        """Makes an euler rotation. 
        This routine also center the ligand so it rotates on 'itself'"""
        center=self._fixed.FindCenter()
        self.Translate(Coord3D()-center)
        AttractEuler(self._fixed, self._fixed, phi, ssi, rot)
        for mcopregion in self._mcopies:
            for cop in mcopregion:
                AttractEuler(cop, cop, phi, ssi, rot)
        self.Translate(center)  #translate the whole object back



class MulticopyForceField(ForceField):
    def __init__(self, mc_rec, mc_lig, cutoff):
        """initialization of forcefield
        mc_rec: an instance of MulticopyRigid(Python) fixed
        mc_lig: mobile one
        """
        ForceField.__init__(self)
        self._mcrec = mc_rec  # _mcrec is the multicopy receptor
        self._lig = lig   # _lig is not in multicopy for now
        self._cutoff = cutoff



    def Function(self, X):
        energy=0.0
        #copy of the proteins:
        #mcrec1=   Rigidbody(self._mcrec)  # no need to copy receptor if it does not move!

        #make a copy of the ligand:
        mclig1=AttractRigidbody(self._lig)

        #now set these proteins to the correct position

        AttractEuler(mclig1, mclig1, X[0],X[1],X[2] )  #Euler rotation. source == dest = mclig1 (copy of ligand, no multicopy)
        #mclig1.AttractEuler(X[0],X[1],X[2])
        mclig1.Translate(Coord3D(X[3], X[4], X[5]))

        #calculates the energy:
        #between the rigid objects:
        FF = AttractForceField2('mbest1k.par',self._mcrec._fixed, mclig1, self._cutoff)
        print FF
        energy+=FF.nonbon8(self._mcrec._fixed, mclig1, True)

        print energy
        #between receptor multicopy parts and ligand fixed part:
        for mcobject in self._mcrec._mcopies:
            ener, weights = EnergyAndWeights(mclig1 , mcobject, FF, self._cutoff)
            print "non pondere: ",sum(ener)
            listpond = map (lambda a, b: a*b,   ener, weights )
            print zip(ener, weights)
            print "avec les poids:", sum(listpond)
        
        #for mcregion in self.mcrec1._mcopies:
            #(en, wei) = mcregion.EnergyAndWeights(mcrec1._fixed, cutoff)


    def Derivatives(self, X, G):
        pass


if __name__ == '__main__':
    #creates a new multicopyrigid object:
    cop=[]
    for i in range(1,10):
        rigbdy=Rigidbody('tmp/copies/part/reduced/copie%i.part.pdb.red'%i)
        print rigbdy.Size()
        attrigbdy=AttractRigidbody(rigbdy)
        cop.append(attrigbdy)
    print len(cop), 'copies'
    mainProt = Rigidbody('tmp/copies/main.red')
    mainProt = AttractRigidbody(mainProt)
    print mainProt.Size()
    
    
    mcProt=MulticopyRigid(mainProt, [cop])
    lig = Rigidbody("tmp/1GOT_c_l.red")
    lig=AttractRigidbody(lig)
    assert(type(lig) == AttractRigidbody)
    print lig.Size()
    ff = MulticopyForceField(mcProt, lig,500)
    x=Vdouble()
    for i in range(10):
        x.append(0.0)
    ff.Function(x)
    