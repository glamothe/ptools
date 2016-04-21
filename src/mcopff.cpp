#include "mcopff.h"
#include <cassert>
#include <iostream>
#include <math.h>

namespace PTools
{

/////////////////// -- Class Mcoprigid -- ////////////////////

Mcoprigid::Mcoprigid(){
    _complete = false;

};

Mcoprigid::Mcoprigid(std::string filename){

    ReadMcoprigidPDB(filename);
    _center = _core.FindCenter();
    iniWeights();
    
}

void Mcoprigid::iniWeights(){
    for(int i=0; i < _vregion.size(); i++){
        std::vector<dbl> newvector;
        _weights.push_back(newvector);
        _denorm_weights.push_back(newvector);
        for(int j=0; j < _vregion[i].size(); j++){
            dbl weight = 1/_vregion[i].size();
            _weights[i].push_back(weight);
            _denorm_weights[i].push_back(0);
        }
        denormalize_weights();
    }
}

void Mcoprigid::setCore(AttractRigidbody& core) {
    _core=core;
    _center = _core.FindCenter();

};


void Mcoprigid::AttractEulerRotate(const dbl& phi, const dbl& ssi, const dbl& rot)
{
//Warning: makes euler rotation without centering
//the Mcoprigid object must be centered


//rotates the core body:
    _core.AttractEulerRotate(phi, ssi, rot);


//for each multicopy region, rotates the copy:
    for (uint i=0; i < _vregion.size(); i++)
        for (uint j=0; j<_vregion[i].size(); j++)
            _vregion[i][j].AttractEulerRotate(phi, ssi, rot);

}


void Mcoprigid::Translate(const Coord3D& c)
{
//translates the core body:
    _core.Translate(c);

//for each multicopy region, translates the copy:
    for (uint i=0; i < _vregion.size(); i++)
        for (uint j=0; j < _vregion[i].size(); j++)
            _vregion[i][j].Translate(c);

}


void Mcoprigid::ReadMcoprigidPDB(const std::string name) {

	// pointer toward the filename given in the constructor argument
    std::ifstream file(name.c_str()); 
    if (!file)
    {
        std::ostringstream oss;
        throw std::invalid_argument("##### ReadPDB:Could not open file \"" + name + "\" #####") ;
    }

    ReadMcoprigidPDB(file, _core, _vregion);
    file.close();

}


void Mcoprigid::ReadMcoprigidPDB(std::istream& file, AttractRigidbody& core, std::vector<AttractMcop>& regions){

    uint region_num = 0;
    uint copy_num = 0;
    std::string line;
    while(std::getline(file, line)){
        if(Mcop::isNewModel(line)){
            // The line is a new model
            if(line.size() > 12) region_num = line_to_region_number(line);
            if(line.size() > 15) copy_num = line_to_copy_number(line);
            AttractRigidbody model;
            while(std::getline(file,line)){
                if(isAtom(line)){
                    // The line is an atom
                    Coord3D pos = pdbToCoords(line);
                    Atomproperty a;
                    pdbToAtomproperty(line, a);
                    // if region_num is core, append attom to core
                    if(region_num == 0) core.AddAtom(a,pos);
                    else model.AddAtom(a,pos);
                }
                
                else{  
                    // if region_num is not core, add region copy
                    if(region_num != 0){
                        // if new region, add region to vector before adding region copy
                        if(regions.size() < region_num)
                            regions.push_back(AttractMcop());
                        // copy
                        regions[region_num-1].addCopy(model);
                    }
                    //Just finished adding a region copy
                    break;
                }
            }
        }
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

uint Mcoprigid::line_to_region_number(std::string line){

    return std::atoi(line.substr(12,1).c_str());    
}

uint Mcoprigid::line_to_copy_number(std::string line){

    return std::atoi(line.substr(15,line.size()-15).c_str());
}

void Mcoprigid::denormalize_weights(){
    for(uint i; i < _weights.size(); i++){
        dbl max_weight = *max_element(_weights[i].begin(), _weights[i].end());
        for(uint j; j < _weights[i].size(); j++){
            _denorm_weights[i][j] = sqrt(_weights[i][j]/max_weight);
        }
    }
}

void Mcoprigid::normalize_weights(){

    for(uint i; i < _denorm_weights.size(); i++){
        dbl sum_squared_denorm_weights = 0;
        for(uint j; j < _denorm_weights[i].size(); j++){
            sum_squared_denorm_weights += pow(_denorm_weights[i][j], 2);
        }
        for(uint j; j < _denorm_weights[i].size(); j++){
            _weights[i][j] = pow(_denorm_weights[i][j], 2)/sum_squared_denorm_weights;
        }
    }
}


/////////////////// -- Class Mcop -- ////////////////////

Mcop::Mcop(std::string filename){
    ReadmcopPDB(filename);
}


void Mcop::ReadmcopPDB(const std::string name) {

    std::string nomfich = name ;
	// pointer toward the filename given in the constructor argument
    std::ifstream file(nomfich.c_str()); 
    if (!file)
    {
        std::ostringstream oss;
        throw std::invalid_argument("##### ReadPDB:Could not open file \"" + nomfich + "\" #####") ;
    }

    ReadmcopPDB(file, _copies);
    file.close();

}

void Mcop::ReadmcopPDB(std::istream& file, std::vector<Rigidbody>& protein) {
    
    ReadModelsPDB(file, protein);

}

void Mcop::ReadModelsPDB(std::istream& file, std::vector<Rigidbody>& protein){

    std::string line;
    while(std::getline(file, line)){
        if(isNewModel(line)){
            // The line is a new model
            Rigidbody model;
            while(std::getline(file,line)){
                if(isAtom(line)){
                    // The line is an atom
                    Coord3D pos = pdbToCoords(line);
                    Atomproperty a;
                    pdbToAtomproperty(line, a);
                    model.AddAtom(a,pos);
                }
                else{
                    protein.push_back(model);
                    //Just finished adding a model
                    break;
                }
            }
        }
    }
}


bool Mcop::isNewModel(const std::string & line){

    if(line.substr(0,5)==(std::string)"MODEL") return true;
    else return false;
}


/////////////////// -- Class AttractMcop -- ////////////////////
AttractMcop::AttractMcop(std::string filename){
    
    Mcop copies(filename);
    for(int i=0; i < copies.size(); i++){
        Rigidbody copy = copies.getCopy(i);
        AttractRigidbody attcopy = AttractRigidbody(copy);
        attract_copies.push_back(attcopy);
    }
    copies.clear();
}

AttractMcop::AttractMcop(const Mcop& mcop){
    
    Mcop copies = mcop;
    for(int i=0; i < copies.size(); i++){
        Rigidbody copy = copies.getCopy(i);
        AttractRigidbody attcopy = AttractRigidbody(copy);
        attract_copies.push_back(attcopy);
    }
    copies.clear();
}


/////////////////// -- Class McopForceField -- ////////////////////

///////////////////////////////////////////////////
//     Forcefield implementation
///////////////////////////////////////////////////



void McopForceField::calculate_weights(Mcoprigid& lig, bool print)
{


//loop over copies regions
    for (uint loopregion=0; loopregion < _receptor._vregion.size() ; loopregion++)
    {
        if (print)
          {
             std::cout << " Region: " << loopregion << "\n";
          }

        //calculates interaction energy between receptor copies and ligand body:
        std::vector<dbl> Eik;

        for (uint copy = 0; copy < _receptor._vregion[loopregion].size(); copy++)
        {
            AttractPairList cpl ( lig._core, _receptor._vregion[loopregion][copy], _cutoff );
            dbl e = _ff.nonbon8( lig._core, _receptor._vregion[loopregion][copy] , cpl );
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
            if (print) std::cout << "copy " << i << "  weight: " << weights[i] << std::endl; 
        }

        _receptor._weights[loopregion]=weights;




    }



}





/** \brief calculates energy of the system
*
* this functions returns nonbonded energy of a receptor with multicopy and a ligand without copy.
*
*/
dbl McopForceField::Function(const Vdouble & v)
{

    dbl ener = 0.0 ;
    dbl enercopy =0.0;

// 1) put the objects to the right place

    _moved_ligand = _centered_ligand;

    Mcoprigid & lig = _moved_ligand ;
    assert(lig._vregion.size()==0);


    lig.AttractEulerRotate(v[0],v[1],v[2]);
    lig.Translate(Coord3D(v[3],v[4],v[5]));


//2) calculates the energy


    //2.1) core ligand body with core receptor

    AttractPairList pl (_receptor._core,   lig._core, _cutoff );
    ener += _ff.nonbon8(_receptor._core, lig._core, pl );


    //2.2) core lignd with receptor copies:
    assert(_receptor._vregion.size() == _receptor._weights.size());
        
    for (uint loopregion=0; loopregion < _receptor._vregion.size() ; loopregion++)
    {

        //calculates interaction energy between receptor copies and ligand body:
//         std::vector<dbl> Eik;


        AttractMcop& ref_ensemble = _receptor._vregion[loopregion];
        std::vector<dbl>& ref_weights = _receptor._weights[loopregion];

        assert( ref_ensemble.size() == ref_weights.size());

        for (uint copynb = 0; copynb < ref_ensemble.size(); copynb++)
        {

            dbl& weight = ref_weights[copynb];
            AttractRigidbody& copy = ref_ensemble[copynb];

            AttractPairList cpl ( lig._core, copy, _cutoff );
            std::vector<Coord3D> copyforce(copy.Size());
            std::vector<Coord3D> coreforce(lig._core.Size());

//             dbl e = _ff.nonbon8( lig._core, _receptor._vregion[loopregion][copy] , cpl );
            dbl e = _ff.nonbon8_forces(lig._core, copy, cpl, coreforce, copyforce);
            enercopy += e * weight;//lig._weights[loopregion][copy];

            //multiply forces by copy weight:
            for(uint i=0; i<copyforce.size(); i++)
            { copyforce[i] = weight*copyforce[i]; }
            for(uint i=0; i<coreforce.size(); i++)
            { coreforce[i] = weight*coreforce[i]; }

            //add force to core ligand and receptor copy
            assert(lig._core.Size() == coreforce.size());
            for (uint i=0; i<lig._core.Size(); i++)
               lig._core.m_forces[i]+=coreforce[i];

            assert(copy.Size()==copyforce.size());
            for(uint i=0; i<copyforce.size(); i++)
               copy.m_forces[i]+=copyforce[i];


        }


    }

    return ener + enercopy;

}


void McopForceField::Derivatives(const Vdouble& v, Vdouble & g )
{

//sum the forces over x, y and z:

Coord3D ligtransForces; //translational forces for the ligand:
for(uint i=0; i<_moved_ligand._core.Size(); i++)
 {
     ligtransForces += _moved_ligand._core.m_forces[i];
 }


Coord3D receptortransForces;
for(uint i=0; i<_receptor._core.Size(); i++)
{
receptortransForces+= _receptor._core.m_forces[i];
}


for (uint i=0; i <_receptor._vregion.size(); i++)
 {
   AttractMcop& ens = _receptor._vregion[i];
   std::vector<dbl> & weights =  _receptor._weights[i];

   for(uint j=0; j<ens.size(); j++)
   {
     AttractRigidbody& copy = ens[j];
       for (uint atomnb=0; atomnb < copy.Size(); ++atomnb)
       {
            receptortransForces += weights[j] * copy.m_forces[atomnb];
       }

   }

//TODO DEBUG:
std::cout << "differences between the two forces: " <<  (ligtransForces - receptortransForces).toString() << std::endl ;

 }





}


} // namespace PTools





