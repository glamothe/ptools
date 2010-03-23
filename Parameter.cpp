


#include <geometry.h>
#include <rigidbody.h>
#include <iostream>
#include <Movement.h>
#include <BasePair.h>
#include <Parameter.h>
using namespace std;


namespace PTools {

Parameter::Parameter()
{
    shift = 0.0;
    slide = 0.0;
    rise = 0.0;
    tilt = 0.0;
    roll = 0.0;
    twist = 0.0;

}


Parameter::Parameter(const BasePair& bp1,const BasePair& bp2)
{
    cout<<"vouvou"<<endl;

}


Parameter::~Parameter()
{

}


////code "steal" from deformDna.cpp (author: Pierre Poulain)
//std::vector<double> DNA::MeasureParameters(Rigidbody& oxyz1, Rigidbody& oxyz2) const{
//	Coord3D X = oxyz1.GetCoords(1) - oxyz1.GetCoords(0);
//	Coord3D Y = oxyz1.GetCoords(2) - oxyz1.GetCoords(0);
//	Coord3D Z = oxyz1.GetCoords(3) - oxyz1.GetCoords(0);
//
//	Coord3D I = oxyz2.GetCoords(1) - oxyz2.GetCoords(0);
//	Coord3D J = oxyz2.GetCoords(2) - oxyz2.GetCoords(0);
//	Coord3D K = oxyz2.GetCoords(3) - oxyz2.GetCoords(0);
//
//	Coord3D e1(ScalProd(I, X), ScalProd(I, Y), ScalProd(I, Z));
//	Coord3D e2(ScalProd(J, X), ScalProd(J, Y), ScalProd(J, Z));
//	Coord3D e3(ScalProd(K, X), ScalProd(K, Y), ScalProd(K, Z));
//
//
//	double beta = asin(e3.x);
//	double alpha = acos(e3.z/cos(beta));
//	double gamma = acos( e1.x / cos(beta) );
//
//	double tilt = alpha;
//	double roll = beta;
//	double twist = gamma;
//
//	Coord3D OO = oxyz2.GetCoords(0) - oxyz1.GetCoords(0);
//	double x_delta = ScalProd(OO, X);
//	double y_delta = ScalProd(OO, Y);
//	double z_delta = ScalProd(OO, Z);
//
//	std::vector<double> param(6);
//	param[0] = x_delta;
//	param[1] = y_delta;
//	param[2] = z_delta;
//	param[3] = tilt;
//	param[4] = roll;
//	param[5] = twist;
//return param;
//}
}