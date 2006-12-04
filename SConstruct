list = Split ("""atom.cpp
		 rigidbody.cpp
		 ptools.cpp
             """)


libs = Split("""boost_python
		python2.4""")



SharedLibrary(File('ptools.so'), list, LIBS=libs, CPPPATH='/usr/include/python2.4/')
