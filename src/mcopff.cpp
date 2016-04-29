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

// Normalized weight initialized at 1/(number of copies) and denormalized weights initialized at 0.
void Mcoprigid::iniWeights(){
    for(uint i=0; i < _vregion.size(); i++){
        std::vector<dbl> newvector;
        _weights.push_back(newvector);
        _denorm_weights.push_back(newvector);
        for(uint j=0; j < _vregion[i].size(); j++){
            dbl weight = 1.0/(double)_vregion[i].size();
            _weights[i].push_back(weight);
            _denorm_weights[i].push_back(0);
        }
    }
    denormalize_weights();
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
    for(uint i=0; i < _weights.size(); i++){
        dbl max_weight = *max_element(_weights[i].begin(), _weights[i].end());
        for(uint j=0; j < _weights[i].size(); j++){
            _denorm_weights[i][j] = sqrt(_weights[i][j]/max_weight);
        }
    }
}

void Mcoprigid::normalize_weights(){

    for(uint i=0; i < _denorm_weights.size(); i++){
        dbl sum_squared_denorm_weights = 0;
        for(uint j=0; j < _denorm_weights[i].size(); j++){
            sum_squared_denorm_weights += pow(_denorm_weights[i][j], 2);
        }
        for(uint j=0; j < _denorm_weights[i].size(); j++){
            _weights[i][j] = pow(_denorm_weights[i][j], 2)/sum_squared_denorm_weights;
        }
    }
}

void Mcoprigid::updateWeights(const std::vector<dbl>& v, int svptr){
    //svptr: state variable "pointer"
    for(uint loopregion=0; loopregion < _vregion.size() ; loopregion++){
        for(uint copynb = 0; copynb < _vregion[loopregion].size(); copynb++){
            dbl & d_w = _denorm_weights[loopregion][copynb];
            d_w = v[svptr] + d_w; //delta weight + original weight 
            svptr += 1;
        }
    }
    normalize_weights();
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
    for(uint i=0; i < copies.size(); i++){
        Rigidbody copy = copies.getCopy(i);
        AttractRigidbody attcopy = AttractRigidbody(copy);
        attract_copies.push_back(attcopy);
    }
    copies.clear();
}

