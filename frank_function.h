//
// Created by book on 2023/6/27.
//

#ifndef STL_FRANK_FUNCTION_H
#define STL_FRANK_FUNCTION_H
#include "frank.h"
__FRANK_NAMESPACE_BEGIN
using namespace frank;

template<typename Arg1,typename Arg2,typename Result>
struct binary_function
{
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};


template<typename T>
struct less : public binary_function<T,T,bool>
{
    bool operator()(const T& x,const T& y){return x < y;}
};

__FRANK_NAMESPACE_END
#endif //STL_FRANK_FUNCTION_H
