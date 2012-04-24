from cython.operator cimport dereference as deref
from libcpp.string cimport string


cdef extern from "<sstream>" namespace "std":
    cdef cppclass stringstream:
       stringstream()
       string str()
       void str(string &s)

       


cdef extern from "rigidbody.h" namespace "PTools":
    cdef cppclass CppRigidbody "PTools::Rigidbody":
        CppRigidbody(string)
        CppRigidbody()
        CppRigidbody(CppRigidbody&)
        unsigned int Size()
        CppCoord3D GetCoords(unsigned int)
        void unsafeGetCoords(unsigned int, CppCoord3D&)
        void ABrotate(CppCoord3D&, CppCoord3D&, double)
        void Translate(CppCoord3D&)
        CppCoord3D FindCenter()
        void syncCoords()
        void AttractEulerRotate(double, double, double)
        void ApplyMatrix(Array2D[double] &)
        CppAtom CopyAtom(unsigned int )

cdef extern from "pdbio.h" namespace "PTools":
    # in fact ReadPDB does not really take a stringstream as input argument, but it makes 
    # cython happy about typing and C++ is also happy due to inheritance
    cdef void ReadPDB(stringstream&, CppRigidbody&)



cdef class Rigidbody:
    cdef CppRigidbody* thisptr

    def __cinit__(self, filename=''):
        cdef CppRigidbody* oldrigidptr
        cdef Rigidbody  oldrigid
        if isinstance(filename, str):
            if filename ==  '':
                self.thisptr = new CppRigidbody()
            else:
                # there is a filename, loading the pdb file
                self.thisptr = _getRigidbody_from_py_name(filename)
        if isinstance(filename, Rigidbody):
            oldrigid = <Rigidbody> filename
            oldrigidptr = <CppRigidbody*> (oldrigid.thisptr)
            
            self.thisptr = new CppRigidbody(deref(oldrigidptr)  )
            if not self.thisptr:
                print "FATAL: this should never happen"

        if hasattr(filename, "read"):
            #we consider filename as a file-like object
            #print "reading rigidbody from file-like"
            self.thisptr = new CppRigidbody()
            loadPDBfromPythonFileLike(filename, self.thisptr)
           
    def __dealloc__(self):
        del self.thisptr
    def __len__(self):
        return self.thisptr.Size()
    def getCoords(self, unsigned int i):
        cdef Coord3D c = Coord3D () 
        cdef CppCoord3D cpp = self.thisptr.GetCoords(i)
        c.x = cpp.x
        c.y = cpp.y
        c.z = cpp.z
        return c
        
    def unsafeGetCoords(self, unsigned int i, Coord3D co):
        self.thisptr.unsafeGetCoords(i, deref(co.thisptr))
        
        
    def Translate(self, Coord3D tr):
        self.thisptr.Translate(deref(tr.thisptr))
    def FindCenter(self):
        cdef Coord3D c = Coord3D()
        cdef CppCoord3D cpp = self.thisptr.FindCenter()
        c.x = cpp.x
        c.y = cpp.y
        c.z = cpp.z
        return c
        
    def ABrotate(self, Coord3D A, Coord3D B, double theta):
        self.thisptr.ABrotate(deref(A.thisptr),deref(B.thisptr),theta)
        return None
        
    def AttractEulerRotate(self, double phi, double ssi, double rot):
        self.thisptr.AttractEulerRotate(phi, ssi, rot)
    
    def syncCoords(self):
        self.thisptr.syncCoords()

    def ApplyMatrix(self, Matrix mat):
        self.thisptr.ApplyMatrix(deref(mat.thisptr))

    def CopyAtom(self, unsigned int atid):
        cdef CppAtom cpp_at = self.thisptr.CopyAtom(atid)
        
        cdef Atom at = Atom()
        cdef CppAtom* cpp_dest = at.thisptr
        cy_copy_atom(&cpp_at , cpp_dest )
        return at
        
        


        
cdef CppRigidbody* _getRigidbody_from_py_name(pyname):
    cdef char* name = pyname
    cdef string *cppname = new string(name)
    cdef CppRigidbody *newrigid = new CppRigidbody(deref(cppname))
    return newrigid


cdef loadPDBfromPythonFileLike(file, CppRigidbody* rigid):
      cdef string cppstring
      lines = file.readlines()      

      for i in lines:
         cppstring += <char*> i
      
      cdef stringstream sstr
      sstr.str(cppstring)
      ReadPDB(sstr, deref(rigid))

      

def c_to_python_string():
    cdef char * test = "hello world"
    cdef bytes b = test
    return b
