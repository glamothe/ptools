from ptools import *
import random
import unittest


class TestSuperpose(unittest.TestCase):
        def setUp(self):
            self.r1 = Rigidbody("1FIN_r.pdb")
            self.r2 = Rigidbody("1FIN_r.pdb")


        def testTranslation_sippl(self):
            x = (random.random()-0.5)*50.0
            y = (random.random()-0.5)*50.0
            z = (random.random()-0.5)*50.0
            x=20
            y=30
            z=50


            self.r2.ABrotate(Coord3D(), Coord3D(0,0,1), 3.1415926/2.0)

            self.r2.Translate(Coord3D(x,y,z))
            
            v = superpose_sippl(self.r1,self.r2)
            v.Print()
            s = superpose(self.r1,self.r2)
            s.matrix.Print()

        def testsippl_et_quatern(self):
            random.seed(123)
            x = (random.random()-0.5)*50.0
            y = (random.random()-0.5)*50.0
            z = (random.random()-0.5)*50.0
            p1=Coord3D(x,y,z)

            x = (random.random()-0.5)*50.0
            y = (random.random()-0.5)*50.0
            z = (random.random()-0.5)*50.0
            p2=Coord3D(x,y,z)

            x = (random.random()-0.5)*50.0
            y = (random.random()-0.5)*50.0
            z = (random.random()-0.5)*50.0
            trans=Coord3D(x,y,z)

            angle = (random.random()-0.5)*50.0 

            self.r2.Translate(trans)
            self.r2.ABrotate(p1,p2,angle)
            self.r3=Rigidbody(self.r2)

            v = superpose_sippl(self.r1,self.r2)
            v.Print()
            s = superpose(self.r1, self.r3)
            s.matrix.Print()
            
        def testnonrigidtransformation(self):
            random.seed(123)
            r = Rigidbody()
            r2 = Rigidbody()
            
            for i in range(300):
                ap = Atomproperty()
                x = (random.random()-0.5)*50.0
                y = (random.random()-0.5)*50.0
                z = (random.random()-0.5)*50.0
                p1=Coord3D(x,y,z)

                atom=Atom(ap,p1)
                r.AddAtom(atom)

            for i in range(300):
                ap = Atomproperty()
                x = (random.random()-0.5)*50.0
                y = (random.random()-0.5)*50.0
                z = (random.random()-0.5)*50.0
                p1=Coord3D(x,y,z)

                atom=Atom(ap,p1)
                r2.AddAtom(atom)


            v = superpose_sippl(r,r2)
            v.Print()
            s = superpose(r, r2)
            s.matrix.Print()
            
            
            
            
            
            
            
            
            
            
            

unittest.main()
