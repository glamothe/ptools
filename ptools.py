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


def Rigidbody_remoteLoad(pdbname):
	import urllib



