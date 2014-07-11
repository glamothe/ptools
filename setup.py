# -*- coding: utf-8 -*-
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import os

import bzrrev


sources =   [
               "cython_wrappers.cpp", 
               "atom.cpp" , 
               "attractrigidbody.cpp", 
               "coordsarray.cpp", 
               "mcopff.cpp", 
               "rigidbody.cpp", 
               "surface.cpp",
               "atomselection.cpp", 
               "basetypes.cpp",
               "forcefield.cpp",
               "pairlist.cpp",
               "rmsd.cpp",
               "version.cpp",
               "attractforcefield.cpp",
               "coord3d.cpp",
               "geometry.cpp",
               "pdbio.cpp",
               "superpose.cpp",
               "minimizers/lbfgs_interface.cpp",
               "minimizers/routines.c",
               "minimizers/lbfgs_wrapper/lbfgsb_wrapper.cpp",
               
             ]
             
sources = [os.path.join('src', i) for i in sources ]  #append the 'src' prefix to all files

sources.append("bindings/_ptools.pyx")


setup(ext_modules=[Extension(
                   "_ptools",                 # name of extension
                   sources = sources,
                   language="c++",   # causes Cython to create C++ source
                   #libraries=['f2c'],
                   library_dirs = ['/usr/lib'],
                   extra_objects = ['dependencies/f2c/libf2c.a'],
                   include_dirs = ['headers', "dependencies/f2c/", "dependencies/boost_1_55_0"],
                   )
                   ], 
      packages = ['.'],
      name="ptools",
      cmdclass={'build_ext': build_ext},
      version = "1.2"
      
      
      
      )
