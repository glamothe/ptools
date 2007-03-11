#ifndef FORCEFIELD_H
#define FORCEFIELD_H



#include "rigidbody.h"

namespace PTools
{


//! Generic forcefield (abstract class)
/**

 */
class ForceField
{
public:


    virtual double Function()=0; //simply return the function value for
    virtual double Function(const double*)=0;
    virtual void Derivatives(const double*, double*)=0; ///< analytical derivative
    virtual void NumDerivatives(const double*, double*){}; ///< numerical derivative for testing purpose

    virtual ~ForceField(){};

} ;


class AttractForceField: public ForceField
{

public:

    AttractForceField(const Rigidbody& recept,const Rigidbody& lig);
    virtual ~AttractForceField(){};


    //this is to satisfy the abstract class:
    virtual double Function() {return Energy();};
    virtual double Function(const double* stateVars) {return Energy(stateVars);};
    virtual void Derivatives(const double* stateVars, double* delta){return Gradient(stateVars, delta);};
    virtual void NumDerivatives(const double* stateVars, double* delta);



    double Energy(); ///< return current energy without moving any object
    double Energy(const double* stateVars); // stateVars: tx ty tz theta phi psi

    double Electrostatic();
    double LennardJones();
    void Gradient(const double* stateVars, double* delta);

    void ShowEnergyParams(); ///< for debug purposes...
    void Trans() {double delta[6] ; Trans(delta,true); };
    



private:
    //private methods:
    void InitParams();

    //private data
    Rigidbody m_refreceptor, m_refligand;
    Rigidbody m_receptor, m_ligand;
    Rigidbody m_savligand;

    std::vector<long double> m_rad ; //rad parameter ... RADius?
    std::vector<long double> m_amp ; //amp parameter ... AMPlitude??

    std::vector<Coord3D> m_ligforces ; //forces

    Coord3D m_ligcenter; //center of mass of the ligand


    std::vector<uint> m_rAtomCat; //receptor atom category (as seen in reduced pdb files)
    std::vector<uint> m_lAtomCat; //ligand atom category

    std::vector<double> m_rAtomCharge;
    std::vector<double> m_lAtomCharge;

    double m_rc[29][29]; //some pre-calculated results
    double m_ac[29][29]; //some pre-calculated results

    void Trans(double* delta, bool print=false); // translational derivatives
    void Rota(double phi,double ssi, double rot, double* delta);
    void ResetForces();

    bool m_energycalled ;

} ;


class TestForceField: public ForceField
{

virtual double Function() {return 0.0;}; 

virtual double Function(const double* X)
   {
        return X[0]*X[0] + (X[1]-2)*(X[1]-2) ;
   }

   virtual void Derivatives(const double* X, double* grad)
   {
        grad[0]=2*X[0];
        grad[1]=2*(X[1]-2);
   }


};



} //namespace PTools





#endif //#ifndef FORCEFIELD_H

