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
        CppAtom(Atomproperty, CppCoord3D)
        Coord3D GetCoords()
        void SetCoords(CppCoord3D& )
        string ToString()
        string ToPdbString()
        void Translate(CppCoord3D&)
        double Dist(CppAtom& , CppAtom& )
        double Dist2(CppAtom& , CppAtom& )

        
cdef class AtomProperty:
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
        return <bytes>  self.thisptr.GetResidId().c_str()