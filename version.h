#ifndef VERSION_H
#define VERSION_H


#include "bzrrev.h"
#include <string>

/*! \brief PTools version class
*
* This simple class is used to define unique
* revision numbers, now relying on bazaar.
* This unique number can be used to know exactly
* which version of the library was used to perform
* a simulation.
*/
struct Version
{

//public attributes
public:
    int revnb;
    std::string revstr;
    std::string revdate;
    std::string revid;
    std::string branchnick;

//public methods
public:
    Version(); //default constructor

};



#endif
