//
// Created by book on 2023/6/19.
//

#ifndef STL_FRANK_LIST_H
#define STL_FRANK_LIST_H
#include "frank.h"
#include <cstddef>

__FRANK_NAMESPACE_BEGIN
using namespace frank;
template<typename T>
struct __list_node
{
    T data;
    void* next;
    void* prev;
};

template<typename T,typename Ref,typename Ptr>
struct __list_iterator
{
    typedef __list_iterator<T,T&,T*> iterator;
    typedef __list_iterator<T,const T&,const T*> const_iterator;
    typedef __list_iterator<T,Ref,Ptr> self;

    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef __list_node<T>* link_type;

    link_type node;

    __list_iterator() = default;
    __list_iterator(link_type x):node(x){}
    __list_iterator(const iterator& x):node(x.node){}

    bool operator==(const self& x){return node==x.node;}
    bool operator!=(const self& x){return !operator==(x);}
    reference operator*()const{return node->data;}
    pointer operator->()const{return &(operator*());}
    self& operator++(){node=(link_type)node->next;return *this;}
    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self& operator--(){node=(link_type)node->prev;return *this;}
    self operator--(int){
        self tmp = *this;
        --*this;
        return tmp;
    }
};

template<typename T,typename Ref,typename Ptr>
inline bidirectional_iterator_tag iterator_category(const __list_iterator<T,Ref,Ptr>)
{
    return bidirectional_iterator_tag();
}

template<typename T,typename Ref,typename Ptr>
inline T* value_type(const __list_iterator<T,Ref,Ptr>)
{
    return nullptr;
}

template<typename T,typename Ref,typename Ptr>
inline ptrdiff_t* difference_type(const __list_iterator<T,Ref,Ptr>)
{
    return nullptr;
}

template<typename T,typename Alloc = alloc>
class list
{
protected:
    typedef __list_node<T> list_node;
    typedef simple_alloc<list_node,Alloc> node_allocator;
    typedef void* void_pointer;

public:
    typedef __list_iterator<T,T&,T*> iterator;
    typedef __list_iterator<T,const T&,const T*> const_iterator;

public:
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef const T& const_reference;
    typedef const T* const_pointer;
    typedef list_node* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef reverse_bidirectional_iterator<iterator,value_type,reference,difference_type> reverse_iterator;
    typedef reverse_bidirectional_iterator<const_iterator,value_type,const_reference,difference_type> const_reverse_iterator;

protected:
    link_type get_node(){return node_allocator::allocate();}
    void put_node(link_type node){node_allocator::deallocate(node);}
    link_type create_node(const T& x)
    {
        link_type p = get_node();
        try{
            construct(&p->data,x);
        }
        catch(...){
            put_node(p);
        }
        return p;
    }

    void destroy_node(link_type p)
    {
        destroy(&p->data);
        put_node(p);
    }

    void empty_initialize(){
        node = get_node();
        node->next = node;
        node->prev = node;
    }

    void fill_initialize(size_type n,const T& t)
    {
        empty_initialize();
        try {
            insert(begin(),n,t);
        }catch (...){
            clear();
            put_node(node);
        }
    }

    void range_initialize(const T* first,const T* last)
    {
        empty_initialize();
        try {
            insert(begin(),first,last);
        }catch (...){
            clear();
            put_node(node);
        }
    }

