# -*- coding: utf-8 -*-
from ptools import *
import random
import unittest

import math

class TestCoord3D(unittest.TestCase):
        def setUp(self):
                self.coo1 = Coord3D(3.0, 4.0, 5.0)
                self.coo2 = Coord3D(1.0, 2.0, 7.5)

        def testPlusOperator(self):
                coo3 = self.coo1 + self.coo2
                self.assertEqual(coo3.x, 4.0)
                self.assertEqual(coo3.y, 6.0)
                self.assertEqual(coo3.z, 12.5)
        def testMinusOperator(self):
                coo3 = self.coo1 - self.coo2
                self.assertEqual(coo3.x, 2.0)
                self.assertEqual(coo3.y, 2.0)
                self.assertEqual(coo3.z, -2.5)
        def testPlusEqualOperator(self):
                coo3 = Coord3D(self.coo1)
                coo3 += self.coo2
                self.assertEqual(coo3.x, 4.0)
                self.assertEqual(coo3.y, 6.0)
                self.assertEqual(coo3.z, 12.5)

class TestAtom(unittest.TestCase):
    def setUp(self):
        co = Coord3D(1,2,3)
        atom = Atom(Atomproperty(), co)
        atom.atomType = 'CA'
        #atom.atomElement = 'C'
        atom.residType = 'LEU'
        atom.residId = 6
        atom.atomId = 123
        atom.atomCharge = -1.23456
        self.atom = atom
        
        
    def testProperties(self):
        atom = self.atom
        self.assertEqual(atom.atomType, 'CA')
        #self.assertEqual(atom.atomElement, 'C')
        self.assertEqual(atom.residType, "LEU")
        self.assertEqual(atom.residId, 6)
        self.assertEqual(atom.atomId, 123)
        self.assertEqual(atom.atomCharge, -1.23456)
        

class TestAtomSelection(unittest.TestCase):
    def setUp(self):
        self.rig = Rigidbody("1F88.pdb")
        
    def testSelectAll(self):
        allAtoms = self.rig.SelectAllAtoms()
        self.assertEqual(len(allAtoms), 5067)
    
    def testSelectCA(self):
        CAatoms = self.rig.CA()
        self.assertEqual(len(CAatoms),643)
        
    def testSelectAtomType(self):
        CAatoms = self.rig.SelectAtomType("CA")
        self.assertEqual(len(CAatoms), 643)
        
    def testSelectBackbone(self):
        bbAtoms = self.rig.Backbone()
        self.assertEqual(len(bbAtoms), 2572)
        
    def testSelectResRange(self):
        res_1_35 = self.rig.SelectResRange(1,35)
        self.assertEqual(len(res_1_35), 566)  # two chains
    
    def testAnd(self):
        res_1_35 = self.rig.SelectResRange(1,35) 
        CAatoms = self.rig.SelectAtomType("CA")
        
        ca_1_35 = res_1_35 & CAatoms
        self.assertEqual(len(ca_1_35), 70)  #2*35: two chains, A and B
        
    def testSelectResidType(self):
        met1 = self.rig.SelectResidType("MET") & self.rig.SelectResRange(1,5) 
        self.assertEqual(len(met1), 16)
        met1A = self.rig.SelectResidType("MET") & self.rig.SelectResRange(1,5) & self.rig.SelectChainId("A")
        self.assertEqual(len(met1A), 8)
        
    def testSelectChainId(self):
        chainA = self.rig.SelectChainId("A")
        self.assertEqual(len(chainA), 2638)
        

