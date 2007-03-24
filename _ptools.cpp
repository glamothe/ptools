
// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <atom.h>
#include <atomselection.h>
#include <coord3d.h>
#include <forcefield.h>
#include <geometry.h>
#include <minimizers/lbfgs_interface.h>
#include <pdbio.h>
#include <rigidbody.h>
#include <rmsd.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
 
#include <boost/python/suite/indexing/indexing_suite.hpp> 
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/wrapper.hpp>
#include <boost/python/call.hpp>




namespace  {

struct PTools_ForceField_Wrapper: PTools::ForceField, wrapper<PTools::ForceField>
{

    double Function(const Vdouble& p0) {
        return this->get_override("Function")(boost::ref(p0));
    }

    void Derivatives(const Vdouble& p0, Vdouble& p1) {
        this->get_override("Derivatives")(boost::ref(p0), boost::ref(p1));
        return;
    }


    uint ProblemSize() {
        return this->get_override("ProblemSize")();
    }

};

struct PTools_AttractForceField_Wrapper: PTools::AttractForceField
{
    PTools_AttractForceField_Wrapper(PyObject* py_self_, const PTools::AttractForceField& p0):
        PTools::AttractForceField(p0), py_self(py_self_) {}

    PTools_AttractForceField_Wrapper(PyObject* py_self_, const PTools::Rigidbody& p0, const PTools::Rigidbody& p1, double p2):
        PTools::AttractForceField(p0, p1, p2), py_self(py_self_) {}

    double Function(const Vdouble& p0) {
        return call_method< double >(py_self, "Function", p0);
    }

    double default_Function(const Vdouble& p0) {
        return PTools::AttractForceField::Function(p0);
    }

    void Derivatives(const Vdouble& p0, Vdouble& p1) {
        call_method< void >(py_self, "Derivatives", boost::ref(p0), boost::ref(p1));
    }

    void default_Derivatives(const Vdouble& p0, Vdouble& p1) {
        PTools::AttractForceField::Derivatives(boost::ref(p0), boost::ref(p1));
    }

    void NumDerivatives(const Vdouble& p0, Vdouble& p1) {
        call_method< void >(py_self, "NumDerivatives", boost::ref(p0), boost::ref(p1));
    }

    void default_NumDerivatives(const Vdouble& p0, Vdouble& p1) {
        PTools::AttractForceField::NumDerivatives(boost::ref(p0), boost::ref(p1));
    }

    uint ProblemSize() {
        return call_method< uint >(py_self, "ProblemSize");
    }

    uint default_ProblemSize() {
        return PTools::AttractForceField::ProblemSize();
    }

    PyObject* py_self;
};


struct PTools_TestForceField_Wrapper: PTools::TestForceField, wrapper<PTools::TestForceField>
{
    double Function(const Vdouble& vec)
    {
        return this->get_override("Function")(boost::ref(vec));
    }

    void Derivatives(const Vdouble& v1, Vdouble& v2)
    {
        this->get_override("Derivatives")(boost::ref(v1),boost::ref(v2));
    }

