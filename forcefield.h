#ifndef FORCEFIELD_H
#define FORCEFIELD_H


#include "basetypes.h"
#include "rigidbody.h"
#include "attractrigidbody.h"
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

    ///the function to optimize
    virtual dbl Function(const Vdouble&)=0;

    /// analytical derivative of the function above
    virtual void Derivatives(const Vdouble& StateVars, Vdouble& delta)
    {
        NumDerivatives(StateVars, delta, true);
    }

    ///numerical derivative for testing purpose. Not very accurate
    virtual void NumDerivatives(const Vdouble& StateVars, Vdouble& delta, bool print=false);

    ///size of the problem (number of variables the minimizer must optimize)
    virtual uint ProblemSize()=0;

    ///this function is called at the beginning of a minimization, by the minimizer (Lbfgs)
    virtual void initMinimization()=0;

    ///virtual destructor (Effective C++ - Scott Meyers - Item 7)
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

    ///called before every minimization by the minimizer (Lbfgs)
    virtual void initMinimization();
    ///analytical derivative
    void Derivatives(const Vdouble&, Vdouble&);
    uint ProblemSize();
    dbl Function(const Vdouble&);

    ///add a new ligand to the ligand list...
    void AddLigand(AttractRigidbody & lig);

    ///after a minimization, get minimized ligand 'i'
    AttractRigidbody GetLigand(uint i);

    /// this function generates the pairlists before a minimization
    void MakePairLists();

    ///non-bonded interactions
    virtual dbl nonbon8(AttractRigidbody& rec, AttractRigidbody& lig, AttractPairList & pairlist, bool print=false)
    {
        std::vector<Coord3D> forcesrec (rec.Size());
        std::vector<Coord3D> forceslig (lig.Size());

        dbl ener = nonbon8_forces(rec, lig, pairlist, forcesrec, forceslig, print);
        rec.addForces(forcesrec);
        lig.addForces(forceslig);
        return ener;
    }

    ///non-bonded interactions, forces are returned separately
    virtual dbl nonbon8_forces(AttractRigidbody& rec, AttractRigidbody& lig, AttractPairList & pairlist, std::vector<Coord3D>& forcerec, std::vector<Coord3D>& forcelig, bool print=false)=0;

    virtual ~BaseAttractForceField(){};


    ///translational derivatives
    void Trans(uint molIndex, Vdouble& delta,uint shift, bool print=false);

    ///rotational derivatives
    void Rota(uint molIndex, dbl phi, dbl ssi, dbl rot, Vdouble& delta, uint shift, bool print=false);



protected:
    //private variables
    Vuint m_rAtomCat; ///< receptor atom category (std vector)
    std::vector<AttractPairList> m_pairlists ; ///< pair lists
    std::vector<AttractRigidbody> m_centeredligand; ///< array of ligands with their centroid at O (required for Euler rotations)
    std::vector<AttractRigidbody> m_movedligand;
    std::vector<Coord3D> m_ligcenter; ///< list of ligands centroids before centering.
    dbl m_cutoff; ///< cutoff for the pairlist generation


    


private:
    //private functions members:


    ///set list of ignored atom types (dummy atoms)
    virtual void setDummyTypeList(AttractRigidbody& lig)=0;


};



class AttractForceField1: public BaseAttractForceField
{
public:
    void InitParams(const std::string & paramsFileName);
    AttractForceField1(std::string paramsFileName, dbl cutoff);
    dbl nonbon8_forces(AttractRigidbody& rec, AttractRigidbody& lig, AttractPairList & pairlist, std::vector<Coord3D>& forcerec, std::vector<Coord3D>& forcelig, bool print=false);

    virtual ~AttractForceField1(){};
private:

    Vdouble m_rad ; //Ri LJ (8,6) parameter
    Vdouble m_amp ; //Ai LJ (8,6) parameter
    // m_rad and m_amp are the Ri (in Angstrom) and Ai (in [RT]^1/2) Lennard-Jones (8,6) parameters respectively as described in M. Zacharias Prot. Sci. 2003, 12, 1271-1282.


    dbl m_rc[64][64]; //some pre-calculated results
    dbl m_ac[64][64]; //some pre-calculated results
    // m_rc[i][j] and m_ac[i][j] are the pre-calculated repulsive (Bij) and attractive (Cij) pair-wise interactions between pseudo atom i and j at a distance rij calculated from: Eij= [Bij/(rij)^8 - Cij/(rij)^6] with Bij = AiAj(Ri+Rj)^8 and Cij = AiAj(Ri+Rj)^6

    int m_ligRestraintIndex;

    dbl m_rstk;

    void setDummyTypeList(AttractRigidbody& lig){}; //forcefield1 has no dummy type
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



/////////////////////////////////////////////////
//         ForceField2
/////////////////////////////////////////////////

/*! \brief Attract ForceField2 parameters
*
*/
struct AttFF2_params
{
    int ipon[31][31];  // flag to switch between saddle point and "normal" minimum curve of the LJ potential.

    dbl rc[31][31];  // some pre-calculated results equal to abc[i][j]*rbc[i][j]^8
    dbl ac[31][31];  // some pre-calculated results equal to abc[i][j]*rbc[i][j]^6

    dbl emin[31][31];  // pre-calculated energy needed to flip the curve from the "normal" LJ minimum to the saddle point.
    dbl rmin2[31][31];  // pre-calculated square distance to switch between the saddle point part and the "normal" minimum one of the LJ potential.
    dbl rbc[31][31];  // pair-wise LJ (8,6) parameters
    dbl abc[31][31];  // pair-wise LJ (8,6) parameters
    int iflo[31][31];  // flag equivalent to ipon (should be removed!)

    std::vector<uint> _dummytypes;

    // In the FF2, the LJ potential flips from a "normal" minimum curve in case of an attractive interaction between pseudo-atoms (iflo=1) to a repulsive saddle-point one in case of a repulsive interaction (iflo=-1).
    // the LJ curve is split into 2 parts: 
    // if the square distance (rij^2) between the pseudo atoms is above rmin2 value, the LJ energy is equal to : Eij = ivor*(rc[i][j]/rij^8 - ac[i][j]/rij^6)
    // if the square distance (rij^2) between the pseudo atoms is below rmin2 value, the LJ energy is equal to : Eij = ivor*(rc[i][j]/rij^8 - ac[i][j]/rij^6) + (ivor-1)*emin


};



/*!  \brief Attract forcefield version 2
*
*   new forcefield with saddle point and pairwise energy
*/
class AttractForceField2 : public BaseAttractForceField
{

public:

    AttractForceField2(const std::string & paramsFileName, dbl cutoff);
    dbl nonbon8_forces(AttractRigidbody& rec, AttractRigidbody& lig, AttractPairList & pairlist, std::vector<Coord3D>& forcerec, std::vector<Coord3D>& forcelig, bool print=false);

    ///allows to reload a file of parameters
    void reloadParams(const std::string & filename, dbl cutoff);


private:

    void resetParams();
    void loadParams(const std::string & filename, dbl cutoff);

    virtual void setDummyTypeList(AttractRigidbody& lig);
    std::string m_filename;   ///< name of parameter file



};


} //namespace PTools





#endif //#ifndef FORCEFIELD_H

