#include "forcefield.h"
#include "geometry.h"
#include "rmsd.h"


#include <fstream>
#include <math.h>  //for fabs()



//temporaire:
//convertit une chaine de caracteres std::string en un autre type
// si >> a ete correctement defini.
#include <sstream>
template<typename T>
bool from_string( const std::string & Str, T & Dest )
{
    // créer un flux à partir de la chaîne donnée
    std::istringstream iss( Str );
    // tenter la conversion vers Dest
    return (iss >> Dest) != 0;
}





namespace PTools
{



void PrintVec(const Vdouble& vec)
{
std::cout << "Vector: " ;
for(uint i=0; i<vec.size(); i++)
    std::cout << vec[i] << "  " ;

std::cout << "\n";

}


void extractExtra(Rigidbody& rig, std::vector<uint>& vCat, std::vector<double>& vCh)
{

    uint   atcategory  = 0;
    double  atcharge   = 0.0;

    for (uint i=0; i<rig.Size(); i++)
    {
        Atom at = rig.GetAtom(i);
        std::string extra = at.GetExtra();
        //std::cout << extra << std::endl;
        std::istringstream iss( extra );
        iss >> atcategory >> atcharge ;
        vCat.push_back(atcategory-1);
        vCh.push_back(atcharge);
    }


}




AttractForceField::AttractForceField(const Rigidbody& recept,const Rigidbody& lig,double cutoff)
        :m_refreceptor(recept), m_refligand(lig), m_receptor(recept), m_ligand(lig),m_savligand(lig),
        plist(recept,lig,cutoff)
{
    m_ligcenter = m_ligand.FindCenter();
    m_refligand.Translate(PTools::minus(m_ligcenter)); //center the ligand (for later Euler rotation)

    InitParams();

    for (uint i=0;i<m_refligand.Size();i++)
        m_ligforces.push_back(Coord3D()); // initialize the forces array

    // reads the "extra" informations of each atoms of ligand and receptor
    // and extracts the "atom category" from this fields
    extractExtra(m_refreceptor, m_rAtomCat, m_rAtomCharge);
    extractExtra(m_refligand,   m_lAtomCat, m_lAtomCharge);



    m_energycalled=false; //initialization
}



void AttractForceField::NumDerivatives(const Vdouble& stateVars, Vdouble& delta)
{

for(uint j=0; j<6; j++)
{

Vdouble newvars1 = stateVars;
Vdouble newvars2 = stateVars;



double h=1e-6;

newvars1[j]+=h;
double F1=Function(newvars1);
newvars2[j]-=h;
double F2=Function(newvars2);
double diff=(F1-F2)/(2*h) ;
delta[j]=diff;
}

}





double AttractForceField::Energy() 
{
ResetForces();
double energy = LennardJones() +  Electrostatic();
m_energycalled=true;
return energy;
}; 



double AttractForceField::Energy(const Vdouble& stateVars)
{
    AttractEuler(m_refligand, m_ligand, stateVars[0], stateVars[1], stateVars[2]);
    m_ligand.Translate(m_ligcenter);
    m_ligand.Translate(Coord3D(stateVars[3],stateVars[4],stateVars[5]));
    return Energy();
}


void AttractForceField::InitParams()
{
    int indice, inull;
    double rad;
    double amp;

    std::ifstream aminon("aminon.par");
    if (!aminon)
    {
        //the file cannot be opened
        std::string msg = "Forcefield.cpp: Cannot Locate file aminon.par\n";
        std::cout << msg ;
        throw msg;
    }

    while (!aminon.eof())
    {
        aminon >> indice >> rad >> amp >> inull ;
        m_rad.push_back(rad) ;
        m_amp.push_back(amp) ;
    }

    assert(m_rad.size()<40);


    //initialisation of the pre-calculated array of rc and ac
    for (uint i=0; i<=m_rad.size();i++)
        for (uint j=0; j<=m_rad.size(); j++)
        {
            m_rc[i][j]=m_amp[i]*m_amp[j]*pow((m_rad[i]+m_rad[j]),8);
            m_ac[i][j]=m_amp[i]*m_amp[j]*pow((m_rad[i]+m_rad[j]),6);
        }
}



double AttractForceField::LennardJones()
{

    double sumLJ=0.0 ;



for (uint iter=0; iter<plist.Size(); iter++)
        {

            uint ir = plist[iter].atrec;
            uint jl = plist[iter].atlig;
            assert(m_rAtomCat[ir] >= 0);
            assert(m_rAtomCat[ir] <= 39);
            assert(m_lAtomCat[jl] >= 0);
            assert(m_lAtomCat[jl] <= 39);

            double alen = m_ac[ m_rAtomCat[ir] ][ m_lAtomCat[jl] ];
            double rlen = m_rc[ m_rAtomCat[ir] ][ m_lAtomCat[jl] ];

            Coord3D dx = m_ligand.GetCoords(jl) - m_receptor.GetCoords(ir) ;
            double r2 = Norm2(dx);

            if (r2 < 0.001 ) r2=0.001;
            double rr2 = 1.0/r2;
            dx = rr2*dx;

            double rr23 = rr2*rr2*rr2 ;
            double rep =  rlen*rr2 ;
            double vlj = (rep-alen)*rr23 ;

            sumLJ += vlj;

            double fb = 6.0*vlj+2.0*(rep*rr23) ;
            Coord3D fdb = fb*dx ;
            m_ligforces[jl] = m_ligforces[jl] + fdb ;
            //receptor%forces%data(i) = receptor%forces%data(i) - fdb
        }
    return sumLJ ;
}



double AttractForceField::Electrostatic()
{
    double sumElectrostatic=0.0 ;

for (uint iter=0; iter<plist.Size(); iter++)
        {
            uint ir = plist[iter].atrec;
            uint jl = plist[iter].atlig;
            double chargeR = m_rAtomCharge[ir];
            double chargeL = m_lAtomCharge[jl];
            double charge = chargeR * chargeL * (332.053986/20.0);

            Coord3D dx = m_ligand.GetCoords(jl) - m_receptor.GetCoords(ir) ;
            double r2 = Norm2(dx);

            if (r2 < 0.001 ) r2=0.001;
            double rr2 = 1.0/r2;
            dx = rr2*dx;

            if (fabs(charge) > 0.0)
            {
                double et = charge*rr2;
                sumElectrostatic+=et;

                Coord3D fdb = (2.0*et)*dx;
                m_ligforces[jl] = m_ligforces[jl] + fdb ;
            }
        }
    return  sumElectrostatic ;
}



void AttractForceField::Gradient(const Vdouble& stateVars, Vdouble& delta)
{
    // !!!!!!!!!! WARNING: this function NEEDS Energy() to be called FIRST !!!!!!!!!!!!!!

    //std::cout << "size of stateVars:" << stateVars.size() << std::endl ;
    assert(stateVars.size()>=ProblemSize());
    assert(delta.size()>=ProblemSize());

    Trans(delta);
    Rota(stateVars[0], stateVars[1], stateVars[2], delta);

    ResetForces();
    m_energycalled = false;
    // return 1-delta:
    for(uint i=0; i<6;i++) delta[i]=-delta[i];
}


void AttractForceField::Trans(Vdouble& delta,bool print)
{
//   In this subroutine the translational force components are calculated
    double flim = 1.0e18;
    double ftr1, ftr2, ftr3, fbetr;

    ftr1=0.0;
    ftr2=0.0;
    ftr3=0.0;
    for (uint i=0;i<m_ligand.Size(); i++)
    {
        ftr1=ftr1 + m_ligforces[i].x;
        ftr2=ftr2 + m_ligforces[i].y;
        ftr3=ftr3 + m_ligforces[i].z;
    }

// force reduction, some times helps in case of very "bad" start structure
    for (uint i=0; i<3; i++)
    {
        fbetr=ftr1*ftr1 +ftr2*ftr2 +ftr3*ftr3;
        if (fbetr > flim)
        {
            ftr1=.01*ftr1;
            ftr2=.01*ftr2;
            ftr3=.01*ftr3;
        }
    }

    delta[3]=ftr1;
    delta[4]=ftr2;
    delta[5]=ftr3;

    //debug:
    if (print) std::cout <<  "translational forces: " << ftr1 <<"  "<< ftr2 <<"  " << ftr3 << std::endl;
    return ;
}



void AttractForceField::Rota(double phi,double ssi, double rot, Vdouble& delta,bool print)
{
    //delta array of doubles of dimension 6 ( 3 rotations, 3 translations)

    double  cs,cp,ss,sp,cscp,sscp,sssp,crot,srot,xar,yar,cssp,X,Y,Z ;
    double  pm[3][3];

// !c
// !c     calculates orientational force contributions
// !c     component 1: phi-angle
// !c     component 2: ssi-angle
// !c     component 3: rot-angle
// !c


    for (uint i=0; i<3;i++)
    {
        delta[i]=0.0;
        for (uint j=0;j<3;j++)
            pm[i][j]=0.0 ;
    }

    cs=cos(ssi);
    cp=cos(phi);
    ss=sin(ssi);
    sp=sin(phi);
    cscp=cs*cp;
    cssp=cs*sp;
    sscp=ss*cp;
    sssp=ss*sp;
    crot=cos(rot);
    srot=sin(rot);

    for (uint i=0; i<m_ligand.Size(); i++)
    {

        Coord3D coords = m_refligand.GetCoords(i);
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

        for (uint j=0;j<3;j++)
        {
            delta[j] += pm[0][j] * m_ligforces[i].x ;
            delta[j] += pm[1][j] * m_ligforces[i].y ;
            delta[j] += pm[2][j] * m_ligforces[i].z ;
        }
    }

    if (print) std::cout << "Rotational forces: " << delta[0] << " " << delta[1] << " " << delta[2] << std::endl;

    return;
}



void AttractForceField::ResetForces()
{
    m_ligforces = std::vector<Coord3D> (m_ligforces.size());
}




void AttractForceField::ShowEnergyParams()
{
    for (unsigned int i=0; i < m_amp.size(); i++ )
        std::cout << "m_amp[" << i+1 << "]=" << m_amp[i]  << "   m_rad[" << i+1 << "]=" << m_rad[i] << std::endl;

}



}




