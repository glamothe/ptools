The PTools library
=======================

About:
--------

The PTools library is a toolbox for macromolecular modeling and docking.

A more detailed introduction can be found here: http://www.biomedcentral.com/1472-6807/9/27

PTools: an opensource molecular docking library

Adrien Saladin, Sébastien Fiorucci, Pierre Poulain, Chantal Prévost and Martin Zacharias
BMC Structural Biology. 2009;9:27. 

doi: 10.1186/1472-6807-9-27



Documentation:
--------------

The documentation is provided in the 'Tutorial/doc' directory. Simply run 'make' in this directory 
to get a pdf documentation.

Source code may be parsed by an automatic documentation generator called 'Doxygen'.
This documentation may only help for the C++ part of the library.
after installing Doxygen, simply type 'doxygen' in the directory which contains the 'Doxyfile'.
Then look into the html/ directory and find the index.html file generated...

Very quick setup guide:
-------------------------

```        
    git clone https://github.com/ptools/ptools.git
    cd ptools
    virtualenv env
    source env/bin/activate
    pip install cython
    python setup.py build
    python setup.py install

```    
    
A more detailed version is in the official tutorial. 


About Git branches:
--------------------

This git repository contains few branches. "master" is the latest "stable" version. 
"develop" contains the current development version. Other branches are features branches, 
wich are eventually merged into develop.