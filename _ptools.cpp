
// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <atom.h>
#include <atomselection.h>
#include <coord3d.h>
#include <forcefield.h>
#include <geometry.h>
#include <pdbio.h>
#include <rigidbody.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace  {

struct PTools_AttractForceField_Wrapper: PTools::AttractForceField
{
    PTools_AttractForceField_Wrapper(PyObject* py_self_, const PTools::AttractForceField& p0):
        PTools::AttractForceField(p0), py_self(py_self_) {}

    PTools_AttractForceField_Wrapper(PyObject* py_self_, const PTools::Rigidbody& p0, const PTools::Rigidbody& p1):
        PTools::AttractForceField(p0, p1), py_self(py_self_) {}

    double Function() {
        return call_method< double >(py_self, "Function");
    }

    double default_Function() {
        return PTools::AttractForceField::Function();
    }

    double Function(const double* p0) {
        return call_method< double >(py_self, "Function", p0);
    }

    double default_Function(const double* p0) {
        return PTools::AttractForceField::Function(p0);
    }

    void Derivatives(const double* p0, double* p1) {
        call_method< void >(py_self, "Derivatives", p0, p1);
    }

    void default_Derivatives(const double* p0, double* p1) {
        PTools::AttractForceField::Derivatives(p0, p1);
    }

    void NumDerivatives(const double* p0, double* p1) {
        call_method< void >(py_self, "NumDerivatives", p0, p1);
    }

    void default_NumDerivatives(const double* p0, double* p1) {
        PTools::AttractForceField::NumDerivatives(p0, p1);
    }

    PyObject* py_self;
};


}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(_ptools)
{
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

    class_< PTools::AttractForceField, PTools_AttractForceField_Wrapper >("AttractForceField", init< const PTools::AttractForceField& >())
        .def(init< const PTools::Rigidbody&, const PTools::Rigidbody& >())
        .def("Function", (double (PTools::AttractForceField::*)() )&PTools::AttractForceField::Function, (double (PTools_AttractForceField_Wrapper::*)())&PTools_AttractForceField_Wrapper::default_Function)
        .def("Function", (double (PTools::AttractForceField::*)(const double*) )&PTools::AttractForceField::Function, (double (PTools_AttractForceField_Wrapper::*)(const double*))&PTools_AttractForceField_Wrapper::default_Function)
        .def("Derivatives", (void (PTools::AttractForceField::*)(const double*, double*) )&PTools::AttractForceField::Derivatives, (void (PTools_AttractForceField_Wrapper::*)(const double*, double*))&PTools_AttractForceField_Wrapper::default_Derivatives)
        .def("NumDerivatives", (void (PTools::AttractForceField::*)(const double*, double*) )&PTools::AttractForceField::NumDerivatives, (void (PTools_AttractForceField_Wrapper::*)(const double*, double*))&PTools_AttractForceField_Wrapper::default_NumDerivatives)
        .def("Energy", (double (PTools::AttractForceField::*)() )&PTools::AttractForceField::Energy)
        .def("Energy", (double (PTools::AttractForceField::*)(const double*) )&PTools::AttractForceField::Energy)
        .def("Electrostatic", &PTools::AttractForceField::Electrostatic)
        .def("LennardJones", &PTools::AttractForceField::LennardJones)
        .def("Gradient", &PTools::AttractForceField::Gradient)
        .def("ShowEnergyParams", &PTools::AttractForceField::ShowEnergyParams)
        .def("Trans", (void (PTools::AttractForceField::*)() )&PTools::AttractForceField::Trans)
    ;

}

