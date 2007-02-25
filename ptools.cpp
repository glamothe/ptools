
// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <atom.h>
#include <atomselection.h>
#include <coord3d.h>
#include <geometry.h>
#include <pdbio.h>
#include <rigidbody.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
BOOST_PYTHON_MODULE(ptools)
{
    class_< PTools::Coord3D >("Coord3D", init<  >())
        .def(init< const PTools::Coord3D& >())
        .def(init< double, double, double >())
        .def_readwrite("x", &PTools::Coord3D::x)
        .def_readwrite("y", &PTools::Coord3D::y)
        .def_readwrite("z", &PTools::Coord3D::z)
        .def("Normalize", &PTools::Coord3D::Normalize, return_internal_reference< 1 >())
        .def( other< double >() * self )
        .def( self - self )
        .def( self + self )
        .def( self * other< double >() )
    ;

    def("Norm", &PTools::Norm);
    def("Norm2", &PTools::Norm2);
    class_< PTools::Atom >("Atom", init< const PTools::Atom& >())
        .def(init< PTools::Atomproperty, PTools::Coord3D >())
        .def("GetCoords", &PTools::Atom::GetCoords)
        .def("SetCoords", &PTools::Atom::SetCoords)
        .def("ToString", &PTools::Atom::ToString)
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
    ;

    def("Dist", &PTools::Dist);
    def("Dist2", &PTools::Dist2);
    class_< PTools::Rigidbody >("Rigidbody", init<  >())
        .def(init< const PTools::Rigidbody& >())
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

}

