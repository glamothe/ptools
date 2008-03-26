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


    //virtual dbl Function()=0; //simply return the function value for
    virtual dbl Function(const Vdouble&)=0;

    /// analytical derivative
    virtual void Derivatives(const Vdouble& StateVars, Vdouble& delta)
    {
        NumDerivatives(StateVars, delta, true);
    }
    virtual void NumDerivatives(const Vdouble& StateVars, Vdouble& delta, bool print=false); ///< numerical derivative for testing purpose
    virtual uint ProblemSize()=0;
    virtual void initMinimization()=0; ///< this function is called at the beginning of a minimization

    virtual ~ForceField(){};

} ;

/*
class AttractForceField: public ForceField
{

public:

    AttractForceField(const Rigidbody& recept,const Rigidbody& lig,dbl cutoff);
    virtual ~AttractForceField(){};


    //this is to satisfy the abstract class:
    //virtual dbl Function() {return Energy();};
    virtual dbl Function(const Vdouble& stateVars) {
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


    dbl Energy(); ///< return current energy without moving any object
    dbl Energy(const Vdouble& stateVars); // stateVars: tx ty tz theta phi psi

    dbl Electrostatic();
    dbl LennardJones();

    void Gradient(const Vdouble& stateVars, Vdouble& delta);

    void ShowEnergyParams(); ///< for debug purposes...
    void Trans() {
        Vdouble delta(6) ;
        Trans(delta,true);
    };
    void SetRestraint(dbl rstk) {
        m_rstk = rstk;
    };






private:
    //private methods:
    void InitParams();

    void Trans(Vdouble& delta, bool print=false); // translational derivatives
    void Rota(dbl phi,dbl ssi, dbl rot, Vdouble& delta, bool print=false);
    void ResetForces();

    dbl Constraints(); ///< distance constraints between ligand and receptor

    //private data
    Rigidbody m_refreceptor, m_refligand;
    Rigidbody m_receptor, m_ligand;
    Rigidbody m_savligand;

   

    std::vector<Coord3D> m_ligforces ; //forces

    Coord3D m_ligcenter; //center of mass of the ligand
    Coord3D m_reccenter; //center of mass of the receptor (used for the distance constraint)

    Vuint m_rAtomCat; //receptor atom category (as seen in reduced pdb files)
    Vuint m_lAtomCat; //ligand atom category

    int m_ligRestraintIndex; //index of the ligand atom on which the retraint force should be applied

    Vdouble m_rAtomCharge;
    Vdouble m_lAtomCharge;



    bool m_energycalled ;

    PairList plist;
    std::string _filesuffix;

    static int _minimnb;
    dbl m_rstk;
} ;*/


///////////////////////////////////////////////////////////////
/*!  \brief Common base class for Attract forcefields 1 and 2
*
*   Attract forcefields 1 and 2 are very similar. 
*   nonbon8 and paramters initialization are different, but
*   rotational/translational derivatives can be simple base functions
*/
class BaseAttractForceField: public ForceField
{

public:

    void initMinimization();
    void Derivatives(const Vdouble&, Vdouble&); ///< analytical derivative
    uint ProblemSize(); 
    dbl Function(const Vdouble&);
    void AddLigand(AttractRigidbody & lig); ///< add a new ligand to the ligand list...
    void MakePairLists(); ///< this function generates the pairlists before a minimization
    AttractRigidbody GetLigand(uint i);

    virtual dbl nonbon8(AttractRigidbody& rec, AttractRigidbody& lig, Attract2PairList & pairlist, bool print=false)=0;

    virtual ~BaseAttractForceField(){};

protected:
    //private variables
    Vuint m_rAtomCat; ///< receptor atom category (std vector)
    std::vector<Attract2PairList> m_pairlists ;
    std::vector<AttractRigidbody> m_centeredligand;
    std::vector<AttractRigidbody> m_movedligand;
    std::vector<Coord3D> m_ligcenter;

    dbl m_cutoff;


    //private functions members:
    void Trans(uint molIndex, Vdouble& delta,uint shift, bool print=false); // translational derivatives
    void Rota(uint molIndex, dbl phi, dbl ssi, dbl rot, Vdouble& delta, uint shift, bool print=false);


};



class AttractForceField1: public BaseAttractForceField
{
public:
    void InitParams(const std::string & paramsFileName);
    AttractForceField1(std::string paramsFileName, dbl cutoff);
    dbl nonbon8(AttractRigidbody& rec, AttractRigidbody& lig, Attract2PairList & pairlist, bool print=false);

    virtual ~AttractForceField1(){};
private:

    Vdouble m_rad ; //rad parameter ... RADius?
    Vdouble m_amp ; //amp parameter ... AMPlitude??

    dbl m_rc[64][64]; //some pre-calculated results
    dbl m_ac[64][64]; //some pre-calculated results

    int m_ligRestraintIndex;

    dbl m_rstk;
};









class TestForceField: public ForceField
{

//virtual dbl Function() {return 0.0;};

    virtual uint ProblemSize(){
        return 2;
    };

    virtual dbl Function(const Vdouble& X)
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



///////////////////////////////////////////////////////
/*!  \brief Attract forcefield version 2
*
*   new forcefield with saddle point and pairwise energy
*/

struct AttFF2_params
{
    int ipon[31][31];
// Array2D<int> ipon;
// Array2D<dbl> rc;
    dbl rc[31][31];  //TODO: should be translated into clean std::vectors
// Array2D<dbl> ac;
    dbl ac[31][31];


    dbl emin[31][31];
    dbl rmin2[31][31];
    dbl rbc[31][31];
    dbl abc[31][31];
    int iflo[31][31];  //? read from mkbest1.par (forcefield parameters)

//AttFF2_params() {ipon=Array2D<int>(3000,3000); ac=Array2D<dbl>(3000,3000); rc=Array2D<double>(3000,3000); }

};






class AttractForceField2 : public BaseAttractForceField
{

public:

//     void initMinimization();
    AttractForceField2(std::string paramsFileName, dbl cutoff);
    dbl nonbon8(AttractRigidbody& rec, AttractRigidbody& lig, Attract2PairList & pairlist, bool print=false);

private:


//     std::vector<Coord3D> m_recForces ; ///< ligand forces


};


} //namespace PTools





#endif //#ifndef FORCEFIELD_H

