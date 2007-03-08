from _ptools import *

def Coord3D_toStr(self):
    return PrintCoord(self)

Coord3D.__str__ = Coord3D_toStr

