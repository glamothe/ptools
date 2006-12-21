
// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <atom.h>
#include <coord3d.h>
#include <pdbparser.h>
#include <rigidbody.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
BOOST_PYTHON_MODULE(ptools)
{
    class_< PTools::Coord3D >("Coord3D", init<  >())
        .def(init< const PTools::Coord3D& >())
        .def_readwrite("x", &PTools::Coord3D::x)
        .def_readwrite("y", &PTools::Coord3D::y)
        .def_readwrite("z", &PTools::Coord3D::z)
        .def( self * other< double >() )
        .def( other< double >() * self )
        .def( self + self )
        .def( self - self )
    ;

    class_< PTools::Atom >("Atom", init< const PTools::Atom& >())
        .def(init< PTools::Atomproperty, PTools::Coord3D >())
        .def("GetCoords", &PTools::Atom::GetCoords)
        .def("SetCoords", &PTools::Atom::SetCoords)
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

    class_< PTools::Rigidbody >("Rigidbody", init<  >())
        .def(init< const PTools::Rigidbody& >())
        .def("CopyAtom", &PTools::Rigidbody::CopyAtom)
        .def("size", &PTools::Rigidbody::size)
        .def("AddAtom", (void (PTools::Rigidbody::*)(const PTools::Atomproperty&, PTools::Coord3D) )&PTools::Rigidbody::AddAtom)
        .def("AddAtom", (void (PTools::Rigidbody::*)(const PTools::Atom&) )&PTools::Rigidbody::AddAtom)
        .def("GetCoords", &PTools::Rigidbody::GetCoords)
        .def("SetCoords", &PTools::Rigidbody::SetCoords)
    ;

    def("readPDB", (void (*)(std::ifstream&, PTools::Rigidbody&))&PTools::readPDB);
    def("readPDB", (void (*)(const std::string, PTools::Rigidbody&))&PTools::readPDB);
}

