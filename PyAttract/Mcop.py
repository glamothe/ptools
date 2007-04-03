from ptools import *



class MulticopyRegion:
    " multicopy zone " 
    RT=0.592 #at 298K
    def __init__(self):
        self._copies=[]

    def AddCopy(self, copy):
        self._copies.append(copy)
    def Energy(self):
        pass
        
    def EnergyAndWeights(self, rigid, cutoff):
        coeffs=[]
        energies=[]
        sumCoeffs=0.0
        weights=[]
        for copy in self._copies:
            FF = AttractForceField(copy,rigid,cutoff)
            energy=FF.Energy()
            c = math.exp(-energy/self.RT)
            coeffs.append(c)
            sumCoeffs += c
            energies.append(energy)
        #normalize coeffs:
        for i in xrange(len(coeffs)):
            weights.append(coeffs/sumCoeffs)
            
            
        return (energies, weigths)



class MulticopyRigid:
     "Rigidbody that has multicopy regions"
     def __init__(self,fixed):
         self._fixed=fixed
         self._mcopies=[] #array of "MulticopyRegion" objects
     def Translate(self, T):
         self._fixed.Translate(T)
         for mcop in self._mcopies:
             mcop.Translate(T)
     def AttractEuler(self, phi, ssi, rot):
         """Makes an euler rotation. 
         This routine also center the ligand so it rotates on 'itself'"""
         center=self._fixed.FindCenter()
         self.Translate(Coord3D()-center)
         AttractEuler(self._fixed, self._fixed, phi, ssi, rot)
         for mcop in self._mcopies:
             AttractEuler(self, self, phi, ssi, rot)
         self.Translate(center)
             


class MulticopyForceField(ForceField):
    def __init__(self, mc_rec, mc_lig, cutoff):
        """initialization of forcefield
           mc_rec: an instance of MulticopyRigid(Python) fixed
           mc_lig: mobile one
           """
        #ForceField.__init__(self)
        super(self)
        self._mcrec = mc
        self._mclig = rig
        self._cutoff = cutoff



    def Function(self, X):
        energy=0.0
        #copy of the proteins:
        mcrec1=Rigidbody(self._mcrec)
        mclig1=Rigidbody(self._mclig)
        
        #now set these proteins to the correct position
        mclig1.AttractEuler(X[0],X[1],X[2])
        mclig1.Translate(X[3], X[4], X[5])
        
        #calculates the energy:
        #between the rigid objects:
        FF = AttractForceField(mcrec1._fixed, mclig1._fixed, self._cutoff)
        energy+=FF.Energy()
        
        #between receptor's fixed part and ligand's multicopy parts:
        for mcregion in mclig1._mcopies:
            (en, wei) = mcregion.EnergyAndWeights(mcrec1._fixed, cutoff)
        

    def Derivatives(self, X, G):
        pass


