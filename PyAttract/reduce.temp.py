#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import copy
import os
from optparse import OptionParser

usage = "%prog --ff force_field [--dna] [--cgopt] [--dgrid 1.5] atomic_file.pdb [--allow_missing] > reduced_file.red"
version = "%prog 1.0"
parser = OptionParser(usage)

# --ff option: choice of forcefield
parser.add_option("--ff",type="str",dest="ffversion",help="choice of CG force field : attract1, attract2 or scorpion") 

# --cgopt option: choice of charge optimization with SCORPION
parser.add_option("--cgopt", dest="optimizecharges", action="store_true", default=False,
                  help="optimize SCORPION coarse grained charges, works only with FF Scorpion")

parser.add_option("--dgrid", type="float",dest="delgrid", default=1.5,
                  help="grid spacing (A) for charge optimization (default is 1.5), works only with FF Scorpion and -cgopt option")

# --dna option: reduce dna
parser.add_option("--dna", action="store_true", dest="molDna", default=False, help="reduce dna, works only with FF Attract1")

parser.add_option("--allow_missing", action="store_true", dest="warning",default=False, help="don't stop program if atoms are missing, only display a warning on stderr")


(options, args) = parser.parse_args()

#==========================================================
# check options
#==========================================================

# define reduce data files subdirectory
data_dir="reduce_data/"

if ( (options.molProt or options.molDna) and ( len(args) > 0) ):
        atomicName = args[0]
        completePath=sys.argv[0]
        scriptdir,scriptname = os.path.split(completePath)
        if options.molProt:
                redName = os.path.join(scriptdir,data_dir+"at2cg.prot.dat")
        if options.molDna:
                redName = os.path.join(scriptdir,data_dir+"at2cg.dna.dat")
        ffName = os.path.join(scriptdir,data_dir+"ff_param.dat")
        convName = os.path.join(scriptdir,data_dir+"type_conversion.dat")  
else:
        parser.error("please specify molecule type (--prot or --dna) and atomic file")

# define other parameter files
if options.redName:
        redName  = options.redName
if options.ffName:
        ffName   = options.ffName
if options.convName:
        convName = options.convName

#==========================================================
# check files
#==========================================================
# check if a required file is found
def checkFile(name):
        flag = os.path.exists(name)
        if  not flag :
                sys.stderr.write("ERROR: cannot find %s \n" %(name))
                exit(2)	
checkFile(atomicName)
checkFile(redName)
checkFile(ffName)
checkFile(convName)

