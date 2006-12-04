#include <boost/python.hpp>

#include "atom.h"
#include "rigidbody.h"

using namespace boost::python;
using namespace PTools;



BOOST_PYTHON_MODULE(ptools)
{
    // Add regular functions to the module.
    class_<Coord3D>("Coord3D")
      .def_readwrite("x", &Coord3D::x)
      .def_readwrite("y", &Coord3D::y)
      .def_readwrite("z", &Coord3D::z);	

    class_<Atom>("Atom")
      .def("GetType", &Atom::GetType, "Salut vieux")
      //.def_readwrite("type", &Atom::type)
      .def("SetType", &Atom::SetType);


    class_<Rigidbody> ("Rigidbody")
       .def("size", &Rigidbody::size)
        
       .def("GetAtom", &Rigidbody::GetAtom,return_value_policy<copy_non_const_reference>() );
        //.def("GetAtom", &Rigidbody::GetAtom,return_value_policy<copy_const_reference>() );
        //.add_property("atom", &Var::get, &Var::set);
}
