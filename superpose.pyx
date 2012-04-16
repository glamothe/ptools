cdef extern from "superpose.h" namespace "PTools":
    cdef cppSuperpose_t cppsuperpose "PTools::superpose" ( CppRigidbody& ,CppRigidbody& , int )
    

    
def superpose(Rigidbody ref, Rigidbody mob, int verbosity=0):
    cdef cppSuperpose_t sup = cppsuperpose(deref(ref.thisptr), deref(mob.thisptr), verbosity)
    
    mat = Matrix()

    pysup = Superpose_t(sup.rmsd, mat)

    return pysup
