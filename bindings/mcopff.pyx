from cython.operator cimport dereference as deref
from libcpp.string cimport string

cdef extern from "<sstream>" namespace "std":
    cdef cppclass stringstream:
       stringstream()
       string str()
       void str(string &s)
       
cdef extern from "mcopff.h" namespace "PTools":
    cdef cppclass CppMcop "PTools::Mcop":
        CppMcop()
        CppMcop(string&) except+
        CppMcop(CppMcop&)
        CppRigidbody getCopy(unsigned int)
        CppRigidbody operator[](unsigned int)
        unsigned int size()
        void addCopy(CppRigidbody&)
    cdef cppclass CppAttractMcop "PTools::AttractMcop":
        CppAttractMcop()
        CppAttractMcop(string&) except+
        CppAttractMcop(CppMcop&)
        CppAttractMcop(CppAttractMcop&)
        CppAttractRigidbody getCopy(unsigned int)
        CppAttractRigidbody operator[](unsigned int)
        unsigned int size()
        void addCopy(CppAttractRigidbody&)
    cdef cppclass CppMcoprigid "PTools::Mcoprigid":
        CppMcoprigid()
        CppMcoprigid(string&) except+
        #CppMcoprigid(CppRigidbody&, vector(CppAttractMcop))
        CppMcoprigid(CppMcoprigid&)
        #CppAttractMcop getRegion(unsigned int)
        #CppAttractRigidbody getCore()


cdef class Mcop:
    cdef CppMcop* thisptr
    
    def __cinit__(self, filename=''):
    
        cdef CppMcop* oldptr
        cdef Mcop oldmcop
        cdef char* name
        cdef string *cppname
        cdef CppMcop* newmcop
        
        if isinstance(filename, str):
            if filename ==  '':
                self.thisptr = new CppMcop()
            else:
                # there is a filename, loading the pdb file
                name = filename
                cppname = new string(name)
                newmcop = new CppMcop(deref(cppname))
                del cppname
                self.thisptr = newmcop
        
        elif isinstance(filename, Mcop):
            oldmcop = <Mcop> filename
            oldptr = <CppMcop*> (oldmcop.thisptr)
            self.thisptr = new CppMcop(deref(oldptr))
            if not self.thisptr:
                print "FATAL: this should never happen"
        
        else:
            raise RuntimeError("invalid argument in Mcop()")

    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr
            self.thisptr = <CppMcop*> 0
    
    def getCopy(self, unsigned int i):
        cdef CppRigidbody cpp_copy = self.thisptr.getCopy(i)
        cdef Rigidbody cop = Rigidbody()
        cdef CppRigidbody* new_cop = new CppRigidbody(cpp_copy)
        del cop.thisptr
        cop.thisptr = new_cop
        return cop
    
    def __getitem__(self,unsigned int i):
        cdef CppRigidbody cpp_copy = self.thisptr.getCopy(i)
        cdef Rigidbody cop = Rigidbody()
        cdef CppRigidbody* new_cop = new CppRigidbody(cpp_copy)
        del cop.thisptr
        cop.thisptr = new_cop
        return cop
    
    def __len__(self):
        return self.thisptr.size()
        
    def addCopy(self, Rigidbody rigid):
        self.thisptr.addCopy(deref(<CppRigidbody*>rigid.thisptr))

       
