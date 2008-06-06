//  multicopy-forcefield
//
//



#ifndef _MCOPFF_H_
#define _MCOPFF_H_

#include "forcefield.h"
#include "rigidbody.h"

namespace PTools{


typedef std::vector<AttractRigidbody> ensemble ;

class Mcoprigid //multicopy rigidbody
{

public:

    Mcoprigid();

    //using default copy operator

    void setMain(AttractRigidbody& main) ;
    void addEnsemble(const ensemble& cr){ _vregion.push_back(cr); std::vector<dbl> v; _weights.push_back(v);  };


    void AttractEulerRotate(const dbl& phi, const dbl& ssi, const dbl& rot);
    void Translate(const Coord3D& c);


    void PrintWeights();
    std::vector <std::vector<dbl> > getWeights(){return _weights;};


private:

    AttractRigidbody _main;
    std::vector< ensemble > _vregion ;

    bool _complete ;
    Coord3D _center ; ///<center of mass of the main region

    std::vector< std::vector <dbl> > _weights;

    friend class McopForceField;

};



/** \brief ForceField with multicopy
needs an attract forcefield (either 1 or 2) in constructor

no normal modes yet
*/
class McopForceField: public ForceField
{

public:

    McopForceField(BaseAttractForceField& ff, dbl cutoff)
            :_ff(ff), _cutoff(cutoff) {};


    dbl Function(const Vdouble&);
    void Derivatives(const Vdouble& v, Vdouble & g );


private:

    BaseAttractForceField& _ff ;
    dbl _cutoff;

    bool _update_weights;

    Mcoprigid _centered_ligand ;
    Mcoprigid _moved_ligand ;
    Mcoprigid _receptor;


    void calculate_weights(Mcoprigid& lig);


};

}//namespace PTools

#endif // _MCOPFF_H_



