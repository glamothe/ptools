# -*- coding: utf-8 -*-
import os
import subprocess
import sys
import textwrap

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext


# !!! PLEASE override the following two variables
# !!! to define a custom path to required dependencies:

# user-defined path to libf2c.a
user_path_libf2c = ""

# user-defined path to f2c.h
user_path_f2c_h = ""

# user-defined path to boost library:
user_path_boost = ""


# =============================================================================

def git_version():
    """Return the git revision as a string."""
    cmd = ['git', 'show', '-s', '--format=%h %ci', 'HEAD']
    try:
        git_revision = subprocess.check_output(cmd).strip()
    except OSError:
        git_revision = 'Unknown version. Please use git to download PTools '\
                       'and get reliable versioning informations'
    return git_revision


def write_version_h(filename):
    """Write a header file with the current git revision hash."""
    git_revision = git_version()
    if git_revision.startswith('Unknown'):
        s = "WARNING: it seems that you don't have a git directory."\
            "While the library will compile correcly, informations about"\
            "the current ptools version will be missing. Please use git to"\
            "download PTools and get reliable versioning informations."
        print textwrap.fill(s)

    content = """
/*
 * This file was generated automatically.
 * You should not modify it manually, as it may be re-generated.
 */

#ifndef GITREV_H
#define GITREV_H
#define GIT_REVID   "%(git_revision)s"
#endif /* GITREV_H */
"""
    with open(filename, 'w') as f:
        f.write(content % {'git_revision': git_revision})


# == Methods to locate headers and libraries ==

def find_file(name, path, useEnvPath=False):
    """finds a file named 'name' in a serie of directories
    given in 'path' """
    if useEnvPath:
        additional_path = os.environ['PATH'].split(':')
        path.extend(additional_path)
    for p in path:
        fullfilepath = os.path.join(p, name)
        if os.path.exists(fullfilepath):
            return fullfilepath
    return None  # no file found


def find_header(names, paths, useEnvPath=False):
    """Find a header in a given set of directories."""
    if useEnvPath:
        additional_path = os.environ['PATH'].split(':')
        paths.extend(additional_path)
    for p in paths:
        if os.path.exists(p):
            for n in names:
                if os.path.exists(os.path.join(p, n)):
                    return p
    return None


print "Trying to locate the boost libraries, modify setup.py to change the "\
      " default search path"
boostdir = find_header(["boost/shared_array.hpp"],
                       [user_path_boost, "./ptools_dep/boost_1_55_0",
                        "/usr/include", "/opt/local/include"])

print "Trying to locate libf2c.a static library"
f2clib = find_file("libf2c.a",
                   [user_path_libf2c, "./ptools_dep/f2c", "/usr/lib/",
                    "/usr/local/lib/", "/usr/lib64/"])
f2c_header = find_header(["f2c.h"],
                         [user_path_f2c_h, "./ptools_dep/f2c", "/usr/include",
                          "/usr/local/f2c/"])

not_found_message = 'Note: You can add a custom search path by editing this '\
                    'file (setup.py).\nYou can also install locally the '\
                    'missing dependencies by running: sh ./install-deps.sh'


if boostdir is None:
    print "Cannot locate boost library", not_found_message
    sys.exit(1)
if f2clib is None:
    print "Cannot locate libf2c", not_found_message
    sys.exit(1)


write_version_h('headers/gitrev.h')

print "using boost from", boostdir
print "using f2clib from", f2clib

sources = ["cython_wrappers.cpp",
           "atom.cpp",
           "attractrigidbody.cpp",
           "coordsarray.cpp",
           "mcopff.cpp",
           "rigidbody.cpp",
           "surface.cpp",
           "atomselection.cpp",
           "basetypes.cpp",
           "forcefield.cpp",
           "pairlist.cpp",
           "rmsd.cpp",
           "version.cpp",
           "attractforcefield.cpp",
           "coord3d.cpp",
           "geometry.cpp",
           "pdbio.cpp",
           "superpose.cpp",
           "scorpionforcefield.cpp",
           "minimizers/lbfgs_interface.cpp",
           "minimizers/routines.c",
           "minimizers/lbfgs_wrapper/lbfgsb_wrapper.cpp"]

# Append the 'src' prefix to all files
sources = [os.path.join('src', i) for i in sources]

sources.append("bindings/_ptools.pyx")


ptools = Extension('_ptools',
                   sources=sources,
                   language='c++',
                   library_dirs=[boostdir],
                   include_dirs=['headers', f2c_header, boostdir],
                   extra_objects=[f2clib])

cgopt = Extension('cgopt',
                  sources=['PyAttract/cgopt.pyx', 'PyAttract/chrg_scorpion.c'],
                  language='c',
                  include_dirs=[f2c_header, 'PyAttract'],
                  extra_objects=[f2clib])

setup(ext_modules=[ptools, cgopt],
      cmdclass={'build_ext': build_ext},
      packages=['.'],
      name='ptools',
      version='1.2')
