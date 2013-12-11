from cython.operator cimport dereference as deref
from libcpp.string cimport string


cdef extern from "pdbio.h" namespace "PTools":
    cdef void CppWritePDB "PTools::WritePDB" (CppRigidbody&, string)
    cdef void CppReadAtom "PTools::readAtom" (string, CppAtom) except+




def WritePDB(Rigidbody rig, bytes filename):
    cdef char* c_filename = <char *> filename
    cdef string* cpp_filename = new string(c_filename)
    cdef CppRigidbody* rigptr = rig.thisptr
    CppWritePDB(deref(rigptr),  deref(cpp_filename) )
    del cpp_filename


def strToAtom(bytes line):
    """converts a PDB-formatted Python string into an Atom object"""
    cdef char* c_str = <char *> line
    cdef string* cpp_str = new string(c_str)
    cdef CppAtom* c_at =  new CppAtom()

    CppReadAtom(deref(cpp_str), deref(c_at))
    
    at = Atom()
    del at.thisptr
    at.thisptr = c_at

    del cpp_str

    return at

    