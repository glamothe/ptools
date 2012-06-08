# -*- coding: utf-8 -*-
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(ext_modules=[Extension(
                   "cgopt",                 # name of extension
                   ["cgopt.pyx", "chrg_scorpion.c",
                    ], #  our Cython source
                   language="c",
                   include_dirs=['/opt/local/include/'],  
                   extra_objects = ['/usr/lib/libf2c.a'],                
 

)],   # causes Cython to create C source
      cmdclass={'build_ext': build_ext})
