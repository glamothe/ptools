COMMON_CPP = Split ("""atom.cpp
                       rigidbody.cpp
                       coord3d.cpp
                       pdbio.cpp
                       geometry.cpp
                       atomselection.cpp
                       rmsd.cpp
                       forcefield.cpp
                       pairlist.cpp
                       nonbon8.f90
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

                    
COMMON_LIBS=["g2c", "f95"]
#COMMON_LIBS=["g2c"]

COMMON_CPPPATH=['.']
FFLAGS="-g"

                
#common=Environment(LIBS=COMMON_LIBS,CPPPATH=COMMON_CPPPATH, LIBPATH=".",  FORTRANFLAGS="-g" )
common=Environment(LIBS=COMMON_LIBS,CPPPATH=COMMON_CPPPATH, LIBPATH=".", FORTRAN = '/home/asaladin/bin/g95-install/bin/g95 -fPIC -g',  FORTRANFLAGS="-g" )

common.Append(CCFLAGS='-Wall -O2 -g -DNDEBUG')
#common.Append(CCFLAGS='-Wall -O2 -g -D_GLIBCXX_DEBUG')



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
Alias('cpp',lib2)
print "BUILD_TARGETS is", map(str, BUILD_TARGETS)



#to be able to compile using pyste:
#env = Environment()
#bld = Builder(action = 'pyste --module=_ptools -I. $SOURCE')
#env.Append(BUILDERS = {'Pyste' : bld})

# uncomment this line if pyste is installed
#env.Pyste('ptools.pyste')

#env.SharedLibrary(File('ptools.so'), list, LIBS=libs, CPPPATH=['/usr/include/python2.4/','.'])
