//
// Created by book on 2023/6/30.
//

#ifndef STL_FRANK_ALGO_H
#define STL_FRANK_ALGO_H
#include "frank.h"
__FRANK_NAMESPACE_BEGIN
using namespace frank;

template<typename InputIterator,typename Function>
Function for_each(InputIterator first,InputIterator last,Function f)
{
    for(;first != last;++first)
        f(*first);
    return f;
}

template<typename InputIterator,typename T>
InputIterator find(InputIterator first,InputIterator last,const T& x)
{
    for(;first != last;++first){
        if(*first == x)
            break;
    }
    return first;
}

template<typename InputIterator,typename Predicate>
InputIterator find_if(InputIterator first,InputIterator last,Predicate pred)
{
    while(first != last && !pred(*first)) ++first;
    return first;
}

template<typename InputIterator>
InputIterator adjacent_find(InputIterator first,InputIterator last)
{
    if(first == last) return last;
    InputIterator next = first;
    while (++next != last){
        if(*next == *first) return first;
        first = next;
    }
    return last;
}

template<typename InputIterator,typename BinaryPredicate>
InputIterator adjacent_find(InputIterator first,InputIterator last,BinaryPredicate bianry_op)
{
    if(first == last) return last;
    InputIterator next = first;
    while (++next != last){
        if(bianry_op(*first,*next)) return first;
        first = next;
    }
    return last;
}

template<typename InputIterator,typename T,typename Size>
void count(InputIterator first,InputIterator last,const T& x,Size& n)
{
    for(;first != last;++first){
        if(*first == x) ++n;
    }
}

template<typename InputIterator,typename Predicate,typename Size>
void count_if(InputIterator first,InputIterator last,Predicate pred,Size& n)
{
    for(;first != last;++first){
        if(pred(*first)) ++n;
    }
}

template<typename ForwardIterator1,typename ForwardIterator2,
        typename Distance1,typename Distance2>
inline ForwardIterator1 __search(ForwardIterator1 first1,ForwardIterator1 last1,
                                         ForwardIterator2 first2,ForwardIterator2 last2,
                                         Distance1*,Distance2*)
{
    Distance1 d1 = distance(first1,last1);
    Distance2 d2 = distance(first2,last2);
    ForwardIterator1 current1 = first1;
    ForwardIterator2 current2 = first2;

    if(d1 < d2) return last1;

    while (current2 != last2){
        if(*current1 ==  *current2){
            ++current1;
            ++current2;
        }else{
            if(d1 == d2) return last1;
            else{
                current1 = ++first1;
                current2 = first2;
                --d1;
            }
        }
    }
    return first1;
}


template<typename ForwardIterator1,typename ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1,ForwardIterator1 last1,
                               ForwardIterator2 first2,ForwardIterator2 last2)
{
    return __search(first1,last1,first2,last2,difference_type(first1), difference_type(first2));
}

template<typename ForwardIterator1,typename ForwardIterator2,
        typename Distance1,typename Distance2,typename BinaryPredicate>
inline ForwardIterator1 __search(ForwardIterator1 first1,ForwardIterator1 last1,
                                 ForwardIterator2 first2,ForwardIterator2 last2,
                                 Distance1*,Distance2*,BinaryPredicate binary_pred)
{
    Distance1 d1 = distance(first1,last1);
    Distance2 d2 = distance(first2,last2);
    ForwardIterator1 current1 = first1;
    ForwardIterator2 current2 = first2;

    if(d1 < d2) return last1;

    while (current2 != last2){
        if(binary_pred(*current1,*current2)){
            ++current1;
            ++current2;
        }else{
            if(d1 == d2) return last1;
            else{
                current1 = ++first1;
                current2 = first2;
                --d1;
            }
        }
    }
    return first1;
}