cdef class AttractMcop (Mcop):
    cdef CppAttractMcop* thisptr2
    
    def __cinit__(self, filename=''):
        
        # first deallocate the previously allocated Mcop
        del self.thisptr
        self.thisptr = new CppMcop()
        
        cdef CppAttractMcop* oldptr
        cdef CppMcop* oldmcop_ptr
        cdef AttractMcop oldattractmcop
        cdef Mcop oldmcop
        cdef char* name
        cdef string *cppname
        cdef CppAttractMcop* newattractmcop

        if isinstance(filename, str):
            if filename == '':
                self.thisptr2 = new CppAttractMcop()
            else:
                # there is a filename, laoding the pdb file
                
                name = filename
                cppname = new string(name)
                newattractmcop = new CppAttractMcop(deref(cppname))
                del cppname
                self.thisptr2 = newattractmcop
       
        elif isinstance(filename, AttractMcop):
            oldattractmcop = <AttractMcop> filename
            oldptr = <CppAttractMcop*> (oldattractmcop.thisptr2)
            self.thisptr2 = new CppAttractMcop(deref(oldptr))
            if not self.thisptr2:
                print "FATAL: this should never happen"

        elif isinstance(filename, Mcop):
            
            oldmcop = <AttractMcop> filename
            oldmcop_ptr = <CppMcop*> (oldmcop.thisptr)
            self.thisptr2 = new CppAttractMcop(deref(oldmcop_ptr))
            if not self.thisptr2:
                print "FATAL; this should never happen"

        else:
            raise RuntimeError("invalid argument in AttractMcop()")
    
    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr
            self.thisptr = <CppMcop*> 0
        if self.thisptr2:
            del self.thisptr2
            self.thisptr2 = <CppAttractMcop*> 0
    
    def getCopy(self, unsigned int i):
        cdef CppAttractRigidbody cpp_copy = self.thisptr2.getCopy(i)
        cdef AttractRigidbody cop = AttractRigidbody()
        cdef CppAttractRigidbody* new_cop = new CppAttractRigidbody(cpp_copy)
        del cop.thisptr
        cop.thisptr = <CppRigidbody*> new_cop
        return cop
        
    def __getitem__(self,unsigned int i):
        cdef CppAttractRigidbody cpp_copy = self.thisptr2.getCopy(i)
        cdef AttractRigidbody cop = AttractRigidbody()
        cdef CppAttractRigidbody* new_cop = new CppAttractRigidbody(cpp_copy)
        del cop.thisptr
        cop.thisptr = <CppRigidbody*> new_cop
        return cop
        
    def addCopy(self, AttractRigidbody attrigid):
        self.thisptr2.addCopy(deref(<CppAttractRigidbody*>attrigid.thisptr))
     
    def __len__(self):
        return self.thisptr2.size()
   
  
cdef class Mcoprigid:
    cdef CppMcoprigid* thisptr
    
    def __cinit__(self, filename='', arg2=''):
    
        cdef CppMcoprigid* oldptr
        cdef Mcoprigid oldmcoprigid
        cdef char* name
        cdef string *cppname
        cdef CppMcoprigid* newmcoprigid
        
        if isinstance(filename, str):
            if filename == '':
                self.thisptr = new CppMcoprigid()
            else:
                # there is a filename, loading the pdb file
                name = filename
                cppname = new string(name)
                newmcoprigid = new CppMcoprigid(deref(cppname))
                del cppname
                self.thisptr = newmcoprigid
            
        elif isinstance(filename, Mcoprigid):
            oldmcoprigid = <Mcoprigid> filename
            oldptr = <CppMcoprigid*> oldmcoprigid.thisptr
            self.thisptr = new CppMcoprigid(deref(oldptr))
            if not self.thisptr:
                print "FATAL: this should never happen"
        
        #if filename is AttractRigibody and arg2 is list of AttractMcop
        elif isinstance(arg2, AttractRigidbody) and isinstance(filename, (list, tuple)) and all(isinstance(elem, AttractMcop) for elem in arg2):
            print 'test'
            # TODO
        
        else:
            raise RunetimeError("invalid argument in Mcoprigid()")
            
    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr
            self.thisptr = <CppMcoprigid*> 0
    
    def getCore()
        cdef CppAttractRigidbody cpp_core = self.thisptr.getCore(i)
        cdef AttractRigidbody core = AttractRigidbody()
        cdef CppAttractRigbody* new_core = new CppAttractRigidbody(cpp_core)
        del core.thisptr
        core.thisptr = new_core
        return core    
        
    def getRegion(i)
        cdef CppAttractMcop cpp_region = self.thisptr.getRegion(i)
        cdef AttractMcop region = AttractMcop()
        cdef CppAttractMcop* new_region = new CppAttractMcop(cpp_region)
        del region.thisptr
        region.thisptr = new_region
        return region
        
        
