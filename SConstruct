import os

COMMON_CPP = Split ("""atom.cpp
                       rigidbody.cpp
                       coord3d.cpp
                       pdbio.cpp
                       geometry.cpp
                       atomselection.cpp
                       rmsd.cpp
                       forcefield.cpp
                       pairlist.cpp
                       minimizers/lbfgs_interface.cpp
                       minimizers/routines.f
                       minimizers/lbfgs_wrapper/lbfgsb_wrapper.cpp

                    """)



 #minimizers/lbfgs_wrapper/lbfgsb_wrapper.c   #with bounds
 #minimizers/lbfgs_wrapper/lbfgsb.f    #with bounds

 #minimizers/lbfgs_wrapper/lbfgs_wrapper.cpp 
 #minimizers/lbfgs_wrapper/lbfgs.f
 #minimizers/lbfgs_wrapper/lbfgsb.f



PYTHON_CPP=[]
import fnmatch
import os
for file in os.listdir("Pybindings"):
    if fnmatch.fnmatch(file, "*.cpp"):
        PYTHON_CPP.append("Pybindings/%s"%file)
print PYTHON_CPP

#determine architecture (unix only ?) to determine which 
#static library to import for f95:

#arch = os.uname()[-1]
#if arch=="x86_64":
#	f95="f95_g95_LINUX_64"
#else:
#	f95="f95_g95_LINUX_32"


#COMMON_LIBS=["g2c", f95]
COMMON_LIBS=["g2c"]

COMMON_CPPPATH=['.']
FFLAGS="-g"

                
common=Environment(LIBS=COMMON_LIBS,CPPPATH=COMMON_CPPPATH, LIBPATH=".", FORTRAN='g77 -g -O3 -fPIC' ,   FORTRANFLAGS="-g -fPIC" )
#common=Environment(LIBS=COMMON_LIBS,CPPPATH=COMMON_CPPPATH, LIBPATH=".", FORTRAN = 'g95 -fPIC -g',  FORTRANFLAGS="-g", ENV = {'PATH' : os.environ['PATH']})

#common.Append(CCFLAGS='-Wall -O3 -g -DNDEBUG')                  #fastest(?) release
common.Append(CCFLAGS='-Wall -O2 -g -fPIC')                     #debuging normal
#common.Append(CCFLAGS='-Wall -O2 -g -fPIC -D_GLIBCXX_DEBUG')    #debuging high (use with care !)
#common.Append(CCFLAGS='-Wall -O0 -g -pg -fPIC -DNDEBUG ')        #profiling



python=common.Copy()
nopython=common.Copy()

objects=common.SharedObject(COMMON_CPP)
statics=common.StaticObject(COMMON_CPP)  #to make a static library

python.Append(LIBS=['boost_python', "python2.4"])
#python.Replace(CPPPATH=['.','/usr/include/python2.4'])
python.Append(CPPPATH=['/usr/include/python2.4'])

print "CPPPATH =", python['CPPPATH']

lib1=python.SharedLibrary(File('_ptools.so'),source=[objects,PYTHON_CPP])
#lib2=nopython.SharedLibrary('ptools',source=[objects])
lib2=nopython.StaticLibrary('ptools',source=[statics]) #this makes the library static


Alias('python',lib1)
Alias('cpp',[lib2] )
print "BUILD_TARGETS is", map(str, BUILD_TARGETS)


#### old commands. Not needed but kept here for some time #####

#Alias('cpp',[lib2,tst])
