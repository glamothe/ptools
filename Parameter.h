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

}//end namespace


#endif	/* _PARAMETER_H */

