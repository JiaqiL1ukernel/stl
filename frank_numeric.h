//
// Created by book on 2023/6/29.
//

#ifndef STL_FRANK_NUMERIC_H
#define STL_FRANK_NUMERIC_H
#include "frank.h"

__FRANK_NAMESPACE_BEGIN
using namespace frank;

template<typename T,typename InputIterator>
T accumulate(InputIterator first,InputIterator last,T init)
{
    for(;first != last;++first)
        init += *first;
    return init;
}

template<typename T,typename InputIterator,typename BinaryOperation>
T accumulate(InputIterator first,InputIterator last,T init,BinaryOperation binary_op)
{
    for(;first != last;++first)
        init = binary_op(*first,init);
    return init;
}

template<typename T,typename InputIterator1,typename InputIterator2>
T inner_product(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,T init)
{
    for(;first1 != last1;++first1,++first2)
        init = init + (*first1 * *first2);
    return init;
}

template<typename T,typename InputIterator1,typename InputIterator2,
        typename BinaryOperation1,typename BinaryOperation2>
T inner_product(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,T init,
                BinaryOperation1 binary_op1,BinaryOperation2 binary_op2)
{
    for(;first1 != last1;++first1,++first2)
        init = binary_op1(init,binary_op2(*first1,*first2));
    return init;
}

template<typename T,typename InputIterator,typename OutputIterator>
OutputIterator __partial_sum(InputIterator first,InputIterator last,OutputIterator res,T*)
{
    T value = *first;
    while(++first != last)
    {
        value = *first + value;
        *++res = value;
    }
    return ++res;
}


template<typename InputIterator,typename OutputIterator>
OutputIterator partial_sum(InputIterator first,InputIterator last,OutputIterator res)
{
    if(first == last) return res;
    *res = *first;
    return __partial_sum(first,last,res, value_type(first));
}

template<typename T,typename InputIterator,typename OutputIterator,typename BinaryOperation>
OutputIterator __partial_sum(InputIterator first,InputIterator last,
                             OutputIterator res,T*,BinaryOperation binary_op)
{
    T value = *first;
    while(++first != last)
    {
        value = binary_op(*first,value);
        *++res = value;
    }
    return ++res;
}


template<typename InputIterator,typename OutputIterator,typename BinaryOperation>
OutputIterator partial_sum(InputIterator first,InputIterator last,
                           OutputIterator res,BinaryOperation binary_op)
{
    if(first == last) return res;
    *res = *first;
    return __partial_sum(first,last,res, value_type(first),binary_op);
}

template<typename InputIterator,typename OutputIterator,typename T>
OutputIterator __adjacent_difference(InputIterator first,InputIterator last,OutputIterator res,T*)
{
    T value = *first;
    while (++first != last)
    {
        T tmp = *first;
        *++res = tmp - value;
        value = tmp;
    }
    return ++res;
}

template<typename InputIterator,typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first,InputIterator last,OutputIterator res)
{
    if(first == last) return res;
    *res = *first;
    return __adjacent_difference(first,last,res,value_type(first));
}

template<typename InputIterator,typename OutputIterator,typename T,typename BinaryOperation>
OutputIterator __adjacent_difference(InputIterator first,InputIterator last,
                                     OutputIterator res,T*,BinaryOperation binary_op)
{
    T value = *first;
    while (++first != last)
    {
        T tmp = *first;
        *++res = binary_op(tmp,value);
        value = tmp;
    }
    return ++res;
}

template<typename InputIterator,typename OutputIterator,typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first,InputIterator last,
                                   OutputIterator res,BinaryOperation binary_op)
{
    if(first == last) return res;
    *res = *first;
    return __adjacent_difference(first,last,res,value_type(first),binary_op);
}

template<typename ForwardIterator,typename T>
void iota(ForwardIterator first,ForwardIterator last,T value)
{
    while(first != last)
        *first++ = value++;
}

__FRANK_NAMESPACE_END
#endif //STL_FRANK_NUMERIC_H
