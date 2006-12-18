
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
    class_< PTools::coord3D >("coord3D", init<  >())
        .def(init< const PTools::coord3D& >())
        .def_readwrite("x", &PTools::coord3D::x)
        .def_readwrite("y", &PTools::coord3D::y)
        .def_readwrite("z", &PTools::coord3D::z)
    ;

    class_< PTools::Atom >("Atom", init<  >())
        .def(init< const PTools::Atom& >())
        .def("GetType", &PTools::Atom::GetType)
        .def("SetType", &PTools::Atom::SetType)
        .def("GetResidType", &PTools::Atom::GetResidType)
        .def("SetResidType", &PTools::Atom::SetResidType)
        .def("GetCharge", &PTools::Atom::GetCharge)
        .def("SetCharge", &PTools::Atom::SetCharge)
        .def("GetCoords", &PTools::Atom::GetCoords)
        .def("SetCoords", &PTools::Atom::SetCoords)
        .def("GetChainId", &PTools::Atom::GetChainId)
        .def("SetChainId", &PTools::Atom::SetChainId)
        .def("GetResidId", &PTools::Atom::GetResidId)
        .def("SetResidId", &PTools::Atom::SetResidId)
        .def("GetAtomNumber", &PTools::Atom::GetAtomNumber)
        .def("SetAtomNumber", &PTools::Atom::SetAtomNumber)
    ;

    class_< PTools::Rigidbody >("Rigidbody", init<  >())
        .def(init< const PTools::Rigidbody& >())
        .def("GetAtom", &PTools::Rigidbody::GetAtom)
        .def("size", &PTools::Rigidbody::size)
        .def("AddAtom", &PTools::Rigidbody::AddAtom)
        .def("GetCoord", &PTools::Rigidbody::GetCoord)
    ;

    def("readPDB", (void (*)(std::ifstream&, PTools::Rigidbody&))&PTools::readPDB);
    def("readPDB", (void (*)(const std::string, PTools::Rigidbody&))&PTools::readPDB);
}