template<typename ForwardIterator1,typename ForwardIterator2,typename BinaryPredicate>
inline ForwardIterator1 search(ForwardIterator1 first1,ForwardIterator1 last1,
                               ForwardIterator2 first2,ForwardIterator2 last2,BinaryPredicate binary_pred)
{
    return __search(first1,last1,first2,last2,difference_type(first1), difference_type(first2),binary_pred);
}

template<typename ForwardIterator,typename Integer,typename T>
ForwardIterator search_n(ForwardIterator first,ForwardIterator last,Integer count,const T& value)
{
    if(count <= 0) return first;
    first = find(first,last,value);

    while(first != last){
        Integer n = count - 1;
        ForwardIterator i = first;
        ++i;
        while(i != last && n>0 && *i == value){
            ++i;
            --n;
        }
        if(n != 0){
            first = find(i,last,value);
        }else
            return first;
    }
    return last;
}

template<typename ForwardIterator,typename Integer,typename T,typename BinaryPredicate>
ForwardIterator search_n(ForwardIterator first,ForwardIterator last,
                         Integer count,const T& value,BinaryPredicate binary_pred)
{
    if(count <= 0) return first;
    while(first != last){
        if(binary_pred(*first,value)) break;
        ++first;
    }

    while(first != last){
        Integer n = count - 1;
        ForwardIterator i = first;
        ++i;
        while(i != last && n>0 && binary_pred(*i,value)){
            ++i;
            --n;
        }
        if(n != 0){
            while(i != last){
                if(binary_pred(*i,value)) break;
                ++i;
            }
            first = i;
        }else
            return first;
    }
    return last;
}

template<typename ForwardIterator1,typename ForwardIterator2>
ForwardIterator2 range_swap(ForwardIterator1 first1,ForwardIterator1 last1,
                            ForwardIterator2 first2)
{
    for(;first1 != last1;++first1,++first2)
        iter_swap(first1,first2);
    return first2;
}

template<typename InputIterator,typename OutputIterator,typename UnaryOperation>
OutputIterator transform(InputIterator first,InputIterator last,
                         OutputIterator res,UnaryOperation unary_op)
{
    for(;first != last;++first,++res)
        *res = unary_op(*first);
    return res;
}

template<typename InputIterator1,typename InputIterator2,typename OutputIterator,typename BinaryOperation>
OutputIterator transform(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,
                         OutputIterator res,BinaryOperation bianry_op)
{
    for(;first1 != last1;++first1,++first2,++res)
        *res = bianry_op(*first1,*first2);
    return res;
}

template<typename ForwardIterator,typename T>
void replace(ForwardIterator first,ForwardIterator last,
                        const T& old_value,const T& new_value)
{
    for(;first != last;++first)
        if(*first == old_value) *first = new_value;
}

template<typename ForwardIterator,typename T,typename Predicate>
void replace_if(ForwardIterator first,ForwardIterator last,
                Predicate pred,const T& new_value)
{
    for(;first != last;++first)
        if(pred(*first)) *first = new_value;
}

template<typename InputIterator,typename T,typename OutputIterator>
OutputIterator replace_copy(InputIterator first,InputIterator last,OutputIterator res,
                  const T& old_value,const T& new_value)
{
    for(;first != last;++first,++res)
        *res = *first == old_value? new_value : *first;
    return res;
}

template<typename InputIterator,typename T,typename OutputIterator,typename Predicate>
OutputIterator replace_copy_if(InputIterator first,InputIterator last,OutputIterator res,
                     const T& new_value,Predicate pred)
{
    for(;first != last;++first)
        *res = pred(*first)? new_value : *first;
    return res;
}

template<typename ForwardIterator,typename Generator>
void generate(ForwardIterator first,ForwardIterator last,Generator gen)
{
    for(;first != last;++first)
        *first = gen();
}

template<typename OutputIterator,typename Generator,typename Size>
OutputIterator generate_n(OutputIterator first,Size n,Generator gen)
{
    for(;n > 0;--n,++first)
        *first = gen();
    return first;
}

