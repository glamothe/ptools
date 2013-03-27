# -*- coding: utf-8 -*-
from _ptools import *

def Coord3D_toStr(self):
    return PrintCoord(self)

Coord3D.__str__ = Coord3D_toStr

try:
    s=surreal(5)
except:
    def surreal(i):
        return i


def _version_print(self):
    return "Ptools library version %i"%self.revnb

Version.__str__ = _version_print


def getPDB(pdbname):
    import urllib2
    pdb = urllib2.urlopen("http://www.rcsb.org/pdb/files/%s.pdb"%pdbname)
    content = pdb.read()
    return content