class TestRigidbody(unittest.TestCase):
    def setUp(self):
        self.r = Rigidbody("1FIN_r.pdb")
        self.r2 = Rigidbody()
        at = Atom()
        at.coords = Coord3D(1,0,0)
        self.r2.AddAtom(at)
        at.coords = Coord3D(0,1,0)
        self.r2.AddAtom(at)
        at.coords = Coord3D(0,0,1)
        self.r2.AddAtom(at)
        
        
    def testCopy(self):
        s = Rigidbody(self.r)
        self.assertEqual(len(s), len(self.r))
        self.assertEqual(self.r.FindCenter(), s.FindCenter())
    def testSize(self):
        self.assertEqual(len(self.r), 2365)
    def testCopyAtom(self):
        atom = self.r.CopyAtom(3)
        coords = atom.coords
        self.assertAlmostEqual(coords.x, -16.159)
        self.assertAlmostEqual(coords.y, 189.782 )
        self.assertAlmostEqual(coords.z, 106.402)
        self.assertEqual(atom.atomId, 4)
        self.assertEqual(atom.chainId, '')
        
    def testGetCoords(self):
        coords = self.r.getCoords(3)
        self.assertAlmostEqual(coords.x, -16.159)
        self.assertAlmostEqual(coords.y, 189.782 )
        self.assertAlmostEqual(coords.z, 106.402)
        
    def testTranslate(self):
        tr = Coord3D(3.2, 2.98, 14.22)
        s = Rigidbody(self.r)
        s.Translate(tr)
        coords = s.getCoords(3)
        self.assertAlmostEqual(coords.x, -16.159 + 3.2)
        self.assertAlmostEqual(coords.y, 189.782 + 2.98 )
        self.assertAlmostEqual(coords.z, 106.402 + 14.22)
     
    def testFindCenter(self):
        cen = self.r.FindCenter()
        self.assertTrue(abs(cen.x +20.171249) < 1e-6) 
        self.assertTrue(abs(cen.y - 215.498060) < 1e-6)
        self.assertTrue(abs(cen.z - 119.427781) < 1e-6)
        
        
    def testSetAtom(self):
        atom = self.r.CopyAtom(3)
        atom.coords = Coord3D(3,4,5)
        self.r.SetAtom(3,atom)
        #test to see if the mofification worked:
        atom2 = self.r.CopyAtom(3)
        self.assertTrue( norm2(atom2.coords - Coord3D(3,4,5) ) < 1e6 )
        coords2 = atom2.coords
        self.assertAlmostEqual(coords2.x, 3)
        self.assertAlmostEqual(coords2.y, 4)
        self.assertAlmostEqual(coords2.z, 5)


    def testUnsafeGetCoords(self):
        """in principle GetCoords(i,co) and unsafeGetCoords(i,co) should
        lead to the exact same coordinates if a sync has been done before
        calling the 'unsafe' version"""
        r2 = Rigidbody("1FIN_r.pdb")
        A = Coord3D(4.23, 5.72, 99.02)
        B = Coord3D(1.23, 6.33, 1.234)
        self.r.ABrotate(A,B, 2.2345)
        r2.ABrotate(A,B, 2.2345)
        self.r.Translate(Coord3D(34.23, 123.45,11.972))
        r2.Translate(Coord3D(34.23, 123.45,11.972))

        r2.syncCoords()
        #same rotation and translation for r and r2: should have exact same coordinates
        for i in range(len(self.r)):
            co1 = Coord3D()
            co2 = Coord3D()
            co1 = self.r.getCoords(i)
            r2.unsafeGetCoords(i,co2)
            self.assertEqual(co1,co2)

            
    def testAddAtoms(self):
         r = Rigidbody()
         self.assertEqual(len(r), 0)
         at = Atom()
         at.coords = Coord3D(2,3,4)
         r.AddAtom(at)
         self.assertAlmostEqual(len(r),1)
         
    def testGetAtomProperty(self):
        atprop = self.r.GetAtomProperty(8)
        self.assertEqual(atprop.residType, 'GLU')
        self.assertEqual(atprop.residId, 2)
        self.assertEqual(atprop.atomId, 9)
        
            

class TestBasicMoves(unittest.TestCase):
    def setUp(self):
        self.rigid1=Rigidbody("1FIN_r.pdb")
        self.rigid2=Rigidbody(self.rigid1)
        self.rigid3=Rigidbody(self.rigid2)

    def testBasicRmsd(self):
        rigtmp=Rigidbody(self.rigid1)
        self.assertEqual(Rmsd(self.rigid1, self.rigid1), 0.0)
        self.rigid1.Translate(Coord3D(4,0,0))
        self.assertEqual(Rmsd(rigtmp, self.rigid1), 4)
        
    def testErrorsRmsd(self):
	rigid1 = Rigidbody()
	rigid2 = Rigidbody()
	#Rmsd(rigid1, rigid2)
	self.assertRaises(ValueError, Rmsd, rigid1, rigid2)

    def testTranslation1(self):
        CoM1 = self.rigid1.FindCenter()
        self.rigid1.Translate(Coord3D(3.0, -55.67, 1))
        CoM2 = self.rigid1.FindCenter()
        diff=CoM2-CoM1
        self.assertTrue( norm2(diff + Coord3D(-3.0, 55.67, -1.0)) < 1e-6)
        self.rigid1.Translate(Coord3D(-3.0, 55.67, -1.0))   # translate back
        self.assertTrue(Rmsd(self.rigid1, self.rigid2) < 1e-6)

    def testTranslation2(self):
        vec1 = Coord3D (-123.54, 45.62, -99.003)
        vec2 = Coord3D (36.3125, 2.78, -36.378)
        self.rigid2.Translate(vec1+vec2)
        self.rigid2.Translate(vec1-vec2)
        self.rigid2.Translate(Coord3D() - 2*vec1)  #should be a global null translation + round error
        self.assertTrue(Rmsd(self.rigid2, self.rigid3) < 1e-6)

