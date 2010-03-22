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
        Parameter(const BasePair& bp1,const BasePair& bp2);
        ~Parameter();


    private:
    //atribut
        double x_delta = 0.0 ;
	double y_delta = 0.0 ;
	double z_delta = 0.0 ;
	double tilt = 0.0 ;
	double roll = 0.0 ;
	double twist = 0.0 ;
    //method

  };

}//end namespace


#endif	/* _PARAMETER_H */

