cdef extern from "atom.h" namespace "PTools":
    cdef cppclass CppAtomproperty "PTools::Atomproperty":
        string GetType()
        void SetType(string )
        string GetResidType() 
        void SetResidType(string)
        double GetAtomCharge() 
        void SetAtomCharge(double)
        string GetChainId()
        void SetChainId(string chainid)
        unsigned int GetResidId() 
        void SetResidId(unsigned int)
        unsigned int GetAtomId()
        void SetAtomId(unsigned int)
        void SetExtra(string extra)
        string GetExtra()
        
    cdef cppclass CppAtom "PTools::Atom":
        CppAtom()
        CppAtom(CppAtomproperty, CppCoord3D)
        CppCoord3D GetCoords()
        void SetCoords(CppCoord3D& )
        string ToString()
        string ToPdbString()
        void Translate(CppCoord3D&)
        
    cdef double cppDist "PTools::Dist" (CppAtom& , CppAtom& )
    cdef double cppDist2 "PTools::Dist2" (CppAtom& , CppAtom& )

cdef extern from "cython_wrappers.h":
    cdef void cy_copy_atom(CppAtom* , CppAtom* )
    
        
cdef class Atomproperty:
    cdef CppAtomproperty * thisptr
    
    def __cinit__(self):
        self.thisptr = new CppAtomproperty()
        
    def __dealloc__(self):
        del self.thisptr
    
    
    def GetType(self):
        return <bytes> self.thisptr.GetType().c_str()
        
    def SetType(self, bytes b):
        cdef char* c = b
        cdef string cppname = string(c)
        self.thisptr.SetType(cppname)
        
    def GetResidType(self):
        return <bytes> self.thisptr.GetResidType().c_str()
    
    def SetResidType(self, bytes residtype):
        cdef char * c_residtype = residtype
        self.thisptr.SetResidType(string(c_residtype))
    
    def GetAtomCharge(self):
        return self.thisptr.GetAtomCharge()
    
    def SetAtomCharge(self, double charge):
        self.thisptr.SetAtomCharge(charge)
    
    def GetChainId(self):
        return <bytes> self.thisptr.GetChainId().c_str()
    
    def SetChainId(self, bytes chainid):
        cdef char * c_chainid = chainid
        self.thisptr.SetChainId(string(c_chainid))
        
    def GetResidId(self):
        return self.thisptr.GetResidId()
        
    def SetResidId(self, unsigned int resid):
        self.thisptr.SetResidId(resid)
        
    def GetAtomId(self):
        return self.thisptr.GetAtomId()
    def SetAtomId(self, unsigned int atomid):
        self.thisptr.SetAtomId(atomid)
    def SetExtra(self, bytes extra):
        cdef char* c_extra = extra
        self.thisptr.SetExtra(string(c_extra))
    def GetExtra(self):
        return self.thisptr.GetExtra().c_str()

        
cdef class Atom:
    cdef CppAtom * thisptr
    #def __cinit__(self, Atomproperty atproperty, Coord3D coords):
    def __cinit__(self, *args ):
        cdef CppAtomproperty * cppatpropptr
        cdef CppCoord3D * cpp_coptr
        
        if len(args) == 0:
            self.thisptr = new CppAtom()
        else:
           atproperty = <Atomproperty?> args[0]
           coords = <Coord3D?> args[1]
           cppatpropptr = atproperty.thisptr
           cpp_coptr = coords.thisptr
           self.thisptr = new CppAtom(deref(cppatpropptr), deref(cpp_coptr))
        
    def __dealloc__(self):
        
        del self.thisptr
     
    def GetCoords(self):
        co = Coord3D()
        cdef CppCoord3D c_coord3D = self.thisptr.GetCoords()
        co.x = c_coord3D.x
        co.y = c_coord3D.y
        co.z = c_coord3D.z
        
        return co
        
    def SetCoords(self, Coord3D co):
        self.thisptr.SetCoords(deref(co.thisptr))
        
    def ToString(self):
        return <bytes> self.thisptr.ToString().c_str()
    def __str__(self):
        return self.ToString()
        
    def ToPdbString(self):
        return <bytes> self.thisptr.ToPdbString().c_str()
    
    def Translate(self,Coord3D co):
        self.thisptr.Translate(deref(co.thisptr))
    
def Dist(Atom at1, Atom at2):
    return cppDist(deref(at1.thisptr), deref(at2.thisptr))
    
def Dist2(Atom at1, Atom at2):
    return cppDist2(deref(at1.thisptr), deref(at2.thisptr))
        
