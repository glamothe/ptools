from cython.operator cimport dereference as deref
from libcpp.string cimport string


cdef extern from "atomselection.h" namespace "PTools":
    cdef cppclass CppAtomSelection "PTools::AtomSelection":
        CppAtomSelection()
        CppAtomSelection(CppAtomSelection&)
        CppAtomSelection(CppRigidbody)

        unsigned int Size()
        void SetRigid(CppRigidbody&)
        CppAtom operator[]
        CppAtom CopyAtom(unsigned int)
        void AddAtomIndex(unsigned int)
        CppRigidbody CreateRigid()

        CppAtomSelection non(CppAtomSelection &)

    cdef AtomSelection operator& (CppAtomSelection&, CppAtomSelection&)
    cdef AtomSelection operator| (CppAtomSelection&, CppAtomSelection&)
#    cdef AtomSelection operator not (CppAtomSelection&, CppAtomSelection&) 

cdef class AtomSelection:

    cdef CppAtomSelection * thisptr

    def __cinit__(self, arg=None):
        cdef AtomSelection atsel
        cdef CppAtomSelection* atselptr
        cdef Rigidbody rig
        cdef CppRigidbody* rigptr

        self.thisptr = <CppAtomSelection*> 0
        if arg is None:
            self.thisptr = new CppAtomSelection()
            return

        if isinstance(AtomSelection, arg):
            atsel  = <AtomSelection> arg
            atselptr  = <CppAtomSelection*> atsel.thisptr
            self.thisptr = new CppAtomSelection(deref(atselptr))
            return

        if isinstance(Rigidbody, arg):
            rig = <Rigidbody> arg
            rigptr = rig.thisptr
            self.thisptr = new CppAtomSelection(deref(rigptr))

        raise RuntimeError("cannot reach here")

    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr

    
