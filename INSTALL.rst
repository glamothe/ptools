.. highlight: shell

*****************************
Installing the Ptools library
*****************************

The Ptools library has few dependencies namely :

- a C++ compiler (e.g. the `GNU Compiler`_),
- a developers version of the Python_ interpreter,
- the Boost_ library, 
- the f2c_ library,
- the Cython_ Python package,
- the Git_ version control system,
- the pip_ Python package will be required to install more Python packages.


Installing the dependencies
===========================

Here is the set of instructions required to install all Ptools dependencies 
on a Debian like system (including Ubuntu)::

    $ sudo apt-get update
    $ sudo apt-get install g++ libboost-dev libf2c2-dev python-dev git python-pip

Alternatively you can use the Boost and f2c library legacy version (see
`Building with boost and f2c legacy`_)

We also recommand using a virtual environment. Virtual environments are usefull
to isolate Python packages from the rest of the Python packages installed
on your system. Hence, it dramatically limits the potential conflicts.
If ``virtualenv`` is not already present on your system, install it using ``pip``::

    $ pip install virtualenv


Building and installing Ptools
==============================

Quick setup guide
-----------------

Assuming all dependencies are installed in standard locations, here is
the set of instructions required to build Ptools.

1. First you need to setup a virtual environment in which Ptools will be 
installed::

    $ virtualenv ptools-env
    $ source ptools-env/bin/activate
    $ pip install cython

    Note that, as you are now running in a virtual environment, Python packages
    that you install are not installed system-wide but only in your local
    environment.

2. Retrieve Ptools sources from its GitHub repository::

    $ git clone https://github.com/ptools/ptools.git

3. Build and install Ptools::

    $ cd ptools
    $ python setup.py install

Ptools has been installed to your ``ptools-env`` environment. To use it in a
new terminal, just type::

    $ source ptools-env/bin/activate
    
    You will then be able to import it in your Python scripts


Troubleshooting
===============


boost/shared_array.hpp: No such file or directory
-------------------------------------------------

Ptools did not find the boost headers. You can explicitely tell Ptools where
to find it using the ``--with-boost-include-dir`` option at build time.
In this example, boost has been installed in the directory ``/opt/boost``::

    $ python setup.py build_ext --with-boost-include-dir=/opt/boost/include
    $ python setup.py install

Alternatively, you can use the ``BOOST_INCLUDE_DIR`` environment variable::

    $ export BOOST_INCLUDE_DIR=/opt/boost/include
    $ python setup.py install


Building with Boost and f2c legacy
===================================

You might want to build Ptools using Ptools approved legacy versions of 
``Boost`` and ``f2c`` libraries.
Doing so has been made quite straight forward by Ptools developers team::

    $ python setup.py build_ext --use-legacy-f2c
    $ python setup.py install




.. _Boost: http://www.boost.org/
.. _f2c: http://www.netlib.org/f2c/
.. _Cython: http://cython.org/
.. _GNU Compiler: http://gcc.gnu.org/
.. _Python: http://www.python.org/
.. _Git: http://git-scm.com/
.. _pip: https://pypi.python.org/pypi/pip