    void range_initialize(const_iterator first,const_iterator last)
    {
        empty_initialize();
        try {
            insert(begin(),first,last);
        }catch (...){
            clear();
            put_node(node);
        }
    }

protected:
    link_type node;

public:
    list(){empty_initialize();}
    list(size_type n,const T& t){fill_initialize(n,t);}
    list(int n,const T& t){ fill_initialize(n,t);}
    list(long n,const T& t){ fill_initialize(n,t);}
    explicit list(size_type n){ fill_initialize(n,T());}
    list(const_iterator first,const_iterator last){range_initialize(first,last);}
    list(const T* first,const T* last){ range_initialize(first,last);}
    list(const list<T,Alloc>& l){ range_initialize(l.begin(),l.end());}
    ~list(){clear();
        put_node(node);}

public:
    iterator begin(){return (link_type)node->next;}
    const_iterator begin()const{return (link_type)node->next;}
    iterator end(){return node;}
    const_iterator end()const{return node;}
    reverse_iterator rbegin(){return reverse_iterator(end());}
    const_reverse_iterator rbegin()const{return const_reverse_iterator(end());}
    reverse_iterator rend(){return reverse_iterator(begin());}
    const_reverse_iterator rend()const{return const_reverse_iterator(begin());}
    bool empty()const{return node==node->next;}
    size_type size()const{return distance(begin(),end());}
    size_type max_size()const{return size_t(-1);}
    reference front(){return *begin();}
    const_reference front()const{return *begin();}
    reference back(){return *--end();}
    const_reference back()const{return *--end();}
    void swap(list<T,Alloc>& l){ frank::swap(node,l.node);}
    iterator insert(iterator position,const T& t)
    {
        link_type tmp = create_node(t);
        tmp->next = position.node;
        tmp->prev = position.node->prev;
        position.node->prev = tmp;
        (link_type (tmp->prev))->next = tmp;
        return tmp;
    }
    iterator insert(iterator position){return insert(position,T());}
    void insert(iterator position,size_type n,const T& t);
    void insert(iterator position,int n,const T& t);
    void insert(iterator position,long n,const T& t);

    void insert(iterator position,const_iterator first,const_iterator last);
    void insert(iterator position,const T* first,const T* last);
    void push_back(const T& t){ insert(end(),t);}
    void push_front(const T& t){ insert(begin(),t);}

    iterator erase(iterator position)
    {
        if(position==end())return nullptr;
        link_type pre_node = (link_type)position.node->prev;
        link_type next_node = (link_type)position.node->next;
        pre_node->next = next_node;
        next_node->prev = pre_node;
        destroy_node(position.node);
        return next_node;
    }

    iterator erase(iterator first,iterator last);
    void resize(size_type new_size,const T& t);
    void resize(size_type new_size){ resize(new_size,T());}
    void clear();

    void pop_back(){
        if(empty())return;
        erase(--end());}
    void pop_front(){
        if(empty())return;
        erase(begin());}

    list<T,Alloc>& operator=(const list<T,Alloc>& x);

    template<typename U,typename Ulloc>
    friend bool operator==(const list<U,Ulloc>& x,const list<U,Ulloc>& y);

protected:
    void transfer(iterator position,iterator first,iterator last)
    {
        if(position == last)return;
        (*(link_type((*last.node).prev))).next = position.node;
        (*(link_type((*first.node).prev))).next = last.node;
        (*(link_type((*position.node).prev))).next = first.node;
        link_type tmp = link_type((*position.node).prev);
        (*position.node).prev = (*last.node).prev;
        (*last.node).prev = (*first.node).prev;
        (*first.node).prev = tmp;
    }

public:
    void unique();
    void merge(list& x);
    void reverse();
    void sort();
    void splice(iterator position,list &t){
        if(!t.empty()){
            transfer(position,t.begin(),t.end());
        }
    }

    void splice(iterator position,list &t,iterator i)
    {
        iterator j = i;
        ++j;
        if(position==i || position==j)return;
        transfer(position,i,j);
    }

    void splice(iterator position,iterator first,iterator last){
        if(first==last)return;
        transfer(position,first,last);
    }

    void remove(const T& t)
    {
        iterator first = begin();
        iterator last = end();
        while(first!=last){
            iterator next = first;
            ++next;
            if(*first == t)
                erase(first);
            first = next;
        }
    }



};


