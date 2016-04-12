.. highlight:: shell

*****************************
Installing the Ptools library
*****************************

The Ptools library has few dependencies namely :

- a C++ compiler (e.g. the `GNU Compiler`_),
- a developpers version of the Python_ interpreter,
- the Boost_ library, 
- the f2c_ library,
- the Cython_ Python package,
- the Git_ version control system,
- the Python pip_ package will be required to install more Python packages.


Installing the dependencies
===========================

Here is the set of instructions required to install all Ptools dependencies 
on a Debian like system (including Ubuntu)::

    $ sudo apt-get update
    $ sudo apt-get install g++ libboost-dev libf2c2-dev python-dev git python-pip

Alternatively you can use the Boost and f2c library legacy version (see
`installing boost and f2c legacy`_)


Building and installing Ptools
==============================

We recommand using a virtual environment. Virtual environment are usefull to
isolate some Python packages from the rest of the Python packages installed
on your system. Hence, it dramatically limits the conflicts in Python packages.
If `virtualenv` is not already present on your system, install it using pip::

    $ pip install virtualenv

Assuming all dependencies are installed in standard locations, here is
the set of instructions required to build Ptools.

1. First you need to initialize a virtual environment in which Ptools will be
installed::

    $ virtualenv ptools-env
    $ source ptools-env/bin/activate



Installing boost and f2c legacy
===============================










.. _Boost: http://www.boost.org/
.. _f2c: http://www.netlib.org/f2c/
.. _Cython: http://cython.org/
.. _GNU Compiler: http://gcc.gnu.org/
.. _Python: http://www.python.org/
.. _Git: http://git-scm.com/
.. _pip: https://pypi.python.org/pypi/pip