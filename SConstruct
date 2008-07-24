import os
import os.path


COMMON_CPP = Split ("""atom.cpp
                       rigidbody.cpp
                       attractrigidbody.cpp
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
                       mcopff.cpp
                       surface.cpp
                       coordsarray.cpp
                       QBestFit.cpp
                    """)



COMMON_LIBS=[""]
#COMMON_LIBS=["gfortran"]
#COMMON_CPPPATH=['.', '/sw/include/boost-1_33_1']
COMMON_CPPPATH=['.']
FFLAGS="-g"



def FIND_FILE(name,path, useEnvPath=False):
   if useEnvPath:
      additional_path = os.environ['PATH'].split(':')
      path.extend(additional_path)
   
   for p in path:
     if os.path.exists(p+'/'+name):
        return p+'/'+name
   return None 



def FIND_HEADER(names, paths, useEnvPath=False):
   #find a library in a given set of directories
   
   if useEnvPath:
      additional_path = os.environ['PATH'].split(':')
      paths.extend(additional_path)
   
   for p in paths:
      if os.path.exists(p):
         files=os.listdir(p)
	 for n in names:
	    if os.path.exists(p+"/"+n):
	       return p
	       	       
   return None



#find g++ / g77 or g++/gfortran:
FORTRANPROG = ""
LIB_PATH=['.']

print "Searching for compilers..."
gpp = FIND_FILE("g++", ["/usr/bin"], True)
if gpp is None:
   print "Error: cannot locate g++, compilation aborted"
   Exit(1)
else:
   print "g++ found here: ", gpp

g77 = FIND_FILE("g77", ["/usr/bin","/sw/bin/"], True)
if g77 is None:
   print "Cannot locate g77 fortran compiler, trying gfortran:"
   gfortran=FIND_FILE("gfortran", ["/usr/bin"], True)
   if gfortran is None:
      print "Error: no fortran compiler found, aborting"
      Exit(1)
   else: #gfortran compiler, try to locate libgfortran
      print "using fortran compiler: gfortran"
      FORTRANPROG="gfortran -O2 -g -fPIC"
      gfortranlib=FIND_HEADER(["libgfortran.a", "libgfortran.la",\
      "libgfortran.so"], ["/usr/lib","/sw/lib","/usr/lib/gcc/x86_64-redhat-linux/3.4.6/"], True)
      if gfortranlib is not None:
         print "libgfortran found here: ", gfortranlib
         LIB_PATH.append(gfortranlib)
         COMMON_LIBS.append("gfortran")
      else:
         print "WARNING: libgfortran not found, may not compile..."      
    
else: #g77 compiler
      print "using fortran compiler: g77"
      FORTRANPROG="g77 -O2 -g -fPIC"
      g2clib = FIND_HEADER(["libg2c.a", "libg2c.so", "libg2c.la"], ["/usr/lib",
      "/usr/local/lib/", "/sw/lib/","/usr/lib/gcc/x86_64-redhat-linux/3.4.6/"],True)
      if g2clib is None:
         print "Warning: libg2c not found, may not compile..."
      else:
         print "   libg2c found here:", g2clib
	 LIB_PATH.append(g2clib)
         COMMON_LIBS.append("g2c")

      
   
   







boostdir=FIND_HEADER(["boost/shared_array.hpp"], ["/usr/include", \
"/sw/include/boost-1_33_1"] )

if boostdir is None:
   print "cannot locate Boost hearders directory, still trying to compile..."
else:
   print "boost directory found here: ", boostdir 
   COMMON_CPPPATH.append(boostdir)

python25dir=FIND_HEADER(["Python.h"], ["/usr/include/python2.5", \
"/sw/include/python2.5/Python.h"])

if python25dir is not None:
   print "python2.5 found, configuring path and libs"
   PYTHON_CPPPATH=[python25dir]
   PYTHON_LIBS=["python2.5"]


if python25dir is None:
   print "cannot locate python2.5, tying with python2.4:"
   python24dir = FIND_HEADER(["Python.h"], ["/usr/include/python2.4/",
   "/sw/include/python2.4/"])
   if python24dir is None:
      print "cannot locate Python2.4, the library may not compile..."
   else:
      PYTHON_CPPPATH=[python24dir]
      PYTHON_LIBS=["python2.4"]


 



PYTHON_CPP=[]
import fnmatch
import os
for file in os.listdir("Pybindings"):
    if fnmatch.fnmatch(file, "*.cpp"):
        PYTHON_CPP.append("Pybindings/%s"%file)
#print PYTHON_CPP

#determine architecture (unix only ?) to determine which 
#static library to import for f95:

#arch = os.uname()[-1]
#if arch=="x86_64":
#	f95="f95_g95_LINUX_64"
#else:
#	f95="f95_g95_LINUX_32"




print "common cpp path:", COMMON_CPPPATH                
		
common=Environment(LIBS=COMMON_LIBS,CPPPATH=COMMON_CPPPATH, LIBPATH=LIB_PATH, FORTRAN=FORTRANPROG,   FORTRANFLAGS="-g -fPIC" )


#common.Append(CCFLAGS='-Wall -O2 -fPIC -Woverloaded-virtual -DNDEBUG')                  #fastest(?) release
#common.Append(CCFLAGS='-Wall -O2 -g -fPIC -D_GLIBCXX_DEBUG')    #debuging high (use with care !)
#common.Append(CCFLAGS='-Wall -O0 -g -pg -fPIC -DNDEBUG ')        #profiling


#check for some boost header files:
has_boost=False
conf = Configure(common)
if conf.CheckCXXHeader("boost/shared_array.hpp"):
    has_boost=True

if not has_boost:
    print "cannot find boost::shared_array include file"
    Exit(1)
common = conf.Finish()





python=common.Copy()
nopython=common.Copy()

python.Append(CPPPATH=PYTHON_CPPPATH, LIBS=PYTHON_LIBS)


objects=common.SharedObject(COMMON_CPP)
statics=common.StaticObject(COMMON_CPP)  #to make a static library

python.Append(LIBS=['boost_python'])


#check python header file:
conf = Configure(python)
if not conf.CheckCHeader('Python.h'):
        print "you must install either python2.4-dev or python2.5-dev"
        Exit(1)

env = conf.Finish()


print "using CPPPATH =", python['CPPPATH']



lib1=python.SharedLibrary(File('_ptools.so'),source=[objects,PYTHON_CPP])
#lib2=nopython.SharedLibrary('ptools',source=[objects])
lib2=nopython.StaticLibrary('ptools',source=[statics]) #this makes the pure C++ static library


Alias('python',lib1)
Alias('cpp',[lib2] )
print "BUILD_TARGETS is", map(str, BUILD_TARGETS)


#### old commands. Not needed but kept here for some time #####

#Alias('cpp',[lib2,tst])
