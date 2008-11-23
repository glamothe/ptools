#include <coord3d.h>
#include <ptools.h>


#include <cxxtest/TestSuite.h>

using namespace PTools;


class TestCoord3D : public CxxTest::TestSuite
{
Coord3D coo1, coo2, coo3;


public:
        void setUp() {
                this->coo1 = Coord3D(3.0, 4.0, 5.0);
                coo2 = Coord3D(1.0, 2.0, 7.5);
        }

        void testPlusOperator()
        {
                coo3 = coo1 + coo2 ;
                TS_ASSERT_EQUALS(coo3.x, 4.0);
                TS_ASSERT_EQUALS(coo3.y, 6.0);
                TS_ASSERT_EQUALS(coo3.z, 12.5);

        }


        void testMinusOperator(){
                coo3 = coo1 - coo2;
                TS_ASSERT_EQUALS(coo3.x, 2.0);
                TS_ASSERT_EQUALS(coo3.y, 2.0);
                TS_ASSERT_EQUALS(coo3.z, -2.5);
        }

        void testPlusEqualOperator()
        {
                coo3 = Coord3D(coo1);
                coo3 += coo2;
                TS_ASSERT_EQUALS(coo3.x, 4.0);
                TS_ASSERT_EQUALS(coo3.y, 6.0);
                TS_ASSERT_EQUALS(coo3.z, 12.5);
        }


};


class TestRigidbody: public CxxTest::TestSuite
{
public:

Rigidbody r,s, r2;
Coord3D A, B;



    void setUp()
    {
        r =  Rigidbody("1FIN_r.pdb");
    }


    void testCopy()
    {
        s = Rigidbody(r);
        TS_ASSERT_EQUALS(s.Size(), r.Size())
        TS_ASSERT_EQUALS(r.FindCenter(), s.FindCenter());
    }


    void testSize()
    {        TS_ASSERT_EQUALS(r.Size(), 2365);
    }

    void testSetAtom()
    {
        Atom atom (r.CopyAtom(3) ) ;
        atom.SetCoords(Coord3D(3,4,5));
        r.SetAtom(3,atom);
        //#test to see if the mofification worked:
        Atom atom2 = r.CopyAtom(3);
        TS_ASSERT( Norm2(atom2.GetCoords() - Coord3D(3,4,5) ) < 1e6 );

    }


    void testUnsafeGetCoords()
    {
//         """in principle GetCoords(i,co) and unsafeGetCoords(i,co) should
//         lead to the exact same coordinates if a sync has been done before
//         calling the 'unsafe' version"""
        r2 = Rigidbody("1FIN_r.pdb");
        A = Coord3D(4.23, 5.72, 99.02);
        B = Coord3D(1.23, 6.33, 1.234);
        r.ABrotate(A,B, 2.2345);
        r2.ABrotate(A,B, 2.2345);
        r.Translate(Coord3D(34.23, 123.45,11.972));
        r2.Translate(Coord3D(34.23, 123.45,11.972));

        r2.syncCoords();
//         #same rotation and translation for r and r2: should have exact same coordinates

        for (int i=0; i<r.Size(); i++)
        {
            Coord3D co1 = Coord3D();
            Coord3D co2 = Coord3D();
            co1 = r.GetCoords(i);
            r2.unsafeGetCoords(i,co2);
            TS_ASSERT_EQUALS(co1,co2);
        }
    }

};


