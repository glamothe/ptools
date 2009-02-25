// $Id$
/****************************************************************************
 *   Copyright (C) 2009   Adrien Saladin                                    *
 *   adrien.saladin@gmail.com                                               *
 *   Copyright (C) 2009   Chantal Prevost                                   *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 ***************************************************************************/



#ifndef _CONSTRAINT_H_
#define _CONSTRAINT_H_

#include "attractrigidbody.h"


namespace PTools
{

struct Constraint
{
int lig1, lig2, at1 , at2;
double rstk;
};



Constraint CreateDefaultConstraint(const AttractRigidbody& rec,const AttractRigidbody& lig, uint recnb, uint lignb);

}

#endif //_CONSTRAINT_H_


