all: ptools python

# generates the C++ static library
ptools:
	scons -j4 cpp

# generates the C++ shared library (but NOT the bindings files - see below )
python: bindings
	scons -j4 python

# generates python bindings files
# Py++ pygccxml and gccxml required
bindings:
	python interface.py

clean:
	scons -c
