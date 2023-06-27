//
// Created by book on 2023/6/26.
//

#ifndef STL_FRANK_QUEUE_H
#define STL_FRANK_QUEUE_H

#include "frank.h"
#include "frank_deque.h"
#include "frank_heap.h"
#include "frank_function.h"
#include "frank_vector.h"

__FRANK_NAMESPACE_BEGIN
template<typename T,typename Sequence = deque<T>>
class queue
{
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::const_reference const_reference;

protected:
    Sequence c;

public:
    bool empty()const{return c.empty();}
    size_type size()const{return c.size();}
    reference back(){return c.back();}
    const_reference back()const{return c.back();}
    reference front(){return c.front();}
    const_reference front()const{return c.front();}
    void push(const_reference x){c.push_back(x);}
    void pop(){c.pop_front();}
};

template<typename T,typename Sequence = vector<int>,typename Compare = less<typename Sequence::value_type>>
class prioroty_queue
{
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::const_reference const_reference;

protected:
    Sequence c;
    Compare comp;

public:
    prioroty_queue() = default;
    explicit prioroty_queue(const Compare& x):comp(x),c(){}

    prioroty_queue(const value_type* first,const value_type* last,
                   const Compare& x):comp(x),c(first,last){
        make_heap(first,last,comp);
    }

    prioroty_queue(const value_type* first,const value_type* last)
    :comp(),c(first,last)
    {
        make_heap(first,last);
    }

    bool empty()const{return c.empty();}
    size_type size()const{return c.size();}
    const_reference top() const{return c.front();}

    void push(const value_type& x){
        try{
            c.push_back(x);
            push_heap(c.begin(),c.end(),comp);
        }catch(...){
            c.clear();
        }
    }

    void pop(){
        try{
            pop_heap(c.begin(),c.end(),comp);
            c.pop_back();
        }catch(...){
            c.clear();
        }
    }

};


__FRANK_NAMESPACE_END

#endif //STL_FRANK_QUEUE_H
