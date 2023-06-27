//
// Created by book on 2023/6/27.
//

#ifndef STL_FRANK_HEAP_H
#define STL_FRANK_HEAP_H
#include "frank.h"
__FRANK_NAMESPACE_BEGIN

template<typename RandomIterator,typename Distance,typename T>
inline void __push_heap(RandomIterator first,Distance hole_index,
                        Distance top_index,T value)
{
    Distance parent = (hole_index - 1) / 2;
    while(top_index < hole_index && *(first+parent) < value){
        *(first + hole_index) = *(first + parent);
        hole_index = parent;
        parent = (hole_index - 1) / 2;
    }
    *(first + hole_index) = value;
}

template<typename RandomIterator,typename Distance,typename T>
inline void __push_heap_aux(RandomIterator first,RandomIterator last,
                            Distance*,T*)
{
    __push_heap(first,Distance(last-first-1),Distance(0),T(*(last-1)));
}

template<typename RandomIterator>
inline void push_heap(RandomIterator first,RandomIterator last)
{
    __push_heap_aux(first,last,difference_type(first), value_type(first));
}

template<typename RandomIterator,typename T,typename Distance>
inline void __adjust_heap(RandomIterator first,Distance hole_index,Distance len,T value)
{
    Distance top_index = hole_index;
    Distance larger_child = 2 * hole_index + 2;
    while(larger_child < len)
    {
        if(*(first + larger_child) < *(first + (larger_child -1)))
            --larger_child;
        *(first + hole_index) = *(first + larger_child);
        hole_index = larger_child;
        larger_child = 2 * hole_index + 2;
    }
    if(larger_child == len){
        *(first + hole_index) = *(first + (larger_child -1));
        hole_index = larger_child - 1;
    }
    __push_heap(first,hole_index,top_index,value);
}

template<typename RandomIterator,typename T,typename Distance>
inline void __pop_heap(RandomIterator first,RandomIterator last,RandomIterator result,
                       T value,Distance*)
{
    *result = *first;
    __adjust_heap(first,Distance(0),Distance(last-first),value);
}

template<typename RandomIterator,typename T>
inline void __pop_heap_aux(RandomIterator first,RandomIterator last,T*)
{
    __pop_heap(first,last-1,last-1,T(*(last-1)), difference_type(first));
}

template<typename RandomIterator>
inline void pop_heap(RandomIterator first,RandomIterator last)
{
    __pop_heap_aux(first,last,value_type(first));
}

template<typename RandomIterator,typename Distance,typename T,typename Compare>
inline void __push_heap(RandomIterator first,Distance hole_index,
                        Distance top_index,T value,Compare comp)
{
    Distance parent = (hole_index - 1) / 2;
    while(top_index < hole_index && comp(*(first+parent),value)){
        *(first + hole_index) = *(first + parent);
        hole_index = parent;
        parent = (hole_index - 1) / 2;
    }
    *(first + hole_index) = value;
}

template<typename RandomIterator,typename Compare,typename Distance,typename T>
inline void __push_heap_aux(RandomIterator first,RandomIterator last,
                            Compare comp,Distance*,T*)
{
    __push_heap(first,Distance((last - first) -1),Distance(0),T(*(last-1)),comp);
}

template<typename RandomIterator,typename Compare>
inline void push_heap(RandomIterator first,RandomIterator last,Compare comp)
{
    __push_heap_aux(first,last,comp, difference_type(first),value_type(first));
}

template<typename RandomIterator,typename T,typename Distance,typename Compare>
inline void __adjust_heap(RandomIterator first,Distance hole_index,Distance len,T value,Compare comp)
{
    Distance top_index = hole_index;
    Distance larger_child = 2 * hole_index + 2;
    while(larger_child < len)
    {
        if(comp(*(first + larger_child),*(first + (larger_child -1))))
            --larger_child;
        *(first + hole_index) = *(first + larger_child);
        hole_index = larger_child;
        larger_child = 2 * hole_index + 2;
    }
    if(larger_child == len){
        *(first + hole_index) = *(first + (larger_child -1));
        hole_index = larger_child - 1;
    }
    __push_heap(first,hole_index,top_index,value,comp);
}

template<typename RandomIterator,typename T,typename Distance,typename Compare>
inline void __pop_heap(RandomIterator first,RandomIterator last,RandomIterator result,
                       T value,Distance*,Compare comp)
{
    *result = *first;
    __adjust_heap(first,Distance(0),Distance(last-first),value,comp);
}

template<typename RandomIterator,typename T,typename Compare>
inline void __pop_heap_aux(RandomIterator first,RandomIterator last,T*,Compare comp)
{
    __pop_heap(first,last-1,last-1,T(*(last-1)), difference_type(first),comp);
}

template<typename RandomIterator,typename Compare>
inline void pop_heap(RandomIterator first,RandomIterator last,Compare comp)
{
    __pop_heap_aux(first,last,value_type(first),comp);
}

template<typename RandomIterator>
inline void sort_heap(RandomIterator first,RandomIterator last)
{
    while(last - first > 1)
        pop_heap(first,last--);
}

template<typename RandomIterator,typename Compare>
inline void sort_heap(RandomIterator first,RandomIterator last,Compare comp)
{
    while(last - first > 1)
        pop_heap(first,last--,comp);
}

template<typename RandomIterator,typename Distance,typename T>
inline void __make_heap(RandomIterator first,RandomIterator last,
                        Distance*,T*)
{
    Distance len = last - first;
    Distance hole_index = (len - 2)/2;
    while(1){
        __adjust_heap(first,hole_index,len,T(*(first + hole_index)));
        if(hole_index == 0) return;
        --hole_index;
    }
}

template<typename RandomIterator>
inline void make_heap(RandomIterator first,RandomIterator last)
{
    __make_heap(first,last, difference_type(first), value_type(first));
}

template<typename RandomIterator,typename Distance,typename T,typename Compare>
inline void __make_heap(RandomIterator first,RandomIterator last,
                        Distance*,T*,Compare comp)
{
    Distance len = last - first;
    if(len < 2) return;
    Distance hole_index = (len - 2)/2;
    while(1){
        __adjust_heap(first,hole_index,len,T(*(first + hole_index)),comp);
        if(hole_index == 0) return;
        --hole_index;
    }
}

template<typename RandomIterator,typename Compare>
inline void make_heap(RandomIterator first,RandomIterator last,Compare comp)
{
    __make_heap(first,last, difference_type(first), value_type(first),comp);
}
__FRANK_NAMESPACE_END
#endif //STL_FRANK_HEAP_H
