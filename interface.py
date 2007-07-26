import os
from pyplusplus import module_builder

#Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t( [os.path.abspath('./ptools.h'), os.path.abspath('./py_details.h')]
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
getatom = rigidbody.member_function("GetAtomReference")
getatom.call_policies = module_builder.call_policies.return_internal_reference()
rigidbody.include()

attractrigidbody=mb.class_("AttractRigidbody")
attractrigidbody.include()

atomproperty=mb.class_("Atomproperty")
atomproperty.include()

atom = mb.class_("Atom")
atom.include()


attractEuler = mb.free_functions("AttractEuler")
attractEuler.include()

forceField = mb.class_("ForceField")
forceField.include()

attractForceField = mb.class_("AttractForceField")
attractForceField.include()

attractForceField2 = mb.class_("AttractForceField2")
attractForceField2.include()


#this will introduce the PairList template
#this is tricky and quite ugly. Looking for a better alternative
pairlist = mb.class_("T_PairList<PTools::Rigidbody>")  #regular pairlist (for Rigidbody)
pairlist.include()
att2pairlist=mb.class_("T_PairList<PTools::AttractRigidbody>") #the new Attract 2 pairlist (works with AttractRigidbody)
att2pairlist.include()
mb.namespace( 'py_details' ).exclude()  #exclude the py_details ugly namespace


lbfgs = mb.class_("Lbfgs")
lbfgs.include()

rmsd = mb.free_function("Rmsd")
rmsd.include()

Norm = mb.free_function("Norm")
Norm.include()
Norm2 = mb.free_function("Norm2")
Norm2.include()
PrintCoord=mb.free_function("PrintCoord")
PrintCoord.include()
WritePDB=mb.free_function("WritePDB")
WritePDB.include()


atomselection = mb.class_("AtomSelection")
atomselection.include()



#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='_ptools' )

#Writing code to file.
mb.split_module( 'Pybindings' )
