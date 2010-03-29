/* 
 * File:   Parameter.h
 * Author: boyer
 *
 * Created on March 22, 2010, 11:55 PM
 */

#ifndef _PARAMETER_H
#define	_PARAMETER_H

/*! \brief
* a class to compute and hold the parameter between two base pairs
*/
namespace PTools
{

  class Parameter
  {
    public:
        Parameter();
        Parameter(const BasePair& bp1, const BasePair& bp2);
        ~Parameter();
        Rigidbody buildAxisGeometricCenter(const Rigidbody& bp)const;
        void MeasureParameters(const Rigidbody& oxyz1, const Rigidbody& oxyz2);

        double getRise() const ;
        void setRise(double rise) ;

        double getRoll() const ;
        void setRoll(double roll);

        double getShift() const ;
        void setShift(double shift);

        double getSlide() const ;
        void setSlide(double slide);

        double getTilt() const ;
        void setTilt(double tilt);

        double getTwist() const ;
        void setTwist(double twist) ;

        


    private:
    //atribut
        double shift;
	double slide;
	double rise;
	double tilt;
	double roll;
	double twist;
    //method
        
  };
  
double dotProduct( const Coord3D& u, const Coord3D& v);
}//end namespace

#endif	/* _PARAMETER_H */

