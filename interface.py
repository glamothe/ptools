import os
from pyplusplus import module_builder

#Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t( [r"/ibpc/rhea/saladin/Src/ptools/trunk/ptools.h"]
                                      , gccxml_path=r"" 
                                      , define_symbols=[] )


#Well, don't you want to see what is going on?
#mb.print_declarations()

mb.classes().exclude()
mb.free_functions().exclude()

coord3D = mb.class_("Coord3D")
coord3D.include()
normalize = coord3D.member_function("Normalize")
normalize.call_policies = module_builder.call_policies.return_internal_reference()

rigidbody = mb.class_("Rigidbody")
getatom = rigidbody.member_function("GetAtom")
getatom.call_policies = module_builder.call_policies.return_internal_reference()
rigidbody.include()

atom = mb.class_("Atom")
atom.include()


attractEuler = mb.free_functions("AttractEuler")
attractEuler.include()

forceField = mb.class_("ForceField")
forceField.include()

attractForceField = mb.class_("AttractForceField")
attractForceField.include()

lbfgs = mb.class_("Lbfgs")
lbfgs.include()




#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='_ptools' )

#Writing code to file.
mb.split_module( 'Pybindings' )
