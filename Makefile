ptools:
	scons cpp
all: ptools.cpp
	scons -j2 cpp python
clean:
	scons -c

ptools.cpp: ptools.pyste
	pyste --module=_ptools ptools.pyste
