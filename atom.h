#include <string>
#include <iostream>


#ifndef ATOM_H
#define ATOM_H

namespace PTools{

class Atom
{

    private:
    std::string type;  ///< CA, H, N, ...
    std::string residtype; ///< LEU, ARG, ...
    std::string chainId; ///< A, B, etc.
    uint idresid; ///< residue number
    uint atomnumber; ///< atom number
    double charge; ///< charge of the atom

    public:
    
    const std::string GetType() const {return type;};
    void SetType(std::string newtype) { type = newtype;}
    
     


};

};

 #endif
 
 
