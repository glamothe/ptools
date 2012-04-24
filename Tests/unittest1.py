# -*- coding: utf-8 -*-
from ptools import *
import random
import unittest


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

class TestRigidbody(unittest.TestCase):
    def setUp(self):
        self.r = Rigidbody("1FIN_r.pdb")
    def testCopy(self):
        s = Rigidbody(self.r)
        self.assertEqual(len(s), len(self.r))
        self.assertEqual(self.r.FindCenter(), s.FindCenter())
    def testSize(self):
        self.assertEqual(len(self.r), 2365)
    def testCopyAtom(self):
        atom = self.r.CopyAtom(3)
        coords = atom.GetCoords()
        self.assertAlmostEqual(coords.x, -16.159)
        self.assertAlmostEqual(coords.y, 189.782 )
        self.assertAlmostEqual(coords.z, 106.402)
        self.assertEqual(atom.GetAtomId(), 4)
        self.assertEquel(atom.GetChainId(), '')
        
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
        self.assertAlmostEqual(cen.x, -20.171249, delta=1e-6) 
        self.assertAlmostEqual(cen.y, 215.498060, delta=1e-6)
        self.assertAlmostEqual(cen.z, 119.427781, delta=1e-6)
        
        
    def testSetAtom(self):
        atom = self.r.CopyAtom(3)
        atom.SetCoords(Coord3D(3,4,5))
        self.r.SetAtom(3,atom)
        #test to see if the mofification worked:
        atom2 = self.r.CopyAtom(3)
        self.assertTrue( norm2(atom2.GetCoords() - Coord3D(3,4,5) ) < 1e6 )


    def testUnsafeGetCoords(self):
        """in principle GetCoords(i,co) and unsafeGetCoords(i,co) should
        lead to the exact same coordinates if a sync has been done before
        calling the 'unsafe' version"""
        self.r2 = Rigidbody("1FIN_r.pdb")
        A = Coord3D(4.23, 5.72, 99.02)
        B = Coord3D(1.23, 6.33, 1.234)
        self.r.ABrotate(A,B, 2.2345)
        self.r2.ABrotate(A,B, 2.2345)
        self.r.Translate(Coord3D(34.23, 123.45,11.972))
        self.r2.Translate(Coord3D(34.23, 123.45,11.972))

        self.r2.syncCoords()
        #same rotation and translation for r and r2: should have exact same coordinates
        for i in range(len(self.r)):
            co1 = Coord3D()
            co2 = Coord3D()
            co1 = self.r.getCoords(i)
            self.r2.unsafeGetCoords(i,co2)
            self.assertEqual(co1,co2)

            
    def testAddAtoms(self):
         r = Rigidbody()
         self.assertEqual(len(r), 0)
         at = Atom()
         at.SetCoords(Coord3D(2,3,4))
         r.AddAtom(at)
         self.assertAlmostEqual(len(r),1)
            

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
        self.assertTrue(self.c.Size() == 2)
        
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
        a = Rigidbody("pk6a.red")
        c = Rigidbody("pk6c.red")
        a = AttractRigidbody(a)
        c = AttractRigidbody(c)
        a.setRotation(False)
        a.setTranslation(False)
        FF = AttractForceField2("mbest1k.par", 20.0)
        FF.AddLigand(a)
        FF.AddLigand(c)
        x = Vdouble()
        for i in range(6):
            x.append(0)
        self.assertTrue( (FF.Function(x)+32.9487770656) < 1e-6) #energy from ptools 0.3
	self.assertEqual(FF.Function(x), FF.getVdw() + FF.getCoulomb())

         



unittest.main()
