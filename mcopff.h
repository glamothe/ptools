// $Id$
//  multicopy-forcefield
//
//



#ifndef m_MCOPFF_H_
#define m_MCOPFF_H_


#include "forcefield.h"
#include "rigidbody.h"

namespace PTools{


// typedef std::vector<AttractRigidbody> ensemble ;


class Region
{
private:

std::vector<AttractRigidbody> m_copies;

public:
/*
   void AttractEulerRotate(const dbl& phi, const dbl& ssi, const dbl& rot)
   {
      for (uint i=0; i<_copies.size(); i++)
      {
          m_copies[i].AttractEulerRotate(phi, ssi, rot);
      }

   };

   void Translate(const Coord3D& co)
   {
       for(uint i =0; i< m_copies.size(); ++i)
       {
          m_copies[i].Translate(co);
       }
   }*/


   void addCopy(const AttractRigidbody& cop){ m_copies.push_back(cop); };

   size_t size() const {return m_copies.size();};


   AttractRigidbody& operator[](uint i){return m_copies[i];};

};




class Mcoprigid //multicopy rigidbody
{

public:

    Mcoprigid();

    //using default copy operator

    void setMain(AttractRigidbody& main) ;
    void addEnsemble(const Region& reg){ m_vregion.push_back(reg); std::vector<dbl> v(reg.size()) ; m_weights.push_back(v);  };


    void AttractEulerRotate(const dbl& phi, const dbl& ssi, const dbl& rot);
    void Translate(const Coord3D& c);


    void PrintWeights();
    std::vector <std::vector<dbl> > getWeights(){return m_weights;};


private:

    AttractRigidbody m_main;
    std::vector< Region > m_vregion ;

    bool m_complete ;
    Coord3D m_center ; ///<center of mass of the main region

    std::vector< std::vector <dbl> > m_weights;

    friend class McopForceField;

};




//TODO: put this function into McopForcField to change the origin of a copy
/*    void changeLigCenter(int index, const Coord3D& tr)
    {
      this->m_centeredligand[index].Translate(tr);
      this->m_ligcenter[index] -= tr;
    }
*/


/** \brief creator of BaseAttractForceField
this function returns a BaseAttractForceField*.
It's necessary for the multicopy forcefield McopForceField
since this forcefield needs to create new AttractForceField(s) 
(either version 1 or 2)

Actually it should return 
*/
template <class T>
BaseAttractForceField* attractforceFieldCreator(const std::string& paramsfile, dbl cutoff)
{
   return new T(paramsfile, cutoff);  //very simple function isn't it ? 
}




typedef BaseAttractForceField* (*FFcreator) (const std::string&, dbl); //FFcreator is a function pointer to a function 
    //taking a const string& and a double and returning a BaseAttractForceField*


/** \brief ForceField with multicopy
needs an attract forcefield (either 1 or 2) in constructor

no normal modes yet
*/
class McopForceField: public ForceField
{

public:
    //TODO: this class leaks when creating BaseAttractForceFields without deleting pointers in a destructor

    McopForceField(FFcreator creator, const std::string paramsfile , dbl cutoff
                   ,const Mcoprigid& rec, const AttractRigidbody& lig )
        :m_creator(creator), 
         m_receptor(rec),
         m_ligand(lig),
         m_paramsfile(paramsfile),
         m_cutoff(cutoff)
         { init(); };

    ~McopForceField();


    dbl Function(const Vdouble&);
    void Derivatives(const Vdouble& v, Vdouble & g );


//     void setReceptor(const Mcoprigid& rec) { m_receptor = rec;};
//     void setLigand(const Mcoprigid& lig) { m_centered_ligand = lig;  };

    void calculate_weights(bool print=false);

    uint ProblemSize() {return 6;};
    void initMinimization(){};

    void dbgPlayWithFF();
    std::vector<BaseAttractForceField*> dbgGetFF(){return m_forcefields;}


private:


    bool m_update_weights;




//     BaseAttractForceField* (*m_creator)(const std::string&, double); //function pointer to a creator of BaseAttractForceField
    FFcreator m_creator;  //simplest declaration

    Mcoprigid m_receptor;
    AttractRigidbody m_ligand;


    void init();

    static const bool m_verbose = false;
    std::string m_paramsfile;
    dbl m_cutoff;


    std::vector<BaseAttractForceField*> m_forcefields;

    std::vector<Vdouble> m_arrayOfDerivatives;
    Vdouble m_mainderivatives;

    std::vector<dbl> m_arrayOfEnergies;



};

}//namespace PTools

#endif // m_MCOPFF_H_