template<typename InputIterator,typename OutputIterator,typename T>
OutputIterator remove_copy(InputIterator first,InputIterator last,
                           OutputIterator res,const T& value)
{
    for(;first != last;++first)
    {
        if(*first != value){
            *res = *first;
            ++res;
        }
    }
    return res;
}

template<typename InputIterator,typename OutputIterator,typename Predicate>
OutputIterator remove_copy_if(InputIterator first,InputIterator last,
                              OutputIterator res,Predicate pred)
{
    for(;first != last;++first)
    {
        if(!pred(*first)){
            *res = *first;
            ++res;
        }
    }
    return res;
}

template<typename ForwardIterator,typename T>
ForwardIterator remove(ForwardIterator first,ForwardIterator last,const T& value)
{
    first = find(first,last,value);
    if(first == last) return first;
    ForwardIterator next = first;
    return remove_copy(++next,last,first,value);
}

template<typename ForwardIterator,typename Predicate>
ForwardIterator remove_if(ForwardIterator first,ForwardIterator last,
                          Predicate pred)
{
    while(first != last){
        if(pred(*first)) break;
        ++first;
    }
    if(first == last) return first;

    ForwardIterator next = first;
    return remove_copy_if(++next,last,first,pred);
}

template<typename InputIterator,typename OutputIterator,typename T>
inline OutputIterator __unique_copy(InputIterator first,InputIterator last,
                                    OutputIterator res,T*)
{
    T value = *first;
    *res = value;
    while(++first != last){
        if(value != *first){
            value = *first;
            *++res = value;
        }
    }
    return ++res;
}


template<typename InputIterator,typename OutputIterator>
inline OutputIterator __unique_copy(InputIterator first,InputIterator last,
                                    OutputIterator res,output_iterator_tag)
{
    return __unique_copy(first,last,res, value_type(first));
}

template<typename InputIterator,typename OutputIterator>
inline OutputIterator __unique_copy(InputIterator first,InputIterator last,
                                    OutputIterator res,forward_iterator_tag)
{
    *res = *first;
    while(++first != last)
        if(*first != *res) *++res = *first;
    return ++res;
}

template<typename InputIterator,typename OutputIterator>
inline OutputIterator unique_copy(InputIterator first,InputIterator last,
                                  OutputIterator res)
{
    if(first == last) return res;
    return __unique_copy(first,last,res, iterator_category(res));
}

template<typename InputIterator,typename OutputIterator,typename T,typename BinaryPredicate>
inline OutputIterator __unique_copy(InputIterator first,InputIterator last,OutputIterator res,
                                    T*,BinaryPredicate binary_pred)
{
    T value = *first;
    *res = value;
    while(++first != last){
        if(!binary_pred(*first,value)){
            value = *first;
            *++res = value;
        }
    }
    return ++res;
}


template<typename InputIterator,typename OutputIterator,typename BinaryPredicate>
inline OutputIterator __unique_copy(InputIterator first,InputIterator last,OutputIterator res,
                                    output_iterator_tag,BinaryPredicate binary_pred)
{
    return __unique_copy(first,last,res, value_type(first),binary_pred);
}

template<typename InputIterator,typename OutputIterator,typename BinaryPredicate>
inline OutputIterator __unique_copy(InputIterator first,InputIterator last,OutputIterator res,
                                    forward_iterator_tag,BinaryPredicate binary_pred)
{
    *res = *first;
    while(++first != last)
        if(!binary_pred(*first,*res)) *++res = *first;
    return ++res;
}

template<typename InputIterator,typename OutputIterator,typename BinaryPredicate>
inline OutputIterator unique_copy(InputIterator first,InputIterator last,
                                  OutputIterator res,BinaryPredicate binary_pred)
{
    if(first == last) return res;
    return __unique_copy(first,last,res, iterator_category(res),binary_pred);
}

template<typename ForwardIterator>
ForwardIterator unique(ForwardIterator first,ForwardIterator last)
{
    first = adjacent_find(first,last);
    return unique_copy(first,last,first);
}

