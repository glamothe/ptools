#!/usr/bin/env python

import sys
import re
from ptools import *
import math
import argparse

parser = argparse.ArgumentParser(description='Process some integers.')

parser.add_argument("file", metavar="H-parameters.txt", help="the resulting file of extractHelicoidalParameters.py on the docking simulation by PyAttract")
parser.add_argument('-p','--pitch', nargs=2,type=float, metavar=('min', 'max'),default=None,help="get only results with a pitch between min and max")
parser.add_argument('-n','--nbMono', nargs=2,type=float, metavar=('min', 'max'),default=None,help="get only results with a number of monomer by turn between min and max")
parser.add_argument('-d','--direction', choices=["R","L"],default=None, help="get only results that are rigth-handed (R) or left-handed(L) helix")
args = parser.parse_args()

if not (args.pitch or args.nbMono or  args.sense):
    print "At least one option expected (-p,-n or -s). Use -h for more details."
    raise SystemExit

f = open(args.file,"r")
pitch,nbMono,sense= False,False,False
if args.pitch:
    pitch_inf, pitch_sup = args.pitch
    pitch = True
if args.nbMono:
    nbmono_inf, nbmono_sup = args.nbMono
    nbMono = True

select = []

for l in f:
    lspl = l.split()
    if pitch and not(pitch_inf<float(lspl[3])< pitch_sup):
        continue
    if nbMono and not(nbmono_inf<float(lspl[2])< nbmono_sup):
        continue
    if args.direction and not( lspl[4] == args.direction):
        continue
    select.append ([float(lspl[5]),l])

select=sorted(select)

for el in select:
    print el[1],
#f.close()