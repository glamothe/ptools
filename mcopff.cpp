// $Id$
#include "mcopff.h"
#include <cassert>


namespace PTools
{


Mcoprigid::Mcoprigid()
{
    m_complete = false;

}




void Mcoprigid::setMain(AttractRigidbody& main) {
    m_main=main;
    m_center = m_main.FindCenter();

}


void Mcoprigid::AttractEulerRotate(const dbl& phi, const dbl& ssi, const dbl& rot)
{
//Warning: makes euler rotation without centering
//the Mcoprigid object must be centered


//rotates the main body:
    m_main.AttractEulerRotate(phi, ssi, rot);


//for each multicopy region, rotates the copy:
    for (uint i=0; i < m_vregion.size(); i++)
        for (uint j=0; j < m_vregion[i].size(); j++)
            m_vregion[i][j].AttractEulerRotate(phi, ssi, rot);

}


void Mcoprigid::Translate(const Coord3D& c)
{
//translates the main body:
    m_main.Translate(c);

//for each multicopy region, translates the copy:
    for (uint i=0; i < m_vregion.size(); i++)
        for (uint j=0; j<m_vregion[i].size(); j++)
            m_vregion[i][j].Translate(c);

}



void Mcoprigid::PrintWeights()
{
    for (uint loopregion=0; loopregion< m_weights.size(); loopregion++)
    {
        std::cout << "copy region: " << loopregion << std::endl << "weights: ";
        for (uint copy=0; copy < m_weights[loopregion].size(); copy++)
        {
            std::cout << m_weights[loopregion][copy] << "  " ;
        }

    }
}





///////////////////////////////////////////////////
//     Forcefield implementation
///////////////////////////////////////////////////


McopForceField::~McopForceField()
{
for(uint i=0; i<m_forcefields.size(); i++)
  delete m_forcefields[i];
}


void McopForceField::dbgPlayWithFF()
{
BaseAttractForceField* ff = m_forcefields[0];

Vdouble v(6);
v[3]=50;
dbl e=ff->Function(v);

std::cerr << "PlayWithFF: e = " << e;
std::cerr << "\nPlayWithFF: atom1 position=\n";

AttractRigidbody movedlig = ff->getMovedLigand(1);
Coord3D co = movedlig.GetCoords(0);

std::cerr << co.toString();

}


void McopForceField::init()
{

//loop over copies regions
    for (uint loopregion=0; loopregion < m_receptor.m_vregion.size() ; loopregion++)
    {
        if (m_verbose)
        {
            std::cout << "Creating forcefields for Region: " << loopregion << "\n";
        }


        //calculates interaction energy between receptor copies and ligand body:


        for (uint copy = 0; copy < m_receptor.m_vregion[loopregion].size(); copy++)
        {

            BaseAttractForceField * ff = m_creator(m_paramsfile, m_cutoff);   //we now have a FF1 or FF2 where we can shift the ligand out of its center

            AttractRigidbody arb = m_receptor.m_vregion[loopregion][copy];
            arb.setTranslation(false);
            arb.setRotation(false);

            ff->AddLigand(arb); //add a copy in multicopy from the receptor
            ff->AddLigand(m_ligand); //add the ligand
            assert(ff->ProblemSize()==6);

            m_forcefields.push_back(ff);

            m_arrayOfEnergies.push_back(0);

        }

    }


    //add forcefield for main lig and main rec:
    BaseAttractForceField* ffmain = m_creator(m_paramsfile, m_cutoff);
    AttractRigidbody rec = m_receptor.m_main;
    rec.setTranslation(false);
    rec.setRotation(false);
    ffmain->AddLigand(rec);
    ffmain->AddLigand(m_ligand);


    m_forcefields.push_back(ffmain);


    if (m_verbose) std::cout << m_forcefields.size()<< " forcefields were added\n";

}







void McopForceField::calculate_weights(bool print)
{

    int countff = 0;

//loop over copies regions
    for (uint loopregion=0; loopregion < m_receptor.m_vregion.size() ; loopregion++)
    {
        if (print)
        {
            std::cout << " Region: " << loopregion << "\n";
        }

        //calculates interaction energy between receptor copies and ligand body:
        std::vector<dbl> Eik;

        for (uint copy = 0; copy < m_receptor.m_vregion[loopregion].size(); copy++)
        {
            dbl e = m_arrayOfEnergies[countff++];
            Eik.push_back(e);
        }



        const dbl RT = 0.592;

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

        m_receptor.m_weights[loopregion]=weights;

    }


}



















/** \brief calculates energy of the system
*
* this functions returns nonbonded energy of a receptor with multicopy and a ligand without copy.
*
*/
dbl McopForceField::Function(const Vdouble & v)
{


    dbl ener = 0.0 ;  //energy main-main
    dbl enercopy =0.0; //energy ligand/receptor copy


    Vdouble mainForces; //forces of lig<->main part of the receptor

    std::vector<Vdouble> arrayofforces;
    std::vector<dbl> arrayofenergies;

    int countff = 0;

    //copies /  main ligand energy

    for (uint loopregion=0; loopregion < m_receptor.m_vregion.size() ; loopregion++)
    {
        //get weights for region loopregion:
        const std::vector<dbl> & weights (m_receptor.m_weights[loopregion]) ;

        assert(m_receptor.m_vregion[loopregion].size() == m_receptor.m_weights[loopregion].size());

        for (uint copy = 0; copy < m_receptor.m_vregion[loopregion].size(); copy++)
        {

            //iteration over each copy.
            //get a reference of the forcefield
            BaseAttractForceField* ff =  m_forcefields[countff] ;
            dbl e = ff->Function(v); //get energy
            arrayofenergies.push_back(e);

            Vdouble d (6);

            ff->Derivatives(v,d); //get derivatives

            arrayofforces.push_back(d);

            dbl weight = weights[copy];

            enercopy += e * weight;
            countff++;
        }

    }


    std::swap(m_arrayOfDerivatives, arrayofforces); //
    std::swap(m_arrayOfEnergies, arrayofenergies);


    //main ligand vs main receptor:

    BaseAttractForceField* ffmain = m_forcefields[countff];
    ener = ffmain->Function(v);
    assert(ffmain->ProblemSize()==6);


    Vdouble mainderiv(6);
    ffmain->Derivatives(v,mainderiv);

    std::swap(m_mainderivatives, mainderiv);

    dbl totalener = ener + enercopy;
 
    if (m_verbose)
    {
        std::cout << "Main energy: " << ener << "  copy energy: " << enercopy << " total energy: " << totalener <<std::endl;
    }


    return totalener;



}





void McopForceField::Derivatives(const Vdouble& v, Vdouble& g)
{
    assert(g.size()==6);
    Vdouble g2(6);

    int count = 0;


    for (uint loopregion=0; loopregion < m_receptor.m_vregion.size() ; loopregion++)
    {
        const std::vector<dbl> & weights (m_receptor.m_weights[loopregion]) ;

        for (uint copy = 0; copy < m_receptor.m_vregion[loopregion].size(); copy++)
        {
            assert(m_forcefields[count]->ProblemSize()==6);
            dbl weight = weights[copy];

            for (int i=0; i<6; i++)
            {
                g2[i]+= m_arrayOfDerivatives[count][i] * weight;
            }

            count++;
        }
    }

    for(int i=0; i<6; i++)
    {
     g2[i] += m_mainderivatives[i];
    }


    std::swap(g,g2);
}




/*
void McopForceField::Derivatives(const Vdouble& v, Vdouble & g )
{

//sum the forces over x, y and z:

Coord3D ligtransForces; //translational forces for the ligand:
for(uint i=0; i<m_moved_ligand.m_main.Size(); i++)
 {
     ligtransForces += m_moved_ligand.m_main.m_forces[i];
 }


Coord3D receptortransForces;
for(uint i=0; i< m_receptor.m_main.Size(); i++)
{
receptortransForces+= m_receptor.m_main.m_forces[i];
}


for (uint i=0; i < m_receptor.m_vregion.size(); i++)
 {
   Region& ens = m_receptor.m_vregion[i];
   std::vector<dbl> & weights =  m_receptor.m_weights[i];

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
*/





} // namespace PTools
