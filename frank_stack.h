//
// Created by book on 2023/6/26.
//

#ifndef STL_FRANK_STACK_H
#define STL_FRANK_STACK_H
#include "frank.h"
#include "frank_deque.h"
__FRANK_NAMESPACE_BEGIN
using namespace frank;

template<typename T,typename Sequence = deque<T>>
class stack
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
    reference top(){return c.back();}
    const_reference top()const{return c.back();}
    void push(const_reference x){c.push_back(x);}
    void pop(){ c.pop_back();}
};


__FRANK_NAMESPACE_END
#endif //STL_FRANK_STACK_H
