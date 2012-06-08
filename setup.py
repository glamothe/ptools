# -*- coding: utf-8 -*-
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(ext_modules=[Extension(
                   "_ptools",                 # name of extension
                   ["_ptools.pyx", "cython_wrappers.cpp", "atom.cpp" , "attractrigidbody.cpp", "coordsarray.cpp", "mcopff.cpp", "rigidbody.cpp", "surface.cpp",
                   "atomselection.cpp", "basetypes.cpp", "forcefield.cpp", "pairlist.cpp", "rmsd.cpp", "version.cpp",
                   "attractforcefield.cpp","scorpionforcefield.cpp", "coord3d.cpp", "geometry.cpp", "pdbio.cpp", "superpose.cpp" ], #  our Cython source
                   language="c++")],   # causes Cython to create C++ source
                   include_dirs=['/opt/local/include/'],                  
       cmdclass={'build_ext': build_ext})
