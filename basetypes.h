#ifndef BASETYPES_H
#define BASETYPES_H


#include <vector>
#include <iostream>
#include <cassert>
#include <boost/shared_array.hpp>


//switch to choose between automatic differenciation
//or normal double arithmetic




#include "derivify.h"  //must be included in any cases

#ifdef AUTO_DIFF
typedef surreal dbl;
#else
typedef double dbl;
#endif


typedef unsigned int uint;
typedef std::vector<int> Vint;
typedef std::vector<uint> Vuint;
typedef std::vector<dbl> Vdouble;

#ifndef NDEBUG
#define Debug(func) func
#else
#define Debug(func)
#endif



// power (of int) faster much faster than pow()
// expands pow<6>(a) to a*a*a*a*a*a
template<size_t I,size_t N> struct metapow
{
    static inline dbl Pow( dbl x )
    {
        return x*metapow<I+1,N>::Pow(x);
    }
};

template<size_t N> struct metapow<N,N>
{
    static inline dbl Pow( dbl x ) { return 1; }
};

template<size_t N> dbl pow(dbl x)
{
    return metapow<0,N>::Pow(x);
}











/*! \brief 2-dimensional Object Oriented array
*
*  this class provides a 2-dimensional OO array. Memory is managed automatically.
*  The size of the array cannot be modified after creation.
*  It should be compatible with functions that needs raw pointers (with few modifications of the object).
*  WARNING: the assignemnt like 'Array2D array2=array1;' or 'Array2D array2(array1)' leads to two objects
*  sharing the same memory (this reduce the cost of copying and transmission by value,
*  like for Python objects). To make array2 independant of array1 (deep copy), use: array2.detach() after the
*  copy.
*  It should be reasonably fast for most usages
*/
template <class mytype>
class Array2D
{
public:
    Array2D(){};
    Array2D(int row, int columns)
            :m_rows(row),
            m_columns(columns)
    {
        m_size = row*columns;
        msa_data  = boost::shared_array<mytype>( new mytype[m_size]);
    }

    /// index operator (fortran-like syntax). a(0,2) gives raw 1, column 3.
    mytype& operator() (int r, int c)
    {
        assert(r<m_rows);
        assert(c<m_columns);
        return msa_data[r*m_columns+c];
    }

    const mytype& operator() (int r, int c) const
    {
        assert(r<m_rows);
        assert(c<m_columns);
        return msa_data[r*m_columns+c];
    }


    const void * id() {
        return (void *) &msa_data[0];
    }

    /// "deep copy" of the array
    void detach()
    {
        mytype * olddata = msa_data.get();
        mytype * newdata = new mytype[m_size];
        memcpy( newdata, olddata, m_size*sizeof(mytype) );
        msa_data=boost::shared_array<mytype>(newdata);
    }

    void Print()
    {
        for (uint r=0; r<m_rows; r++)
        {
            for (uint c=0; c<m_columns; c++)
                std::cout << (*this)(r,c) << "  " ;
            std::cout << std::endl;
        }
    }


private:
    boost::shared_array<mytype> msa_data;
    int m_rows;
    int m_columns;
    size_t m_size;

} ;


typedef Array2D<dbl> Matrix;



#endif