class TestRotations(unittest.TestCase):
    def setUp(self):
       
        at1 = Atom(Atomproperty(),Coord3D(1,0,0))
        at2 = Atom(Atomproperty(),Coord3D(0,1,0))
        at3 = Atom(Atomproperty(),Coord3D(0,0,1))
        at4 = Atom(Atomproperty(),Coord3D(1,1,1))
        
        rig = Rigidbody()
        rig.AddAtom(at1)
        rig.AddAtom(at2)
        rig.AddAtom(at3)
        rig.AddAtom(at4)
        
        self.rig = rig
        
    def testRotZ(self):
        
        
        self.rig.ABrotate(Coord3D(0,0,0), Coord3D(0,0,1), math.pi/2)
        # i should now be j
        co1 = self.rig.CopyAtom(0).coords
        self.assertAlmostEqual(co1.x, 0)
        self.assertAlmostEqual(co1.z, 0)
        self.assertAlmostEqual(co1.y, 1)
        
        # j becomes -i
        co2 = self.rig.CopyAtom(1).coords
        self.assertAlmostEqual(co2.x, -1)
        self.assertAlmostEqual(co2.y, 0)
        self.assertAlmostEqual(co2.z, 0)

        #k is still k:
        co3 = self.rig.CopyAtom(2).coords
        self.assertAlmostEqual(co3.x, 0)
        self.assertAlmostEqual(co3.y, 0)
        self.assertAlmostEqual(co3.z, 1)
        
    def testRotX(self):
        self.rig.ABrotate(Coord3D(0,0,0), Coord3D(1,0,0), math.pi/2)
        
        #i is still i
        co1 = self.rig.CopyAtom(0).coords
        self.assertAlmostEqual(co1.x, 1)
        self.assertAlmostEqual(co1.z, 0)
        self.assertAlmostEqual(co1.y, 0)
        
        # j becomes k
        co2 = self.rig.CopyAtom(1).coords
        self.assertAlmostEqual(co2.x, 0)
        self.assertAlmostEqual(co2.y, 0)
        self.assertAlmostEqual(co2.z, 1)

        #k becomes -j
        co3 = self.rig.CopyAtom(2).coords
        self.assertAlmostEqual(co3.x, 0)
        self.assertAlmostEqual(co3.y, -1)
        self.assertAlmostEqual(co3.z, 0)
        

    def testRotY(self):
        self.rig.ABrotate(Coord3D(0,0,0), Coord3D(0,1,0), math.pi/2)
        
        #i becomes -j
        co1 = self.rig.CopyAtom(0).coords
        self.assertAlmostEqual(co1.x, 0)
        self.assertAlmostEqual(co1.z, -1)
        self.assertAlmostEqual(co1.y, 0)
        
        # j is still j
        co2 = self.rig.CopyAtom(1).coords
        self.assertAlmostEqual(co2.x, 0)
        self.assertAlmostEqual(co2.y, 1)
        self.assertAlmostEqual(co2.z, 0)

        #k becomes i
        co3 = self.rig.CopyAtom(2).coords
        self.assertAlmostEqual(co3.x, 1)
        self.assertAlmostEqual(co3.y, 0)
        self.assertAlmostEqual(co3.z, 0)
        
        
    def testRotZ_trans(self):
        
        self.rig.ABrotate(Coord3D(1,1,1), Coord3D(1,1,3), math.pi/2)
        
        
        co1 = self.rig.CopyAtom(0).coords
        self.assertAlmostEqual(co1.x, 2)
        self.assertAlmostEqual(co1.z, 0)
        self.assertAlmostEqual(co1.y, 1)
        
        
        co2 = self.rig.CopyAtom(1).coords
        self.assertAlmostEqual(co2.x, 1)
        self.assertAlmostEqual(co2.y, 0)
        self.assertAlmostEqual(co2.z, 0)

        co3 = self.rig.CopyAtom(2).coords
        self.assertAlmostEqual(co3.x, 2)
        self.assertAlmostEqual(co3.y, 0)
        self.assertAlmostEqual(co3.z, 1)

        co4 = self.rig.CopyAtom(3).coords
        self.assertAlmostEqual(co4.x, 1)
        self.assertAlmostEqual(co4.y, 1)
        self.assertAlmostEqual(co4.z, 1)
        
        
        

