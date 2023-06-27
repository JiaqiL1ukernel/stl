//
// Created by book on 2023/6/15.
//

#ifndef STL_FRANK_ITERATOR_TRAITS_H
#include <cstddef>
#include "frank_type_traits.h"
#include "frank.h"
__FRANK_NAMESPACE_BEGIN
using namespace frank;
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag:public input_iterator_tag{};
struct bidirectional_iterator_tag:public forward_iterator_tag{};
struct random_iterator_tag:public bidirectional_iterator_tag{};

template<
        typename Category,
        typename Value_type ,
        typename Difference_type = ptrdiff_t ,
        typename Reference = Value_type&,
        typename Pointer = Value_type*
        >
struct iterator{
    typedef Category iterator_category;
    typedef Value_type value_type;
    typedef Difference_type difference_type;
    typedef Reference reference;
    typedef Pointer pointer;
};

template<typename Iterator>
struct iterator_traits{
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::iterator_category iterator_category;
};

template<typename T>
struct iterator_traits<const T*>{
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef random_iterator_tag iterator_category;
};

template<typename T>
struct iterator_traits<T*>{
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef random_iterator_tag iterator_category;
};

template<typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator& i)
{
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category ();
}

template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&)
{
    typedef typename iterator_traits<Iterator>::difference_type* difference_type;
    return difference_type(0);
}

template<typename Iterator>
inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(nullptr);
}

template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first,InputIterator last,input_iterator_tag)
{
    typename iterator_traits<InputIterator>::value_type n = 0;
    while(first!=last){
        n++;
        first++;
    }
    return n;
}

template<typename RandomIterator>
inline typename iterator_traits<RandomIterator>::difference_type
__distance(RandomIterator first,RandomIterator last,random_iterator_tag)
{
    return last-first;
}

template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type
distance(Iterator first, Iterator last)
{
    return __distance(first,last, iterator_category(first));
}

template<typename InputIterator,typename Distance>
inline void __advance(InputIterator& i,Distance n,input_iterator_tag)
{
    while (n--)i++;
}

template<typename BidirectionIterator , typename Distance>
inline void __advance(BidirectionIterator& i,Distance n,bidirectional_iterator_tag)
{
    if(n>=0)
        while(n--)i++;
    else
        while (n++)i--;
}

template<typename RandomIterator,typename Distance>
inline void __advance(RandomIterator& i, Distance n,random_iterator_tag)
{
    i+=n;
}

template<typename Iterator,typename Distance>
inline void advance(Iterator& i,Distance n)
{
    __advance(i,n, iterator_category(i));
}

template<typename T,typename Distance>
struct input_iterator
{
    typedef input_iterator_tag iterator_category;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef Distance difference_type;
};

struct output_iterator
{
    typedef output_iterator iterator_category;
    typedef void value_type;
    typedef void pointer;
    typedef void reference;
    typedef void difference_type;
};

template <class T, class Distance> struct forward_iterator {
    typedef forward_iterator_tag iterator_category;
    typedef T                    value_type;
    typedef Distance             difference_type;
    typedef T*                   pointer;
    typedef T&                   reference;
};


template <class T, class Distance> struct bidirectional_iterator {
    typedef bidirectional_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef Distance                   difference_type;
    typedef T*                         pointer;
    typedef T&                         reference;
};

template <class T, class Distance> struct random_access_iterator {
    typedef random_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef Distance                   difference_type;
    typedef T*                         pointer;
    typedef T&                         reference;
};

template<typename BidirectionalIterator,typename T,typename Reference,typename Distance>
class reverse_bidirectional_iterator
{
    typedef reverse_bidirectional_iterator<BidirectionalIterator,T,Reference,Distance> self;
protected:
    BidirectionalIterator current;
public:
    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Reference reference;
    typedef T* pointer;
    typedef Distance difference_type;

    reverse_bidirectional_iterator() = default;
    explicit reverse_bidirectional_iterator(BidirectionalIterator x):current(x){}
    BidirectionalIterator base()const{return current;}
    reference operator*(){
        BidirectionalIterator tmp = current;
        return *--tmp;
    }

    pointer operator->(){return &(operator*());}

    self& operator++()
    {
        --current;
        return *this;
    }

    self operator++(int){
        self tmp = *this;
        --current;
        return tmp;
    }

    self& operator--(){
        ++current;
        return *this;
    }

    self operator--(int){
        self tmp = *this;
        ++current;
        return tmp;
    }
};

