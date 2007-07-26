#ifndef FORCEFIELD_H
#define FORCEFIELD_H


#include "basetypes.h"
#include "rigidbody.h"
#include "pairlist.h"

namespace PTools
{


void PrintVec(const Vdouble& vec);


//! Generic forcefield (abstract class)
/**

 */
class ForceField
{
public:


    //virtual double Function()=0; //simply return the function value for
    virtual double Function(const Vdouble&)=0;
    virtual void Derivatives(const Vdouble&, Vdouble&)=0; ///< analytical derivative
    virtual void NumDerivatives(const Vdouble& StateVars, Vdouble& delta); ///< numerical derivative for testing purpose
    virtual uint ProblemSize()=0;

    virtual ~ForceField(){};

} ;


class AttractForceField: public ForceField
{

public:

    AttractForceField(const Rigidbody& recept,const Rigidbody& lig,double cutoff);
    virtual ~AttractForceField(){};


    //this is to satisfy the abstract class:
    //virtual double Function() {return Energy();};
    virtual double Function(const Vdouble& stateVars) {
        //std::cout << "Function: stateVars size: " << stateVars.size() << std::endl;
        return Energy(stateVars);
    };
    virtual void Derivatives(const Vdouble& stateVars, Vdouble& delta){
        //std::cerr << "Derivatives: stateVars.size()"<<stateVars.size() << "\n" ;
        //std::cerr << "Type de delta:" << (std::string) typeid(delta).name() << std::endl ;
        return Gradient(stateVars, delta);
    };
    //virtual void NumDerivatives(const Vdouble& StateVars, Vdouble& delta);
    virtual uint ProblemSize() {
        return 6;
    };


    double Energy(); ///< return current energy without moving any object
    double Energy(const Vdouble& stateVars); // stateVars: tx ty tz theta phi psi

    double Electrostatic();
    double LennardJones();

    void Gradient(const Vdouble& stateVars, Vdouble& delta);

    void ShowEnergyParams(); ///< for debug purposes...
    void Trans() {
        Vdouble delta(6) ;
        Trans(delta,true);
    };
    void SetRestraint(double rstk) {
        m_rstk = rstk;
    };






private:
    //private methods:
    void InitParams();

    void Trans(Vdouble& delta, bool print=false); // translational derivatives
    void Rota(double phi,double ssi, double rot, Vdouble& delta, bool print=false);
    void ResetForces();

    double Constraints(); ///< distance constraints between ligand and receptor

    //private data
    Rigidbody m_refreceptor, m_refligand;
    Rigidbody m_receptor, m_ligand;
    Rigidbody m_savligand;

    Vdouble m_rad ; //rad parameter ... RADius?
    Vdouble m_amp ; //amp parameter ... AMPlitude??

    std::vector<Coord3D> m_ligforces ; //forces

    Coord3D m_ligcenter; //center of mass of the ligand
    Coord3D m_reccenter; //center of mass of the receptor (used for the distance constraint)

    Vuint m_rAtomCat; //receptor atom category (as seen in reduced pdb files)
    Vuint m_lAtomCat; //ligand atom category

    int m_ligRestraintIndex; //index of the ligand atom on which the retraint force should be applied

    Vdouble m_rAtomCharge;
    Vdouble m_lAtomCharge;

    double m_rc[40][40]; //some pre-calculated results
    double m_ac[40][40]; //some pre-calculated results

    bool m_energycalled ;

    PairList plist;
    std::string _filesuffix;

    static int _minimnb;
    double m_rstk;
} ;




class TestForceField: public ForceField
{

//virtual double Function() {return 0.0;};

    virtual uint ProblemSize(){
        return 2;
    };

    virtual double Function(const Vdouble& X)
    {
        PrintVec(X);
        return X[0]*X[0] + (X[1]-2)*(X[1]-2) ;
    }

    virtual void Derivatives(const Vdouble& X, Vdouble& grad)
    {
        PrintVec(X);
        PrintVec(grad);
        grad[0]=2*X[0];
        grad[1]=2*(X[1]-2);
    }


};



/*!  \brief Attract forcefield version 2
*
*   new forcefield with saddle point and pairwise energy
*/
class AttractForceField2 : public ForceField
{

public:

    AttractForceField2(std::string paramsFileName, AttractRigidbody& rec, AttractRigidbody& lig, double cutoff);
    double Function(const Vdouble&);
    void Derivatives(const Vdouble&, Vdouble&){}; ///< analytical derivative
    uint ProblemSize(){return 0;};
    double nonbon8(AttractRigidbody& rec, AttractRigidbody& lig);


private:

    //private variables

	AttractRigidbody m_ligand, m_receptor;

    Vuint m_rAtomCat; ///< receptor atom category (std vector)

    double rbc[31][31];
    double abc[31][31];
    int iflo[31][31];  //? read from mkbest1.par (forcefield parameters)
    int ipon[3000][3000];

    double rc[3000][3000];  //TODO: should be translated into clean std::vectors
    double ac[3000][3000];


    uint m_ligSize ; ///<ligand size
    uint m_recSize ; ///<receptor size
    Attract2PairList m_pairlist; ///<ligand-receptor pairlist

    //Vuint m_lAtomCategory;
    //Vdouble m_lAtomCharge;
    //Vuint m_rAtomCategory;
    //Vdouble m_rAtomCharge;

    //std::vector<Coord3D> m_ligForces ; ///< ligand forces
    std::vector<Coord3D> m_recForces ; ///< ligand forces

    std::vector<AttractRigidbody> m_centeredligand;
    std::vector<AttractRigidbody> m_movedligand;
    std::vector<Coord3D> m_ligcenter;



    //private functions members:
        void Trans(uint molIndex, Vdouble& delta,uint shift, bool print=false); // translational derivatives
        void Rota(uint molIndex, double phi,double ssi, double rot, Vdouble& delta, uint shift, bool print=false);


};




} //namespace PTools





#endif //#ifndef FORCEFIELD_H

