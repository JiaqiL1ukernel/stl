//
// Created by book on 2023/6/27.
//

#ifndef STL_FRANK_SLIST_H
#define STL_FRANK_SLIST_H
#include "frank.h"
__FRANK_NAMESPACE_BEGIN
using namespace frank;

struct __slist_node_base
{
    __slist_node_base* next;
};

template<typename T>
struct __slist_node:public __slist_node_base
{
    T data;
};

struct __slist_iterator_base
{
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef forward_iterator_tag iterator_category;

    __slist_node_base* node;

    __slist_iterator_base(__slist_node_base* _node):node(_node){}
    void incr(){node = node->next;}
    bool operator==(const __slist_iterator_base& x){return node==x.node;}
    bool operator!=(const __slist_iterator_base& x){return node!=x.node;}
};

template<typename T,typename Ref,typename Ptr>
struct __slist_iterator:public __slist_iterator_base
{
    typedef __slist_iterator<T,T&,T*> iterator;
    typedef __slist_iterator<T,const T&,const T*> const_iterator;
    typedef __slist_iterator<T,Ref,Ptr> self;

    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __slist_node<T> list_node;

    __slist_iterator(): __slist_iterator_base(0){}
    __slist_iterator(const iterator& x): __slist_iterator_base(x.node){}
    __slist_iterator(list_node* x): __slist_iterator_base(x){}

    reference operator*(){return ((list_node*)node)->data;}
    pointer operator->(){return &operator*();}

    self& operator++(){
        incr();
        return *this;
    }

    self operator++(int){
        self tmp = *this;
        incr();
        return tmp;
    }

};

inline ptrdiff_t* difference_type(const __slist_iterator_base&)
{
    return nullptr;
}

inline forward_iterator_tag iterator_category(const __slist_iterator_base&)
{
    return forward_iterator_tag();
}

template<typename T,typename Ref,typename Ptr>
T* value_type(const __slist_iterator<T,Ref,Ptr>&)
{
    return nullptr;
}

inline size_t __slist_size(__slist_node_base* node)
{
    size_t result = 0;
    for(;node!= nullptr;node = node->next)
        ++result;
    return result;
}

template<typename T,typename Alloc = alloc>
class slist
{
public:
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef const T& const_reference;
    typedef const T* const_pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;

    typedef __slist_iterator<T,T&,T*> iterator;
    typedef __slist_iterator<T,const T&,const T*> const_iterator;

private:
    typedef __slist_node_base list_node_base;
    typedef __slist_iterator_base iterator_base;
    typedef __slist_node<T> list_node;
    typedef simple_alloc<list_node,Alloc> node_allocator;

    static list_node* create_node(const_reference x){
        list_node* node = node_allocator::allocate();
        try{
            construct(&node->data,x);
            node->next = nullptr;
        }catch (...){
            node_allocator::deallocate(node);
        }
        return node;
    }
};

__FRANK_NAMESPACE_END
#endif //STL_FRANK_SLIST_H
