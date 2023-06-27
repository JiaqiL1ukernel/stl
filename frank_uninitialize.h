//
// Created by book on 2023/6/16.
//

#ifndef STL_FRANK_UNINITIALIZE_H
#define STL_FRANK_UNINITIALIZE_H
#include "frank.h"
#include "frank_iterators.h"
#include "frank_algobase.h"
#include "frank_type_traits.h"
__FRANK_NAMESPACE_BEGIN
using namespace frank;
template<typename Iterator,typename T>
void construct(Iterator position,const T& x)
{
    new(position) T(x);
}

template<typename T>
inline void destroy(T* pointer)
{
    pointer->~T();
}

template<typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first,ForwardIterator last,__false_type)
{
    while(first!=last){
        destroy(&*first);
        ++first;
    }
}

template<typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first,ForwardIterator last,__true_type){}


template<typename ForwardIterator,typename T>
inline void __destroy(ForwardIterator first,ForwardIterator last,T*)
{
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;

}

template<typename ForwardIterator>
inline void destroy(ForwardIterator first,ForwardIterator last)
{
    __destroy(first,last, value_type(first));
}

template<typename InputIterator,typename ForwardIterator>
inline ForwardIterator __uninitialize_copy_aux(InputIterator first,InputIterator last,ForwardIterator result,__true_type)
{
    return copy(first,last,result);
}

template<typename InputIterator,typename ForwardIterator>
inline ForwardIterator __uninitialize_copy_aux(InputIterator first,InputIterator last,ForwardIterator result,__false_type)
{
    ForwardIterator cur = result;
    try {
        for(;first!=last;++first,++cur)
            construct(&*cur,*first);
    }catch(...) {
        destroy(result,cur);
    }


    return cur;
}

template<typename InputIterator,typename ForwardIterator,typename T>
inline ForwardIterator __uninitialize_copy(InputIterator first,InputIterator last,ForwardIterator result,T*)
{
    typedef typename __type_traits<T>::is_POD_type is_POD;
    return __uninitialize_copy_aux(first,last,result,is_POD());
}

template<typename InputIterator,typename ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result)
{
    return __uninitialize_copy(first,last,result, value_type(first));
}

//
template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,Size n,const T& t,__false_type)
{
    ForwardIterator cur = first;
    try {
        for(;n>0;--n,++cur)
            construct(&*cur,t);
    }catch (...){
        destroy(first,cur);
    }
    return cur;

}

template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,Size n,const T& t,__true_type)
{
    return fill_n(first,n,t);
}

template<typename ForwardIterator,typename Size,typename T,typename T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T& t,T1*)
{
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_fill_n_aux(first,n,t,is_POD());
}


template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& t)
{
    return __uninitialized_fill_n(first,n,t, value_type(first));
}


template<typename ForwardIterator,typename T>
inline void __uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,const T& x,__false_type){
    ForwardIterator cur = first;
    try {
        for(;cur!=last;++cur){
            construct(&*cur,x);
        }
    }catch (...){
        destroy(first,cur);
    }
}

template<typename ForwardIterator,typename T>
inline void __uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,const T& x,__true_type){
    fill(first,last,x);
}

template<typename ForwardIterator,typename T,typename T1>
inline void __uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& x,T1*){
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    __uninitialized_fill_aux(first,last,x,is_POD());
}

template<typename ForwardIterator,typename T>
inline void uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& x){
    __uninitialized_fill(first,last,x, value_type(first));
}

template<typename InputIterator,typename ForwardIterator,typename T>
inline void __uninitialized_copy_fill(InputIterator first1,InputIterator last1,
                                      ForwardIterator first2,ForwardIterator last2,
                                      const T& x)
{
    ForwardIterator mid2 = uninitialized_copy(first1,last1,first2);
    try{
        uninitialized_fill(mid2,last2,x);
    }catch(...){
        destroy(first2,mid2);
    }
}

template<typename InputIterator,typename ForwardIterator,typename T>
inline ForwardIterator __uninitialized_fill_copy(ForwardIterator result,ForwardIterator mid,
                                      const T& x,InputIterator first,InputIterator last)
{
    uninitialized_fill(result,mid,x);
    try{
        return uninitialized_copy(first,last,mid);
    }catch(...){
        destroy(result,mid);
    }
}

template<typename InputIterator1,typename InputIterator2,typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_copy(InputIterator1 first1,InputIterator1 last1,
                                                 InputIterator2 first2,InputIterator2 last2,
                                                 ForwardIterator result)
{
    ForwardIterator mid = uninitialized_copy(first1,last1,result);
    try{
        return uninitialized_copy(first2,last2,mid);
    }catch(...){
        destroy(result,mid);
    }
}


__FRANK_NAMESPACE_END
#endif //STL_FRANK_UNINITIALIZE_H