template<typename T,typename Alloc>
void list<T,Alloc>::insert(iterator position,size_type n,const T& t)
{
    for(;n>0;--n){
        insert(position,t);
    }
}
template<typename T,typename Alloc>
void list<T,Alloc>::insert(iterator position,const_iterator first,const_iterator last)
{
    for(;first!=last;++first){
        insert(position,*first);
    }
}

template<typename T,typename Alloc>
void list<T,Alloc>::insert(iterator position,const T* first,const T* last)
{
    for(;first!=last;++first){
        insert(position,*first);
    }
}

template<typename T,typename Alloc>
typename list<T,Alloc>::iterator list<T,Alloc>::erase(iterator first,iterator last)
{
    for(;first != last;++first)
        erase(first);
    return last;
}

template<typename T,typename Alloc>
void list<T,Alloc>::resize(size_type new_size,const T& t)
{
    iterator i = begin();
    size_type len = 0;
    for(;i != end()&& len<new_size;++i,++len);
    if(len == new_size)
        erase(i,end());
    else
        insert(end(),new_size-len,t);
}

template<typename T,typename Alloc>
void list<T,Alloc>::clear()
{
    link_type cur = node;
    while(cur!=node){
        link_type tmp = cur;
        cur = (link_type)cur->next;
        destroy_node(tmp);
    }
    node->next = node;
    node->prev = node;
}

template<typename T,typename Alloc>
list<T,Alloc>& list<T,Alloc>::operator=(const list<T, Alloc> &x)
{
    if(this != &x){
        iterator first1 = begin();
        iterator last1 = end();
        const_iterator first2 = x.begin();
        const_iterator last2 = x.end();
        for(;first1!=last1 && first2!=last2 ; ++first1,++first2)
            *first1 = *first2;
        if(first2 == last2)
            erase(first1,last1);
        else
            insert(first1,first2,last2);
    }
    return *this;
}

template<typename T,typename Alloc>
bool operator==(const list<T,Alloc>& x,const list<T,Alloc>& y)
{
    typedef typename list<T,Alloc>::const_iterator  const_iterator;
    const_iterator first1 = x.begin();
    const_iterator first2 = y.begin();
    for(;first1!=x.end() && first2!=y.end() ; ++first1,++first2){
        if(*first1 != *first2)
            return false;
    }
    return first1==x.end() && first2==y.end();
}

template<typename T,typename Alloc>
void list<T,Alloc>::unique()
{
    if(empty())return;
    iterator first = begin();
    iterator last = end();
    iterator next = first;
    while(++next!=last){
        if(*first == *next)
            erase(next);
        else
            first = next;
        next = first;
    }
}

template<typename T,typename Alloc>
void list<T,Alloc>::merge(list<T,Alloc> &x)
{
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = x.begin();
    iterator last2 = x.end();
    while (first1!=last1 && first2!=last2)
    {
        if(*first1>*first2){
            iterator next = first2;
            transfer(first1,first2,++next);
            first2 = next;
        }else{
            ++first1;
        }
    }
    if(first2!=last2)
        transfer(last1,first2,last2);
}


template<typename T,typename Alloc>
void list<T,Alloc>::reverse()
{
    if(empty() || ((link_type)(node->next))->next == node)return;
    iterator first = begin();
    while(first!=end()){
        iterator old = first;
        ++first;
        transfer(begin(),old,first);
    }

}

template<typename T,typename Alloc>
void list<T,Alloc>::sort()
{
    if (node->next == node || link_type(node->next)->next == node) return;
    list<T, Alloc> carry;
    list<T, Alloc> counter[64];
    int fill = 0;
    while (!empty()) {
        carry.splice(carry.begin(), *this, begin());
        int i = 0;
        while(i < fill && !counter[i].empty()) {
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);
        if (i == fill) ++fill;
    }

    for (int i = 1; i < fill; ++i) counter[i].merge(counter[i-1]);
    swap(counter[fill-1]);
}

__FRANK_NAMESPACE_END

#endif //STL_FRANK_LIST_H