AttractMcop::AttractMcop(const Mcop& mcop){
    
    Mcop copies = mcop;
    for(uint i=0; i < copies.size(); i++){
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


void McopForceField::ini_energies(){
    for(uint i=0; i < _receptor.getRegions().size(); i++){
        std::vector<dbl> newvector;
        _mcop_E.push_back(newvector);
        for(uint j=0; j < _receptor.getRegions()[i].size(); j++){
            _mcop_E[i].push_back(0);
        }
    }
}

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


uint McopForceField::ProblemSize()
{
    uint size = 0;
    if (_centered_ligand.getCore().hastranslation) size += 3;
    if (_centered_ligand.getCore().hasrotation) size += 3;
    for(uint i=0; i<_receptor.getWeights().size(); i++){
        size += _receptor.getWeights()[i].size();
    }
    return size;
}



/** \brief calculates energy of the system
*
* this functions returns nonbonded energy of a receptor with multicopy and a ligand without copy.
*
*/
dbl McopForceField::Function(const Vdouble & v)
{
    denormalize_weights();

    dbl ener_region = 0.0 ;
    dbl ener_core = 0.0 ;

// 1) put the objects to the right place

    _moved_ligand = _centered_ligand;

    Mcoprigid & lig = _moved_ligand ;
    assert(lig._vregion.size()==0);
    //std::vector< std::vector<dbl> > weights;

    //TODO: take into account if no rotation or no translation
    lig.AttractEulerRotate(v[0],v[1],v[2]);
    lig.Translate(Coord3D(v[3],v[4],v[5]));
    assert(_receptor._vregion.size() == _receptor._weights.size());
    assert(_receptor._vregion.size() == _receptor._denorm_weights.size());

    //Update the denormalized weights
    
    uint svptr = 0; // stateVars 'pointer'
    if (lig.getCore().hasrotation) svptr += 3;
    if (lig.getCore().hastranslation) svptr += 3;
    std::vector< std::vector<dbl> > denorm_weights = _receptor._denorm_weights;
    for (uint loopregion=0; loopregion < _receptor._vregion.size() ; loopregion++){
        assert( ref_ensemble.size() == ref_denorm_weights.size());
        AttractMcop& ref_ensemble = _receptor._vregion[loopregion];
        std::vector<dbl> & denorm_weights_loop  = denorm_weights[loopregion];
        for (uint copynb = 0; copynb < ref_ensemble.size(); copynb++){
            dbl & denorm_weight = denorm_weights_loop[copynb];
            denorm_weight = v[svptr] + denorm_weight; //delta weight + original weight
            svptr += 1;
        }
    }

    normalize_weights();
    std::vector< std::vector<dbl> > weights = _receptor._weights;
//2) calculates the energy


    //2.1) core ligand body with core receptor
    AttractPairList pl (_receptor._core, lig._core, _cutoff );
    ener_core += _ff.nonbon8(_receptor._core, lig._core, pl );

    //2.2) core lignd with receptor copies:

        
    for (uint loopregion=0; loopregion < _receptor._vregion.size() ; loopregion++)
    {

        //calculates interaction energy between receptor copies and ligand body:
//         std::vector<dbl> Eik;

        dbl enercopy =0.0;
        AttractMcop& ref_ensemble = _receptor._vregion[loopregion];
        std::vector<dbl>& denorm_weights_loop = denorm_weights[loopregion];
        std::vector<dbl>& weights_loop = weights[loopregion];

        assert( ref_ensemble.size() == denorm_weights_loop.size());
        assert( ref_ensemble.size() == weights_loop.size());

        dbl max_weight = *max_element(weights_loop.begin(), weights_loop.end());

        for (uint copynb = 0; copynb < ref_ensemble.size(); copynb++)
        {

            dbl& denorm_weight = denorm_weights_loop[copynb];
            AttractRigidbody& copy = ref_ensemble[copynb];

            AttractPairList cpl ( lig._core, copy, _cutoff );
            std::vector<Coord3D> copyforce(copy.Size());
            std::vector<Coord3D> coreforce(lig._core.Size());

//             dbl e = _ff.nonbon8( lig._core, _receptor._vregion[loopregion][copy] , cpl );
            dbl e = _ff.nonbon8_forces(lig._core, copy, cpl, coreforce, copyforce);
            _mcop_E[loopregion][copynb] = e;

            enercopy += e*pow(denorm_weight, 2);//lig._denorm_weights[loopregion][copy];
             //multiply forces by copy weight:
            for(uint i=0; i<copyforce.size(); i++)
            { copyforce[i] = pow(denorm_weight,2)*copyforce[i]; }
            for(uint i=0; i<coreforce.size(); i++)
            { coreforce[i] = pow(denorm_weight,2)*coreforce[i]; }

            //add force to ligand and receptor copy
            assert(lig._core.Size() == coreforce.size());
            for(uint i=0; i<lig._core.Size(); i++)
               lig._core.m_forces[i]+=coreforce[i];

            assert(copy.Size()==copyforce.size());
            for(uint i=0; i<copyforce.size(); i++)
               copy.m_forces[i]+=copyforce[i];
        }

        ener_region += max_weight*enercopy;
    }
    _receptor._buffer_weights = weights;
    _receptor._buffer_denorm_weights = denorm_weights;
    return ener_core + ener_region;

}

// To calculation interaction energy between an Mcoprigid receptor with variable region(s) 
// and an Mcoprigid ligand without variable regions.
// Uses the receptor's normalized weights and a given forcefeild and cutoff. 
dbl McopForceField::CalcEnergy(Mcoprigid & receptor, Mcoprigid & ligand, BaseAttractForceField & ff, dbl cutoff){

    dbl ener_region = 0.0 ;
    dbl ener_core = 0.0 ;

    AttractPairList pl (receptor._core, ligand._core, cutoff );
    ener_core += ff.nonbon8(receptor._core, ligand._core, pl );

    //calculates interaction energy between receptor copies and ligand body:
    for(uint loopregion=0; loopregion < receptor.getRegions().size(); loopregion++){
        
        dbl enercopy =0.0;
        
        for(uint copynb=0; copynb < receptor.getRegion(loopregion).size(); copynb++){
            
            dbl weight = receptor.getWeights()[loopregion][copynb];
            AttractRigidbody& copy = receptor.getRegion(loopregion).getCopy(copynb);
            AttractPairList cpl (ligand._core, copy, cutoff);

            std::vector<Coord3D> copyforce(copy.Size());
            std::vector<Coord3D> coreforce(ligand._core.Size());
            
            dbl e = ff.nonbon8_forces(ligand._core, copy, cpl, coreforce, copyforce);
            enercopy += e*weight;
        }
        
        ener_region += enercopy;
    }
    return ener_core + ener_region;
}


void McopForceField::Derivatives(const Vdouble& v, Vdouble & g )
{

Mcoprigid & lig = _moved_ligand;
uint svptr = 0; // stateVars 'pointer'

// calculate de rotational forces:
if (lig.getCore().hasrotation){
    dbl  cs,cp,ss,sp,cscp,sscp,sssp,crot,srot,xar,yar,cssp,X,Y,Z ;
    dbl  pm[3][3];

    // !c     calculates orientational force contributions
    // !c     component 1: phi-angle
    // !c     component 2: ssi-angle
    // !c     component 3: rot-angle

    g[svptr+0] = 0;
    g[svptr+1] = 0;
    g[svptr+2] = 0;

    cs=cos(v[svptr+1]);
    cp=cos(v[svptr+0]);
    ss=sin(v[svptr+1]);
    sp=sin(v[svptr+0]);
    cscp=cs*cp;
    cssp=cs*sp;
    sscp=ss*cp;
    sssp=ss*sp;
    crot=cos(v[svptr+2]);
    srot=sin(v[svptr+2]);

    // for the x, y and z coordinates, we need
    // the coordinates of the centered, non-translated molecule

    AttractRigidbody * pLigCentered = & _centered_ligand._core; // pointer to the centered ligand
    AttractRigidbody * pLigMoved  = & _moved_ligand._core; // pointer to the rotated/translated ligand (for forces)

    assert(shift+2 < delta.size());

    for (uint loopregion=0; loopregion < _receptor._vregion.size() ; loopregion++){
        
        std::vector<dbl>& ref_weights = _receptor._buffer_weights[loopregion];
        dbl max_weight = *max_element(ref_weights.begin(), ref_weights.end());

        for (uint i=0; i< pLigCentered->m_activeAtoms.size(); i++)
        {
            uint atomIndex = pLigCentered->m_activeAtoms[i];

            Coord3D coords = pLigCentered->GetCoords(atomIndex);
            X = coords.x;
            Y = coords.y;
            Z = coords.z;

            xar=X*crot+Y*srot;
            yar=-X*srot+Y*crot;
            pm[0][0]=-xar*cssp-yar*cp-Z*sssp ;
            pm[1][0]=xar*cscp-yar*sp+Z*sscp ;
            pm[2][0]=0.0 ;

            pm[0][1]=-xar*sscp+Z*cscp ;
            pm[1][1]=-xar*sssp+Z*cssp ;
            pm[2][1]=-xar*cs-Z*ss ;

            pm[0][2]=yar*cscp+xar*sp ;
            pm[1][2]=yar*cssp-xar*cp ;
            pm[2][2]=-yar*ss ;

            for(uint j=0; j < 3; j++){
                g[svptr + j] += pm[0][j] * pLigMoved->m_forces[atomIndex].x * max_weight;
                g[svptr + j] += pm[1][j] * pLigMoved->m_forces[atomIndex].y * max_weight;
                g[svptr + j] += pm[2][j] * pLigMoved->m_forces[atomIndex].z * max_weight;
            }
        }

    }
    svptr += 3;

}

//sum the forces over x, y and z: calculate de translational forces
if (lig.getCore().hastranslation){
    g[svptr+0] = 0;
    g[svptr+1] = 0;
    g[svptr+2] = 0;


    for (uint loopregion=0; loopregion < _receptor._vregion.size() ; loopregion++){
        
        std::vector<dbl>& ref_weights = _receptor._buffer_weights[loopregion];
        dbl max_weight = *max_element(ref_weights.begin(), ref_weights.end());
        Coord3D ligtransForces; //translational forces for the ligand from loopregion
        
        for(uint i=0; i<_moved_ligand._core.Size(); i++){
             ligtransForces += _moved_ligand._core.m_forces[i];
        }

        g[svptr+0] += ligtransForces.x * max_weight;
        g[svptr+1] += ligtransForces.y * max_weight;
        g[svptr+2] += ligtransForces.z * max_weight;
    }
   
    svptr += 3;
}

// Calculate de weight derivative

assert(_receptor._vregion.size() == _mcop_E.size());
uint k = 0;
for (uint loopregion=0; loopregion < _receptor._vregion.size() ; loopregion++){
        
    std::vector<dbl>& ref_weights = _receptor._buffer_weights[loopregion];
    std::vector<dbl>& ref_denorm_weights = _receptor._buffer_denorm_weights[loopregion];
    std::vector<dbl>& ref_mcop_E = _mcop_E[loopregion];
    assert(ref_weights.size() == ref_mcop_E.size());
        
    dbl max_weight = *max_element(ref_weights.begin(), ref_weights.end());
    for(uint copynb=0; copynb < _mcop_E[loopregion].size(); copynb++){
        // weight derivative function
        g[svptr + k] = 2*max_weight*ref_denorm_weights[copynb]*ref_mcop_E[copynb];
        k++;
    }
}

/*Coord3D receptortransForces; //translational forces for the receptor:
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

 }*/





}


} // namespace PTools





