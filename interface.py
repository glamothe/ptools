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


coordsarray = mb.class_("CoordsArray")
coordsarray.include()
#matrix44xVect = coordsarray.member_function


rigidbody = mb.class_("Rigidbody")
#getatom = rigidbody.member_function("GetAtomReference")
#getatom.call_policies = module_builder.call_policies.return_internal_reference()
rigidbody.include()

attractrigidbody=mb.class_("AttractRigidbody")
attractrigidbody.include()


Region = mb.class_("Region")
Region.include()

Mcoprigid=mb.class_("Mcoprigid")
Mcoprigid.include()


atomproperty=mb.class_("Atomproperty")
atomproperty.include()

atom = mb.class_("Atom")
atom.include()


#attractEuler = mb.free_functions("AttractEuler")
#attractEuler.include()

forceField = mb.class_("ForceField")
forceField.include()

mb.class_("BaseAttractForceField").include()

attractForceField1 = mb.class_("AttractForceField1")
attractForceField1.include()

attractForceField2 = mb.class_("AttractForceField2")
attractForceField2.include()

McopForceField = mb.class_("McopForceField")
McopForceField.include()


AtomPair = mb.class_("AtomPair")
AtomPair.include()

#this will introduce the PairList template
#this is tricky and quite ugly. Looking for a better alternative
#pairlist = mb.class_("T_PairList<PTools::Rigidbody>")  #regular pairlist (for Rigidbody)
#pairlist.include()


#att2pairlist=mb.class_("T_PairList<PTools::AttractRigidbody>") #the new Attract 2 pairlist (works with AttractRigidbody)

att2pairlist=mb.class_("Attract2PairList")
att2pairlist.include()
#mb.namespace( 'py_details' ).exclude()  #exclude the py_details ugly namespace


lbfgs = mb.class_("Lbfgs")
lbfgs.include()

rmsd = mb.free_function("Rmsd")
rmsd.include()

Norm = mb.free_function("Norm")
Norm.include()
Norm2 = mb.free_function("Norm2")
Norm2.include()
Dist = mb.free_function("Dist").include()
Dist2 = mb.free_function("Dist2").include()

mb.free_function("Dihedral").include()
mb.free_function("Angle").include()


PrintCoord=mb.free_function("PrintCoord")
PrintCoord.include()
WritePDB=mb.free_function("WritePDB")
WritePDB.include()


atomselection = mb.class_("AtomSelection")
atomselection.include()

screw =mb.class_("Screw")
printscrew = screw.member_function("print")
printscrew.rename("Print")
screw.include()

superimpose=mb.free_function("superimpose")
superimpose.include()

surf=mb.class_("Surface")
surf.include()

Matrix=mb.class_("Array2D<double>")
Matrix.include()
Matrix.alias = "Matrix"
Matrix.operators("()", arg_types=["int","int"]).exclude()
Matrix.member_function("id").exclude()

mb.class_("Model").include()
mb.class_("Pdb").include()



try:
    surreal = mb.class_("surreal")
    surreal.include()
except :
    print "no surreal type this time."

#Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='_ptools' )
mb.code_creator.replace_included_headers( ["ptools.h"] )


#Writing code to file.
mb.split_module( 'Pybindings' )
