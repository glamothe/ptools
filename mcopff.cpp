#include "mcopff.h"
#include <cassert>


namespace PTools
{


Mcoprigid::Mcoprigid()
{
    _complete = false;

};




void Mcoprigid::SetMain(AttractRigidbody& main) {
    _main=main;
    _center = _main.FindCenter();

};


void Mcoprigid::AttractEulerRotate(const dbl& phi, const dbl& ssi, const dbl& rot)
{
//Warning: makes euler rotation without centering
//the Mcoprigid object must be centered


//rotates the main body:
    _main.AttractEulerRotate(phi, ssi, rot);


//for each multicopy region, rotates the copy:
    for (uint i=0; i < _vregion.size(); i++)
        for (uint j=0; j<_vregion[i].size(); j++)
            _vregion[i][j].AttractEulerRotate(phi, ssi, rot);

}


void Mcoprigid::Translate(const Coord3D& c)
{
//translates the main body:
    _main.Translate(c);

//for each multicopy region, translates the copy:
    for (uint i=0; i < _vregion.size(); i++)
        for (uint j=0; j<_vregion[i].size(); j++)
            _vregion[i][j].Translate(c);

}



void McopForceField::calculate_weights(Mcoprigid& lig)
{


//loop over copies regions
    for (uint loopregion=0; loopregion < _receptor._vregion.size() ; loopregion++)
    {

        //calculates interaction energy between receptor copies and ligand body:
        std::vector<dbl> Eik;

        for (uint copy = 0; copy < _receptor._vregion[loopregion].size(); copy++)
        {
            Attract2PairList cpl ( lig._main, _receptor._vregion[loopregion][copy], _cutoff );
            dbl e = _ff.nonbon8( lig._main, _receptor._vregion[loopregion][copy] , cpl );
            Eik.push_back(e);
        }



        const dbl RT = 0.592 * 298.0 ;

        //calculate weights
        dbl sumweights = 0.0;
        std::vector<dbl> weights;

        for (uint i=0; i<Eik.size(); i++)
        {
            dbl e = exp( -Eik[i]/RT) ;
            weights.push_back( e )  ;
            sumweights += e ;
        }

        //normalize weights

        for (uint i=0; i< weights.size(); i++)
        {
            weights[i] = weights[i]/sumweights ;
        }

        _receptor._weights[loopregion]=weights;



    }



}


void Mcoprigid::PrintWeights()
{
    for (uint loopregion=0; loopregion<_weights.size(); loopregion++)
    {
        std::cout << "copy region: " << loopregion << std::endl << "weights: ";
        for (uint copy=0; copy<_weights[loopregion].size(); copy++)
        {
            std::cout << _weights[loopregion][copy] << "  " ;
        }

    }
}



/** \brief calculates energy of the system

this functions returns nonbonded energy of a receptor with multicopy and a ligand without copy.

*/
dbl McopForceField::Function(const Vdouble & v)
{

    dbl ener = 0.0 ;
    dbl enercopy =0.0;

// 1) put the objects to the right place
    Mcoprigid lig (_centered_ligand);
    assert(lig._vregion.size()==0);


    lig.AttractEulerRotate(v[0],v[1],v[2]);
    lig.Translate(Coord3D(v[3],v[4],v[5]));


//2) calculates the energy


    //2.1) main ligand body with main receptor

    Attract2PairList pl (_receptor._main,   lig._main, _cutoff );
    ener += _ff.nonbon8(_receptor._main, lig._main, pl );


    //2.2) main lignd with receptor copies:
    assert(_receptor._vregion.size() == _receptor._weights.size());

    for (uint loopregion=0; loopregion < _receptor._vregion.size() ; loopregion++)
    {

        //calculates interaction energy between receptor copies and ligand body:
//         std::vector<dbl> Eik;

        assert(_receptor._vregion[loopregion].size() == _receptor._weights[loopregion].size());

        for (uint copy = 0; copy < _receptor._vregion[loopregion].size(); copy++)
        {
            Attract2PairList cpl ( lig._main, _receptor._vregion[loopregion][copy], _cutoff );
            dbl e = _ff.nonbon8( lig._main, _receptor._vregion[loopregion][copy] , cpl );
            enercopy += e * lig._weights[loopregion][copy];

        }


    }

    return ener + enercopy;

}


void McopForceField::Derivatives(const Vdouble& v, Vdouble & g )
{



}





} // namespace PTools