class TestCoordsArray(unittest.TestCase):
    def setUp(self):
        c = CoordsArray()
        coo1 = Coord3D(3.0, 4.0, 5.0)
        coo2 = Coord3D(1.0, 2.0, 7.5)
        
        c.AddCoord(coo1)
        c.AddCoord(coo2)
        self.c = c
        self.tr = Coord3D(3.0, 4.5, -3.0)
        
    def testSize(self):
        self.assertTrue(len(self.c) == 2)
        
    def testGetAtom(self):
        c1 = Coord3D()
        self.c.GetCoords(0, c1)
        self.assertTrue(  norm2(c1 - Coord3D(3.0, 4.0, 5.0))<1e-6 )

    def testBasicTranslation(self):
        self.c.Translate(self.tr)
        c1 = Coord3D()
        c2 = Coord3D()
        self.c.GetCoords(0, c1 )
        self.c.GetCoords(1, c2 )
        self.assertTrue(c1 == Coord3D(6.0, 8.5, 2.0))

    def testSetCoords(self):
        """brief explanation:
        For lazy evaluation, corrdinates are stored unrotated/untranslated along
        with the rotation/translation 4x4 matrix. When user set the coordinates,
        this means: 'change the current coordinates of atom i' and not 'change
        the initial coordinates of atom i' so here we check that this is the case"""
        self.c.Translate(self.tr); #do some translation
        self.c.AttractEulerRotate(2.0,4.0,5.0) # do a rotation
        co = Coord3D(3,2,1) #new coordinates to be added
        self.c.SetCoords(0,co)
        co2=Coord3D()
        self.c.GetCoords(0,co2) #get the coordinates back
        self.assertTrue(norm2(co-co2)<1.0e-6)




class TestSuperposition(unittest.TestCase):
    def setUp(self):
        self.prot1 = Rigidbody("1FIN_r.pdb")
        random.seed(123)
    def testTransRot(self):
        prot2 = Rigidbody(self.prot1)


        for i in xrange(20):
            #random translation coordinates:
            x = (random.random()-0.5)*50.0
            y = (random.random()-0.5)*50.0
            z = (random.random()-0.5)*50.0
            prot2.Translate(Coord3D(x,y,z))
            a = (random.random()-0.5)*50.0
            b = (random.random()-0.5)*50.0
            c = (random.random()-0.5)*50.0
            prot2.AttractEulerRotate(a,b,c)

            sup = superpose(self.prot1,prot2) # superpose(reference, mobile)
            matrix = sup.matrix
            prot2.ApplyMatrix(matrix)
            self.assertTrue(Rmsd(prot2,self.prot1)<1e-6)

class TestForceFields(unittest.TestCase):
    """ test if calculated energies are stable through library versions """
    def testFF2k(self):
        a = AttractRigidbody("pk6a.red")
        c = AttractRigidbody("pk6c.red")
        #a = AttractRigidbody(a)
        #c = AttractRigidbody(c)
        
        #print "*********** sizes:", a.Size(), '/', c.Size()
        a.setRotation(False)
        a.setTranslation(False)
        FF = AttractForceField2("mbest1k.par", 20.0)
        FF.AddLigand(a)
        FF.AddLigand(c)
        x = []
        for i in range(6):
            x.append(0.0)
        self.assertAlmostEqual(FF.Function(x),-32.9487770656) #energy from ptools 0.3
	self.assertAlmostEqual(FF.Function(x), FF.getVdw() + FF.getCoulomb())

         



unittest.main()
