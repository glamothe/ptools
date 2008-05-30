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
        self.assertEqual(s.Size(), self.r.Size())
        self.assertEqual(self.r.FindCenter(), s.FindCenter())
    def testSize(self):
        self.assertEqual(self.r.Size(), 2365)


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

    def testTranslation1(self):
        CoM1 = self.rigid1.FindCenter()
        self.rigid1.Translate(Coord3D(3.0, -55.67, 1))
        CoM2 = self.rigid1.FindCenter()
        diff=CoM2-CoM1
        self.assertTrue( Norm2(diff + Coord3D(-3.0, 55.67, -1.0)) < 1e-6)
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
        self.assertTrue(  Norm2(c1 - Coord3D(3.0, 4.0, 5.0))<1e-6 )

    def testBasicTranslation(self):
        
        self.c.Translate(self.tr)
        c1 = Coord3D()
        c2 = Coord3D()
        self.c.GetCoords(0, c1 )
        self.c.GetCoords(1, c2 )
        self.assertTrue(c1 == Coord3D(6.0, 8.5, 2.0))
        

unittest.main()