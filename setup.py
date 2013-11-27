# -*- coding: utf-8 -*-
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import bzrrev

setup(ext_modules=[Extension(
                   "_ptools",                 # name of extension
                   ["_ptools.pyx", "cython_wrappers.cpp", "atom.cpp" , "attractrigidbody.cpp", "coordsarray.cpp", "mcopff.cpp", "rigidbody.cpp", "surface.cpp",
                   "atomselection.cpp", "basetypes.cpp", "forcefield.cpp", "pairlist.cpp", "rmsd.cpp", "version.cpp",
                   "attractforcefield.cpp", 
                   "scorpionforcefield.cpp", 
                   "coord3d.cpp", "geometry.cpp", "pdbio.cpp", "superpose.cpp", "minimizers/lbfgs_interface.cpp", "minimizers/routines.c", "minimizers/lbfgs_wrapper/lbfgsb_wrapper.cpp" ], #  our Cython source
                   language="c++",  # causes Cython to create C++ source
                   #libraries=['f2c'],
                   library_dirs = ['/usr/lib'],
                   extra_objects = ['/usr/lib/libf2c.a'],
                   )

                     ], 
      cmdclass={'build_ext': build_ext},

      packages = ['.'],
      name="ptools",
      version = "1.2"
      
      )

