#ifndef BASETYPES_H
#define BASETYPES_H


#include <vector>
#include <iostream>

typedef std::vector<int> Vint;
typedef std::vector<double> Vdouble;




// #warning "basetype.h: never keep that !!!!!!"
// struct Vdouble: public std::vector<double>
// {
// 
// public:
//     Vdouble(int sz):std::vector<double>(sz){};
//     Vdouble():std::vector<double>(){};
//     //Vdouble(const Vdouble& dbl){std::cout << "OPERATEUR DE RECOPIE"; }
// 
// 
//     uint size() const
//     {
//         std::cout<<"##########################\n##########\n size Appelé !!!\n";
//         return std::vector<double>::size();
//     }
// 
// 
// };



#endif