template<typename ForwardIterator,typename BinaryPredicate>
ForwardIterator unique(ForwardIterator first,ForwardIterator last,BinaryPredicate binary_pred)
{
    first = adjacent_find(first,last,binary_pred);
    return unique_copy(first,last,first,binary_pred);
}

template<typename RandomIterator>
inline void __reverse(RandomIterator first,RandomIterator last,random_iterator_tag)
{
    while(first < last)
        iter_swap(first++,--last);
}

template<typename BidirectionalIterator>
inline void __reverse(BidirectionalIterator first,BidirectionalIterator last,bidirectional_iterator_tag)
{
    while (1)
    {
        if(first == last || first == --last) return;
        iter_swap(first++,last);
    }
}

template<typename BidirectionalIterator>
inline void reverse(BidirectionalIterator first,BidirectionalIterator last)
{
    __reverse(first,last, iterator_category(first));
}

template<typename BidirectionalIterator,typename OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first,BidirectionalIterator last,
                            OutputIterator res)
{
    while(first != last){
        --last;
        *res = *last;
        ++res;
    }
    return res;
}

template<typename ForwardIterator,typename OutputIterator>
OutputIterator rotate_copy(ForwardIterator first,ForwardIterator middle,
                           ForwardIterator last,OutputIterator res)
{
    return copy(first,middle,copy(middle,last,res));
}

template <class ForwardIterator, class Distance>
void __rotate(ForwardIterator first, ForwardIterator middle,
              ForwardIterator last, Distance*, forward_iterator_tag) {
    for (ForwardIterator i = middle; ;) {
        iter_swap(first, i);
        ++first;
        ++i;
        if (first == middle) {
            if (i == last) return;
            middle = i;
        }
        else if (i == last)
            i = middle;
    }
}

template <class BidirectionalIterator, class Distance>
void __rotate(BidirectionalIterator first, BidirectionalIterator middle,
              BidirectionalIterator last, Distance*,
              bidirectional_iterator_tag) {
    reverse(first, middle);
    reverse(middle, last);
    reverse(first, last);
}

template <class EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n)
{
    while (n != 0) {
        EuclideanRingElement t = m % n;
        m = n;
        n = t;
    }
    return m;
}

template <class RandomAccessIterator, class Distance, class T>
void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last,
                    RandomAccessIterator initial, Distance shift, T*) {
    T value = *initial;
    RandomAccessIterator ptr1 = initial;
    RandomAccessIterator ptr2 = ptr1 + shift;
    while (ptr2 != initial) {
        *ptr1 = *ptr2;
        ptr1 = ptr2;
        if (last - ptr2 > shift)
            ptr2 += shift;
        else
            ptr2 = first + (shift - (last - ptr2));
    }
    *ptr1 = value;
}

template <class RandomAccessIterator, class Distance>
void __rotate(RandomAccessIterator first, RandomAccessIterator middle,
              RandomAccessIterator last, Distance*,
              random_iterator_tag) {
    Distance n = __gcd(last - first, middle - first);
    while (n--)
        __rotate_cycle(first, last, first + n, middle - first,
                       value_type(first));
}

template <class ForwardIterator>
inline void rotate(ForwardIterator first, ForwardIterator middle,
                   ForwardIterator last) {
    if (first == middle || middle == last) return;
    __rotate(first, middle, last, distance_type(first),
             iterator_category(first));
}

template<typename RandomIterator,typename Distance>
inline void __random_shuffle(RandomIterator first,RandomIterator last,Distance*)
{
    if(first == last) return;
    for(RandomIterator i = first+1;i != last;++i)
        iter_swap(i,first + Distance(lrand48() % ((i - first) + 1)));
}

template<typename RandomIterator>
inline void random_shuffle(RandomIterator first,RandomIterator last)
{
    __random_shuffle(first,last, difference_type(first));
}

