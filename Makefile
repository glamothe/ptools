# generates the C++ static library
ptools:
	scons -j2 cpp

# generates the C++ shared library (but NOT the bindings files - see below )
python:
	scons -j2 python

# generates python bindings files
# Py++ pygccxml and gccxml required
bindings:
	python interface.py


clean:
	scons -c
