#!/usr/bin/env python

from ptools import *
import math
import sys
import os
from stat import *

import shelve
import re


class StructureI:
    def __cmp__(self, other):
            if self.trans < other.trans:
                return -1
            if self.trans > other.trans:
                return 1
            return cmp(self.rot, other.rot)
    pass



class Extractor:
    def __init__(self,filename):
        #assert(isinstance(ligand, Rigidbody))
        #self.ligand = Rigidbody(ligand)
        
        #check if output file has a database associated...
        databasefile = "%s.db"%filename
        self.d = openDatabase(filename)
        
        
    def getMatrix(self, key):
        return self.d[key].matrix
    def getStructure(self, lig, key):
        return rigidXMat44(lig ,self.d[key].matrix)




def rigidXMat44(rigid, mat):
    assert(isinstance(rigid,Rigidbody))
    out=Rigidbody(rigid)
    for i in range(rigid.Size()):
        coords=rigid.GetCoords(i)
        coords2=Coord3D()
        coords2.x = mat[0][0]*coords.x + mat[0][1]*coords.y + mat[0][2]*coords.z + mat[0][3]
        coords2.y = mat[1][0]*coords.x + mat[1][1]*coords.y + mat[1][2]*coords.z + mat[1][3]
        coords2.z = mat[2][0]*coords.x + mat[2][1]*coords.y + mat[2][2]*coords.z + mat[2][3]
        out.SetCoords(i, coords2)
    return out




def readStructures(file):


    out_attach = open(file,'r')
    #determine output format version:
    firstline = out_attach.readline()
    firstline = firstline.split()
    if (len(firstline) == 0):
        outversion = 1
    else:
        outversion = int(firstline[0])

    #now read all lines until it finds a line like: "==        1      1   -14.0032942 94.6604593764"
    # (version 1)

    if outversion == 1 :
        structures = {}
        lststructures = []
        #structures = []
        #dicstruct = {}
        begin=False
        lines = out_attach.readlines()
        for l in lines:
            lspl = l.split()
            if len(lspl) >0 and lspl[0] == "==":
                begin = True
                struct = StructureI()
                struct.trans = int(lspl[1])
                struct.rot = int(lspl[2])
                struct.ener = float(lspl[3])
                struct.rmsd = lspl[4]
                matrix=[]
            if (begin):
                lspl = l.split()
                if lspl[0]=="MAT":
                    matrix.append( [float(lspl[i]) for i in range(1,5) ]  )
                else:
                    if lspl[2] == "END":
                        begin = False   
                        struct.matrix = matrix
                        structures.setdefault(struct.trans,{})[struct.rot]=struct
                        lststructures.append(struct)
                        #structures.append(struct)


    return structures,lststructures




def store(structures,filename):
    sys.stderr.write("storing database\n")
    d = shelve.open(filename)
    for struct in structures:
        key = "%s:%s"%(struct.trans,struct.rot)
        d[key] = struct
    return d





def openDatabase(filename):
    databasefile = "%s.db"%filename
    flag1 = os.path.exists(databasefile)
    if flag1:
        sys.stderr.write("file exists\n")
        #check to see if database is more recent than outfile
        statdb=os.stat(databasefile)
        statout=os.stat(filename)
        if statdb[ST_MTIME] > statout[ST_MTIME]:  #ok database is more recent, no need to regenerate it
            #we can use the database here :-)
            d = shelve.open(databasefile)
            return d

        else:
            #we must delete the database if it exists and generate a new one
            sys.stderr.write("Removing the old database file\n")
            if flag1: os.remove(databasefile)
    #at this point there is no database or it has been removed
    #we must generate it
    sys.stderr.write("reading outfile and creating database\n")
    structures,lststruct = readStructures(filename)

    d=store(lststruct, databasefile)
    
    #reads included receptor, ligand, etc:
    p=re.compile("^compressed ([a-zA-Z0-9\.]+) : \"([^\"]+)\"")
    file = open(filename,'r')
    for l in file.readlines():
        m=p.match(l)
        if m:
            g=m.groups()
            sys.stderr.write("adding filename: %s\n"%g[0])
            d[g[0]]=g[1]

    sys.stderr.write(" done !\n")
    return d
 


def extract(outputfilename, ligand, transnb, rotnb):
    d=openDatabase(outputfilename)
    key="%i:%i"%(transnb,rotnb)
    struct = d[key]
    lig2 = rigidXMat44(ligand,struct.matrix)
    return lig2





def main():
    nargs = len(sys.argv)
    if nargs < 5:
        print "usage: Extract 'outputfile' LigOriginalCoordinates translationNumber RotationNumber"
        raise SystemExit

    outfile = sys.argv[1]
    ligname = sys.argv[2]
    transNB = int(sys.argv[3])
    rotNB =   int(sys.argv[4])

    lig = Rigidbody(ligname)
    lig2=extract(outfile, lig, transNB, rotNB)

    pdb=[]
    for i in range(lig2.Size()):
        atom=lig2.CopyAtom(i)
        pdb.append(atom.ToPdbString())
    print "".join(pdb)









if __name__ == "__main__":
    main()


__all__=["extract"]