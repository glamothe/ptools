
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
        .def("AddAtom", &PTools::Rigidbody::AddAtom)
        .def("GetCoord", &PTools::Rigidbody::GetCoord)
    ;

    def("readPDB", (void (*)(std::ifstream&, PTools::Rigidbody&))&PTools::readPDB);
    def("readPDB", (void (*)(const std::string, PTools::Rigidbody&))&PTools::readPDB);
}

