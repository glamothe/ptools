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
        self.assertTrue(Norm2(co-co2)<1.0e-6)



class TestSuperposition(unittest.TestCase):
    def setUp(self):
        self.prot1 = Rigidbody("1FIN_r.pdb")
        random.seed(123)
    def testTranslation(self):
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

    def testFundamentalBugInAtomSelection(self):
        #first populate two Rigidbody:
        l = list()
        for i in range(8):
            at = self.prot1.CopyAtom(i)
            l.append(at)
        p1 = Rigidbody()
        p2 = Rigidbody()
        
        for i in l:
            p1.AddAtom(i)
        l.reverse()
        for i in l:
            p2.AddAtom(i)
        self.assertTrue(p1.Size()==p2.Size())
        
        #now create two AtomSelection objects to superpose p1 and p2:
        atsel1 = AtomSelection()
        atsel1.SetRigid(p1)
        atsel2 = AtomSelection()
        atsel2.SetRigid(p2)
        
        
        asel1 = atsel1 | p1.SelectAtomType("N")
        asel1 = atsel1 | p1.SelectAtomType("CA")
        asel1 = atsel1 | p1.SelectAtomType("C")
        asel1 = atsel1 | p1.SelectAtomType("O")
        
        
        atsel2 = atsel2 | p2.SelectAtomType("N")
        atsel2 = atsel2 | p2.SelectAtomType("CA")
        atsel2 = atsel2 | p2.SelectAtomType("C")
        atsel2 = atsel2 | p2.SelectAtomType("O")
        
        sup = superpose(atsel1, atsel2)



unittest.main()
