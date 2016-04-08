# -*- coding: utf-8 -*-

from __future__ import print_function

import os
import tarfile
import urllib2
import shutil
import subprocess
import StringIO
import sys
import textwrap

from distutils.core import setup, Command
from distutils.extension import Extension
from distutils.command.build import build
from distutils.errors import DistutilsOptionError


# Directory where legacy f2c library will be downloaded and compiled if
# required.
LEGACY_F2C_DIR = 'f2c_sources'

# URL for downloading a tarball containing ptools dependencies.
PTOOLS_DEP_URL = 'https://codeload.github.com/ptools/ptools_dep/legacy.tar.gz'\
                 '/master'


class CheckDeps(Command):
    description = "check if dependencies are present "\
                  "(optionnaly install them)."

    user_options = [
          ('use-legacy-boost', None, "download an old version "
                                     "of boost headers"),
          ('use-legacy-f2c', None, "download and compile an old version "
                                   "of libf2c"),
          ('with-boost-include-dir=', None, 'location of boost headers'),
          ('with-f2c-include-dir=', None, 'location of libf2c headers'),
          ('with-f2c-library=', None, 'location of libf2c.a'),
    ]

    def initialize_options(self):
          self.use_legacy_f2c = False
          self.use_legacy_boost = False
          self.with_boost_include_dir = ''
          self.with_f2c_include_dir = ''
          self.with_f2c_library = ''

    def finalize_options(self):
          if self.use_legacy_f2c and self.with_f2c_include_dir:
              msg = "must supply either --use-legacy-f2c either "\
                    "--with-f2c-include-dir -- not both"
              raise DistutilsOptionError(msg)

          if self.use_legacy_f2c and self.with_f2c_library:
              msg = "must supply either --use-legacy-f2c either "\
                    "--with-f2c-library -- not both"
              raise DistutilsOptionError(msg)

          if self.use_legacy_boost and self.with_boost_include_dir:
              msg = "must supply either --use-legacy-boost either "\
                    "--with-boost-include-dir -- not both"
              raise DistutilsOptionError(msg)

          # if self.use_legacy_f2c:
          #     install_legacy_f2c()

    def run(self):
        print('coucou')



# For compatibility with Python 2.6.
if sys.version_info >= (2, 7):
    check_output = subprocess.check_output
else:
    def _check_output(args):
        return subprocess.Popen(args, stdout=subprocess.PIPE).communicate()[0]
    check_output = _check_output


def info(s, prefix='--', file=sys.stderr, width=80):
    s = textwrap.fill(s, width)
    print(prefix, s, file=file)
    file.flush()


def fatal(s, prefix='ERROR:', file=sys.stderr, width=80, status=1):
    s = textwrap.fill(s, width)
    print(prefix, s, file=file)
    file.flush()
    exit(status)


def warning(s, prefix='WARNING:', file=sys.stderr, width=80):
    s = textwrap.fill(s, width)
    print(prefix, s, file=file)
    file.flush()


def git_version():
    """Return the git revision as a string."""
    cmd = ['git', 'show', '-s', '--format=%h %ci', 'HEAD']
    try:
        git_revision = check_output(cmd).strip()
    except OSError:
        git_revision = 'Unknown version. Please use git to download PTools '\
                       'and get reliable versioning informations'
    return git_revision


def write_version_h(filename):
    """Write a header file with the current git revision hash."""
    git_revision = git_version()
    if git_revision.startswith('Unknown'):
        s = "it seems that you don't have a git directory. "\
            "While the library will compile correcly, informations about"\
            "the current ptools version will be missing. Please use git to"\
            "download PTools and get reliable versioning informations."
        warning(s)

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


def get_environ(s):
    """Return the value of environment variable `s` if present, else
    an empty string."""
    return os.environ[s] if s in os.environ else ''


def find_file(filename, paths):
    """Try to locate a file in a given set of directories.

    Args:
        filename(str): file to look for.
        paths(list[str]): directories to scan.

    Return:
        str: the absolute path to the file in which directory is the first
            directory where the file has been found.
            An empty string if no file has been found.
    """
    for dirname in paths:
        abspath = os.path.abspath(os.path.join(dirname, filename))
        if os.path.exists(abspath):
            return abspath
    return ''


def find_directory(filename, paths):
    """Try to locate a file in a given set of directories.

    Args:
        filename(str): file to look for.
        paths(list[str]): directories to scan.

    Returns:
        str: the absolute path to the directory in which the file
            has been found. An empty string if no file has been found.
    """
    for dirname in paths:
        abspath = os.path.join(dirname, filename)
        if os.path.exists(abspath):
            return os.path.abspath(dirname)
    return ''


def find_executable(filename):
    """Try to locate a program in the PATH environment variable.

    Args:
        filename(str): program to look for.

    Returns:
        str: absolute path to `filename` if in the path, else
            an empty string.
    """
    return find_file(filename, os.environ['PATH'].split(':'))


def pip_install(package):
    try:
        import pip
    except ImportError:
        fatal('pip not found, cannot install Cython')
    else:
        pip.main(['install', package])


def install_cython():
    try:
        import Cython
    except ImportError:
        info('Cython not found, trying to install it with pip')
        pip_install('Cython')