template<typename RandomIterator,typename Distance,typename RandomNumberGenerator>
inline void __random_shuffle(RandomIterator first,RandomIterator last,Distance*,
                             RandomNumberGenerator& rand)
{
    if(first == last) return;
    for(RandomIterator i = first+1;i != last;++i)
        iter_swap(i,first + rand((i - first) + 1));
}

template<typename RandomIterator,typename RandomNumberGenerator>
inline void random_shuffle(RandomIterator first,RandomIterator last,RandomNumberGenerator& rand)
{
    __random_shuffle(first,last, difference_type(first),rand);
}

template <class ForwardIterator, class OutputIterator, class Distance>
OutputIterator random_sample_n(ForwardIterator first, ForwardIterator last,
                               OutputIterator out, const Distance n)
{
    Distance remaining = 0;
    distance(first, last, remaining);
    Distance m = min(n, remaining);

    while (m > 0) {

        if (lrand48() % remaining < m) {
            *out = *first;
            ++out;
            --m;
        }

        --remaining;
        ++first;
    }
    return out;
}

template <class ForwardIterator, class OutputIterator, class Distance,
        class RandomNumberGenerator>
OutputIterator random_sample_n(ForwardIterator first, ForwardIterator last,
                               OutputIterator out, const Distance n,
                               RandomNumberGenerator& rand)
{
    Distance remaining = 0;
    distance(first, last, remaining);
    Distance m = min(n, remaining);

    while (m > 0) {
        if (rand(remaining) < m) {
            *out = *first;
            ++out;
            --m;
        }

        --remaining;
        ++first;
    }
    return out;
}

template <class InputIterator, class RandomAccessIterator, class Distance>
RandomAccessIterator __random_sample(InputIterator first, InputIterator last,
                                     RandomAccessIterator out,
                                     const Distance n)
{
    Distance m = 0;
    Distance t = n;
    for ( ; first != last && m < n; ++m, ++first)
        out[m] = *first;

    while (first != last) {
        ++t;
        Distance M = lrand48() % t;
        if (M < n)
            out[M] = *first;
        ++first;
    }

    return out + m;
}

template <class InputIterator, class RandomAccessIterator,
        class RandomNumberGenerator, class Distance>
RandomAccessIterator __random_sample(InputIterator first, InputIterator last,
                                     RandomAccessIterator out,
                                     RandomNumberGenerator& rand,
                                     const Distance n)
{
    Distance m = 0;
    Distance t = n;
    for ( ; first != last && m < n; ++m, ++first)
        out[m] = *first;

    while (first != last) {
        ++t;
        Distance M = rand(t);
        if (M < n)
            out[M] = *first;
        ++first;
    }

    return out + m;
}

template <class InputIterator, class RandomAccessIterator>
inline RandomAccessIterator
random_sample(InputIterator first, InputIterator last,
              RandomAccessIterator out_first, RandomAccessIterator out_last)
{
    return __random_sample(first, last, out_first, out_last - out_first);
}

template <class InputIterator, class RandomAccessIterator,
        class RandomNumberGenerator>
inline RandomAccessIterator
random_sample(InputIterator first, InputIterator last,
              RandomAccessIterator out_first, RandomAccessIterator out_last,
              RandomNumberGenerator& rand)
{
    return __random_sample(first, last, out_first, rand, out_last - out_first);
}

template<typename BidirectionalIterator,typename Predicate>
BidirectionalIterator partition(BidirectionalIterator first,BidirectionalIterator last,
                                Predicate pred)
{
    while(true){
        while (true){
            if(first == last) return first;
            else if(pred(*first))
                ++first;
            else
                break;
        }
        --last;
        while (true){
            if(first == last) return first;
            else if(!pred(*last))
                --last;
            else
                break;
        }
        iter_swap(first,last);
        ++first;
    }
}



__FRANK_NAMESPACE_END
#endif //STL_FRANK_ALGO_H
