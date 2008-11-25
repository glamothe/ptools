// $Id$
#ifndef VERSION_H
#define VERSION_H


#include "svnrev.h"
#include <string>


struct Version
{

int revnb;
std::string revstr;
std::string revdate;
std::string revstamp;


Version(); //default constructor

};



#endif