def install_legacy_f2c():
    def f2c_files(members):
        for tarinfo in members:
            if 'libf2c2-20090411' in tarinfo.name:
                yield tarinfo

    def download_legacy_f2c():
        url = PTOOLS_DEP_URL
        tarball_f2c_dir = 'ptools-ptools_dep-66b145b/libf2c2-20090411'
        info("Downloading f2c")
        response = urllib2.urlopen(url)
        compressed = StringIO.StringIO(response.read())
        tar = tarfile.open(fileobj=compressed, mode='r:gz')
        tar.extractall(members=f2c_files(tar))
        tar.close()
        shutil.move(tarball_f2c_dir, LEGACY_F2C_DIR)
        shutil.rmtree(tarball_f2c_dir.split('/')[0])
        info("Downloading f2c done")

    if not os.path.exists(LEGACY_F2C_DIR):
        download_legacy_f2c()

    info("Compiling f2c")
    cflags = 'CFLAGS=-ansi -g -O2 -fomit-frame-pointer -D_GNU_SOURCE '\
             '-fPIC -DNON_UNIX_STDIO -Df2c'

    # First compilation.
    args = ['make', '-C', LEGACY_F2C_DIR, '-f', 'makefile.u', cflags]
    subprocess.call(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Library compilation (write log).
    args = ['make', '-C', LEGACY_F2C_DIR, '-f', 'makefile.u', cflags,
            'libf2c.a']
    log = os.path.join(LEGACY_F2C_DIR, 'compile.log')
    with open(log, 'wt') as logfile:
        subprocess.call(args, stdout=logfile, stderr=subprocess.STDOUT)

    libf2c = os.path.join(LEGACY_F2C_DIR, 'libf2c.a')
    if not os.path.exists(libf2c):
        fatal("error occured during f2c compilation. "
              "Please check {}.".format(log))
    info("Compiling f2c done")


def find_boost():
    """Try to locate the boost include directory (look for
    the shared_array.hpp header file)."""
    boostdir = find_directory('boost/shared_array.hpp',
                              [get_environ('BOOST_INCLUDE_DIR'),
                               '/usr/include', '/usr/local/include',
                               '/opt/local/include'])
    if not boostdir:
        fatal("Boost not found. Specify headers location by using the "
              "BOOST_INCLUDE_DIR environment variable. If it is not "
              "installed, you can either install a recent version "
              "or use the --use-legagy-boost option.")
    else:
        info("Boost include directory found at {}".format(boostdir))
    return boostdir


def find_f2c():
    """Try to locate the libf2c include directory and libf2c.a library."""
    # Search f2c.h.
    f2cdir = find_directory('f2c.h',
                            [get_environ('F2C_INCLUDE_DIR'),
                             LEGACY_F2C_DIR,
                             '/usr/include', '/usr/local/include',
                             '/opt/local/include'])
    if not f2cdir:
        fatal("f2c.h not found. Specify headers location by using the "
              "F2C_INCLUDE_DIR environment variable. If it is not "
              "installed, you can either install a recent version "
              "or use the --use-legagy-f2c option.")
    else:
        info("f2c.h found at {}".format(f2cdir))

    # Search libf2c.a.
    f2clib = find_file('libf2c.a',
                       [get_environ('F2C_LIBRARY'),
                        LEGACY_F2C_DIR,
                        '/usr/lib', '/usr/local/lib', '/opt/local/lib',
                        '/usr/lib64', '/usr/local/lib64'])
    if not f2clib:
        fatal("libf2c.a not found. Specify its location by using the "
              "F2C_LIBRARY environment variable. If it is not "
              "installed, you can either install a recent version "
              "or use the --use-legagy-f2c option.")
    else:
        info("libf2c.a found at {}".format(f2clib))
    return f2cdir, f2clib


def setup_package():
    setup(cmdclass={'check_deps': CheckDeps})
    exit()
    boost_include_dir = find_boost()
    f2c_include_dir, f2clib = find_f2c()

    write_version_h('headers/gitrev.h')

    sources = ['src/cython_wrappers.cpp',
               'src/atom.cpp',
               'src/attractrigidbody.cpp',
               'src/coordsarray.cpp',
               'src/mcopff.cpp',
               'src/rigidbody.cpp',
               'src/surface.cpp',
               'src/atomselection.cpp',
               'src/basetypes.cpp',
               'src/forcefield.cpp',
               'src/pairlist.cpp',
               'src/rmsd.cpp',
               'src/version.cpp',
               'src/attractforcefield.cpp',
               'src/coord3d.cpp',
               'src/geometry.cpp',
               'src/pdbio.cpp',
               'src/superpose.cpp',
               'src/scorpionforcefield.cpp',
               'src/minimizers/lbfgs_interface.cpp',
               'src/minimizers/routines.c',
               'src/minimizers/lbfgs_wrapper/lbfgsb_wrapper.cpp']

    sources.append("bindings/_ptools.pyx")

    ptools = Extension('_ptools',
                       sources=sources,
                       language='c++',
                       library_dirs=[boost_include_dir],
                       include_dirs=['headers',
                                     f2c_include_dir, boost_include_dir],
                       extra_objects=[f2clib])

    cgopt = Extension('cgopt',
                      sources=['PyAttract/cgopt.pyx',
                               'PyAttract/chrg_scorpion.c'],
                      language='c',
                      include_dirs=[f2c_include_dir, 'PyAttract'],
                      extra_objects=[f2clib])

    # At this stage, Cython should have been installed.
    from Cython.Distutils import build_ext
    setup(ext_modules=[ptools, cgopt],
          cmdclass={'build_ext': build_ext},
          packages=['.'],
          name='ptools',
          version='1.2')


if __name__ == '__main__':
    setup_package()