    uint ProblemSize()
    {
        return this->get_override("ProbleSize")();
    }
};


}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(_ptools)
{
 class_< Vdouble >("std_vector_double") 
        .def(vector_indexing_suite<std::vector<double> >());
    class_< PTools::Coord3D >("Coord3D", init<  >())
        .def(init< const PTools::Coord3D& >())
        .def(init< double, double, double >())
        .def_readwrite("x", &PTools::Coord3D::x)
        .def_readwrite("y", &PTools::Coord3D::y)
        .def_readwrite("z", &PTools::Coord3D::z)
        .def("Normalize", &PTools::Coord3D::Normalize, return_internal_reference< 1 >())
        .def( self + self )
        .def( self - self )
        .def( other< double >() * self )
        .def( self * other< double >() )
    ;

    def("Norm", &PTools::Norm);
    def("Norm2", &PTools::Norm2);
    def("PrintCoord", &PTools::PrintCoord);
    class_< PTools::Atom >("Atom", init< const PTools::Atom& >())
        .def(init< PTools::Atomproperty, PTools::Coord3D >())
        .def("GetCoords", &PTools::Atom::GetCoords)
        .def("SetCoords", &PTools::Atom::SetCoords)
        .def("ToString", &PTools::Atom::ToString)
        .def("ToPdbString", &PTools::Atom::ToPdbString)
        .def("Translate", &PTools::Atom::Translate)
        .def("GetType", &PTools::Atomproperty::GetType)
        .def("SetType", &PTools::Atomproperty::SetType)
        .def("GetResidType", &PTools::Atomproperty::GetResidType)
        .def("SetResidType", &PTools::Atomproperty::SetResidType)
        .def("GetAtomCharge", &PTools::Atomproperty::GetAtomCharge)
        .def("SetAtomCharge", &PTools::Atomproperty::SetAtomCharge)
        .def("GetChainId", &PTools::Atomproperty::GetChainId)
        .def("SetChainId", &PTools::Atomproperty::SetChainId)
        .def("GetResidId", &PTools::Atomproperty::GetResidId)
        .def("SetResidId", &PTools::Atomproperty::SetResidId)
        .def("GetAtomId", &PTools::Atomproperty::GetAtomId)
        .def("SetAtomId", &PTools::Atomproperty::SetAtomId)
        .def("SetExtra", &PTools::Atomproperty::SetExtra)
        .def("GetExtra", &PTools::Atomproperty::GetExtra)
    ;

    def("Dist", &PTools::Dist);
    def("Dist2", &PTools::Dist2);
    class_< PTools::Rigidbody >("Rigidbody", init<  >())
        .def(init< const PTools::Rigidbody& >())
        .def(init< std::string >())
        .def("CopyAtom", &PTools::Rigidbody::CopyAtom)
        .def("GetAtom", &PTools::Rigidbody::GetAtom, return_internal_reference< 1 >())
        .def("Size", &PTools::Rigidbody::Size)
        .def("AddAtom", (void (PTools::Rigidbody::*)(const PTools::Atomproperty&, PTools::Coord3D) )&PTools::Rigidbody::AddAtom)
        .def("AddAtom", (void (PTools::Rigidbody::*)(const PTools::Atom&) )&PTools::Rigidbody::AddAtom)
        .def("GetCoords", &PTools::Rigidbody::GetCoords)
        .def("SetCoords", &PTools::Rigidbody::SetCoords)
        .def("FindCenter", &PTools::Rigidbody::FindCenter)
        .def("Translate", &PTools::Rigidbody::Translate)
        .def("SelectAllAtoms", &PTools::Rigidbody::SelectAllAtoms)
        .def("SelectAtomType", &PTools::Rigidbody::SelectAtomType)
        .def("SelectResidType", &PTools::Rigidbody::SelectResidType)
        .def("SelectChainId", &PTools::Rigidbody::SelectChainId)
        .def("SelectResRange", &PTools::Rigidbody::SelectResRange)
        .def("CA", &PTools::Rigidbody::CA)
        .def("ABrotate", &PTools::Rigidbody::ABrotate)
        .def("ResetMatrix", &PTools::Rigidbody::ResetMatrix)
        .def("PrintMatrix", &PTools::Rigidbody::PrintMatrix)
        .def("ApplyMatrix", &PTools::Rigidbody::ApplyMatrix)
        .def("GetMatrix", &PTools::Rigidbody::GetMatrix)
        .def( self + self )
    ;

    def("ReadPDB", (void (*)(const std::string, PTools::Rigidbody&))&PTools::ReadPDB);
    def("ReadPDB", (void (*)(std::ifstream&, PTools::Rigidbody&))&PTools::ReadPDB);
    def("WritePDB", &PTools::WritePDB);
    def("ABrotate", &PTools::ABrotate);
    def("Translate", &PTools::Translate);
    class_< PTools::AtomSelection >("AtomSelection", init<  >())
        .def(init< const PTools::AtomSelection& >())
        .def(init< PTools::Rigidbody& >())
        .def("Size", &PTools::AtomSelection::Size)
        .def("SetRigid", &PTools::AtomSelection::SetRigid)
        .def("CopyAtom", &PTools::AtomSelection::CopyAtom)
        .def("AddAtomIndex", &PTools::AtomSelection::AddAtomIndex)
        .def("CreateRigid", &PTools::AtomSelection::CreateRigid)
        .def( self & self )
        .def( self | self )
    ;

    class_<PTools_ForceField_Wrapper , boost::noncopyable >("ForceField", init<  >())
        .def("Function", pure_virtual(&PTools::ForceField::Function))
        .def("Derivatives", pure_virtual(&PTools::ForceField::Derivatives))
        //.def("NumDerivatives", &PTools::ForceField::NumDerivatives, &PTools_ForceField_Wrapper::default_NumDerivatives)
        .def("ProblemSize", pure_virtual(&PTools::ForceField::ProblemSize))
    ;

    class_< PTools::AttractForceField, bases< PTools::ForceField > , PTools_AttractForceField_Wrapper >("AttractForceField", init< const PTools::AttractForceField& >())
        .def(init< const PTools::Rigidbody&, const PTools::Rigidbody&, double >())
        .def("Function", (double (PTools::AttractForceField::*)(const Vdouble&) )&PTools::AttractForceField::Function, (double (PTools_AttractForceField_Wrapper::*)(const Vdouble&))&PTools_AttractForceField_Wrapper::default_Function)
        .def("Derivatives", (void (PTools::AttractForceField::*)(const Vdouble&, Vdouble&) )&PTools::AttractForceField::Derivatives, (void (PTools_AttractForceField_Wrapper::*)(const Vdouble&, Vdouble&))&PTools_AttractForceField_Wrapper::default_Derivatives)
        .def("NumDerivatives", (void (PTools::AttractForceField::*)(const Vdouble&, Vdouble&) )&PTools::AttractForceField::NumDerivatives, (void (PTools_AttractForceField_Wrapper::*)(const Vdouble&, Vdouble&))&PTools_AttractForceField_Wrapper::default_NumDerivatives)
        .def("ProblemSize", (uint (PTools::AttractForceField::*)() )&PTools::AttractForceField::ProblemSize, (uint (PTools_AttractForceField_Wrapper::*)())&PTools_AttractForceField_Wrapper::default_ProblemSize)
        .def("Energy", (double (PTools::AttractForceField::*)() )&PTools::AttractForceField::Energy)
        .def("Energy", (double (PTools::AttractForceField::*)(const Vdouble&) )&PTools::AttractForceField::Energy)
        .def("Electrostatic", &PTools::AttractForceField::Electrostatic)
        .def("LennardJones", &PTools::AttractForceField::LennardJones)
        .def("Gradient", &PTools::AttractForceField::Gradient)
        .def("ShowEnergyParams", &PTools::AttractForceField::ShowEnergyParams)
        .def("Trans", (void (PTools::AttractForceField::*)() )&PTools::AttractForceField::Trans)
    ;

    class_< PTools::TestForceField, bases< PTools::ForceField >  >("TestForceField", init<  >())
        .def(init< const PTools::TestForceField& >())
        .def("Function", &PTools::ForceField::Function)
        .def("Derivatives", &PTools::ForceField::Derivatives)
        .def("ProblemSize", &PTools::ForceField::ProblemSize)
    ;

    class_< PTools::Lbfgs >("Lbfgs", init< const PTools::Lbfgs& >())
        .def(init< PTools::ForceField& >())
        .def("minimize", &PTools::Lbfgs::minimize)
        .def("GetX", &PTools::Lbfgs::GetX)
    ;


    def("AttractEuler", &PTools::AttractEuler);
    def("Rmsd", &PTools::Rmsd);


}

