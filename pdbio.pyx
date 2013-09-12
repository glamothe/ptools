from cython.operator cimport dereference as deref
from libcpp.string cimport string


cdef extern from "pdbio.h" namespace "PTools":
    cdef void CppWritePDB "PTools::WritePDB" (CppRigidbody&, string)


def WritePDB(Rigidbody rig, bytes filename):
    cdef char* c_filename = <char *> filename
    cdef string* cpp_filename = new string(c_filename)
    cdef CppRigidbody* rigptr = rig.thisptr
    CppWritePDB(deref(rigptr),  deref(cpp_filename) )
    del cpp_filename
