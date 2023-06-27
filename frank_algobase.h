//
// Created by book on 2023/6/16.
//

#ifndef STL_FRANK_ALGOBASE_H
#define STL_FRANK_ALGOBASE_H
#include "frank.h"
#include "frank_type_traits.h"
#include "frank_iterators.h"
#include <cstddef>
#include <string.h>
#include <algorithm>
__FRANK_NAMESPACE_BEGIN
using namespace frank;

template<typename RandomIterator,typename OutputInterator,typename Distance>
inline OutputInterator __copy_d(RandomIterator first,RandomIterator last,OutputInterator result,Distance*) {
    for (Distance n = last - first; n > 0; n--, first++, result++)
    {
        *result = *first;
    }
    return result;
}



template<typename InputIterator,typename OutputIterator>
inline OutputIterator __copy(InputIterator first,InputIterator last,OutputIterator result,input_iterator_tag)
{
    for(;first!=last;first++,result++)
    {
        *result = *first;
    }
    return result;
}

template<typename RandomIterator,typename OutputIterator>
inline OutputIterator __copy(RandomIterator first,RandomIterator last,OutputIterator result,random_iterator_tag)
{
    return __copy_d(first,last,result, distance_type(first));
}

template<typename T>
inline T* __copy_t(const T* first,const T* last,T* result,__true_type)
{
    memmove(result,first, sizeof(T)*(last-first));
    return result+(last-first);
}

template<typename T>
inline T* __copy_t(const T* first,const T* last,T* result,__false_type)
{
    return __copy_d(first,last,result, distance_type(first));
}


template<typename InputIterator,typename OutputIterator>
struct __copy_dispatch{
    OutputIterator operator()(InputIterator first,InputIterator last,OutputIterator result)
    {
        return __copy(first,last,result, iterator_category(first));
    }
};

template<typename T>
struct __copy_dispatch<T*,T*>{
    T* operator()(T* first,T* last,T* result){
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first,last,result,t());
    }
};

template<typename T>
struct __copy_dispatch<const T*,T*>{
    T* operator()(const T* first,const T* last,T* result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first,last,result,t());
    }
};

template<typename InputIterator,typename OutputIterator>
inline OutputIterator copy(InputIterator first,InputIterator last,OutputIterator result)
{
    return __copy_dispatch<InputIterator,OutputIterator>()(first,last,result);
}


//
template<typename BidirectionalIterator1,typename BidirectionalIterator2>
BidirectionalIterator2 __copy_backward
(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result)
{
    while(first!=last)
    {
        *--result = *--last;
    }
    return result;
}


template<typename T>
T* __copy_backward_t(const T* first,const T* last,T* result,__true_type)
{
    const ptrdiff_t N = last - first;
    memmove(result-N,first, sizeof(T)*N);
    return result-N;
}

template<typename T>
T* __copy_backward_t(const T* first,const T* last,T* result,__false_type)
{
    return __copy_backward(first,last,result);
}


template<typename BidirectionalIterator1,typename BidirectionalIterator2>
struct __copy_backward_dispatch
{
    BidirectionalIterator2 operator()(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result)
    {
        return __copy_backward(first,last,result);
    }
};

template<typename T>
struct __copy_backward_dispatch<T*,T*>
{
    T* operator()(T* first,T* last,T* result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_backward_t(first,last,result,t());
    }
};


template<typename T>
struct __copy_backward_dispatch<const T*,T*>
{
    T* operator()(const T* first,const T* last,T* result){
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_backward_t(first,last,result,t());
    }
};

template<typename BidirectionalIterator1,typename BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward
(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result)
{
    return __copy_backward_dispatch<BidirectionalIterator1,BidirectionalIterator2>()(first,last,result);
}

template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator fill_n(ForwardIterator first,Size n,const T& t)
{
    for(;n>0;--n,++first)
        *first = t;
    return first;
}

template<typename ForwardIterator,typename T>
void fill(ForwardIterator first,ForwardIterator last,const T& t){
    while(first!=last){
        *first = t;
        ++first;
    }
}

template<typename InputIterator1,typename InputIterator2>
bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2)
{
    for(;first1!=last1;++first1,++first2){
        if(*first1!=*first2)
            return false;
    }
    return true;
}

template<typename InputIterator1,typename InputIterator2,typename BinaryPredicate>
bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,BinaryPredicate binary_pred)
{
    for (;first1!=last1;++first1,++first2){
        if(!binary_pred(*first1,*first2))
            return false;
    }
    return true;
}

//
template <class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2) {
    for ( ; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2)
            return true;
        if (*first2 < *first1)
            return false;
    }
    return first1 == last1 && first2 != last2;
}

template <class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             Compare comp) {
    for ( ; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (comp(*first1, *first2))
            return true;
        if (comp(*first2, *first1))
            return false;
    }
    return first1 == last1 && first2 != last2;
}

inline bool
lexicographical_compare(const unsigned char* first1,
                        const unsigned char* last1,
                        const unsigned char* first2,
                        const unsigned char* last2)
{
    const size_t len1 = last1 - first1;
    const size_t len2 = last2 - first2;
    const int result = memcmp(first1, first2, std::min(len1, len2));
    return result != 0 ? result < 0 : len1 < len2;
}

inline bool lexicographical_compare(const char* first1, const char* last1,
                                    const char* first2, const char* last2)
{

    return lexicographical_compare((const signed char*) first1,
                                   (const signed char*) last1,
                                   (const signed char*) first2,
                                   (const signed char*) last2);
}

template<typename T>
inline const T& max(const T& x,const T& y)
{
    return (x>=y)?x:y;
}

template<typename T>
inline const T& min(const T& x,const T& y)
{
    return x<y?x:y;
}

template<typename T,typename Compare>
inline const T& min(const T& x,const T& y,Compare comp)
{
    return comp(x,y)?x:y;
}

template<typename T,typename Compare>
inline const T& max(const T& x,const T& y,Compare comp)
{
    return comp(y,x)?x:y;
}

//
template<typename T>
inline void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

template<typename ForwardIterator1,typename ForwardIterator2,typename T>
inline void __iter_swap(ForwardIterator1 a,ForwardIterator2 b,T*)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template<typename ForwardIterator1,typename ForwardIterator2>
inline void iter_swap(ForwardIterator1 a,ForwardIterator2 b)
{
    __iter_swap(a,b, value_type(a));
}

__FRANK_NAMESPACE_END
#endif //STL_FRANK_ALGOBASE_H