template<typename BidirectionalIterator,typename T,typename Reference,typename Distance>
inline bidirectional_iterator_tag iterator_category
(const reverse_bidirectional_iterator<BidirectionalIterator,T,Reference,Distance>&)
{
    return bidirectional_iterator_tag();
}

template<typename BidirectionalIterator,typename T,typename Reference,typename Distance>
inline T* value_type
(const reverse_bidirectional_iterator<BidirectionalIterator,T,Reference,Distance>&)
{
    return nullptr;
}

template<typename BidirectionalIterator,typename T,typename Reference,typename Distance>
inline Distance* difference_type
(const reverse_bidirectional_iterator<BidirectionalIterator,T,Reference,Distance>&)
{
    return nullptr;
}

template<typename BidirectionalIterator,typename T,typename Reference,typename Distance>
inline bool operator==
                (const reverse_bidirectional_iterator<BidirectionalIterator,T,Reference,Distance>& x,
                 const reverse_bidirectional_iterator<BidirectionalIterator,T,Reference,Distance>& y)
{
    return x.base() == y.base();
}



template<typename RandomIterator,typename T,typename Reference = T&,typename Distance = ptrdiff_t>
class reverse_iterator
{
protected:
    typedef reverse_iterator<RandomIterator,T,Reference,Distance> self;
public:
    typedef random_iterator_tag iterator_category;
    typedef T value_type;
    typedef T* pointer;
    typedef Reference reference;
    typedef Distance difference_type;

protected:
    RandomIterator current;

    reverse_iterator() = default;
    explicit reverse_iterator(RandomIterator x):current(x){}
    RandomIterator base()const{return current;}
    reference operator*(){
        RandomIterator tmp = current;
        return *--tmp;
    }
    pointer operator->(){return &(operator*());}
    self& operator--(){
        ++current;
        return *this;
    }

    self operator--(int ){
        self tmp = *this;
        ++current;
        return tmp;
    }

    self& operator++(){
        --current;
        return *this;
    }

    self operator++(int){
        self tmp = *this;
        --current;
        return tmp;
    }

    self& operator+=(Distance n){
        current-=n;
        return *this;
    }

    self operator+(Distance n){
        return self(current-n);
    }

    self & operator-=(Distance n){
        current+=n;
        return *this;
    }

    self operator-(Distance n){
        return self(current+n);
    }

    T operator[](Distance n){
        return *(*this+n);
    }
};

template<typename RandomIterator,typename T,typename Reference ,typename Distance>
inline random_iterator_tag iterator_category
(const reverse_iterator<RandomIterator,T,Reference,Distance>&){
    return random_iterator_tag();
}

template<typename RandomIterator,typename T,typename Reference ,typename Distance>
inline T* value_type
(const reverse_iterator<RandomIterator,T,Reference,Distance>&){
    return nullptr;
}

template<typename RandomIterator,typename T,typename Reference ,typename Distance>
inline Distance* difference_type
(const reverse_iterator<RandomIterator,T,Reference,Distance>&){
    return nullptr;
}

template<typename RandomIterator,typename T,typename Reference ,typename Distance>
inline bool operator==
        (const reverse_iterator<RandomIterator,T,Reference,Distance>& x,
         const reverse_iterator<RandomIterator,T,Reference,Distance>& y){
    return x.base() == y.base();
}

template<typename RandomIterator,typename T,typename Reference ,typename Distance>
inline bool operator<
        (const reverse_iterator<RandomIterator,T,Reference,Distance>& x,
         const reverse_iterator<RandomIterator,T,Reference,Distance>& y){
    return x.base() < y.base();
}

template<typename RandomIterator,typename T,typename Reference ,typename Distance>
inline Distance operator-
        (const reverse_iterator<RandomIterator,T,Reference,Distance>& x,
         const reverse_iterator<RandomIterator,T,Reference,Distance>& y){
    return y.base() - x.base();
}

template<typename RandomIterator,typename T,typename Reference ,typename Distance>
inline reverse_iterator<RandomIterator,T,Reference,Distance> operator+
(Distance n,const reverse_iterator<RandomIterator,T,Reference,Distance>& x){
    return reverse_iterator<RandomIterator,T,Reference,Distance>(x.base()-n);
}



__FRANK_NAMESPACE_END



#endif //STL_FRANK_ITERATOR_TRAITS_H
#define STL_FRANK_ITERATOR_TRAITS_H
