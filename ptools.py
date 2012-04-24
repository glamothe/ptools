# -*- coding: utf-8 -*-
from _ptools import *


def getPDB(pdbname):
    import urllib2
    pdb = urllib2.urlopen("http://www.rcsb.org/pdb/files/%s.pdb"%pdbname)
    #content = pdb.read()
    
    rigid = Rigidbody(pdb)
    
    return rigid


