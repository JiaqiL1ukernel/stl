//
// Created by book on 2023/6/16.
//

#ifndef STL_FRANK_VECTOR_H
#define STL_FRANK_VECTOR_H
#include <memory>
#include "frank_iterators.h"
#include <ext/alloc_traits.h>
#include "frank_algobase.h"
#include "frank_uninitialize.h"
#include <algorithm>
__FRANK_NAMESPACE_BEGIN
using namespace frank;
template<typename T,typename Alloc = alloc>
class vector{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef simple_alloc<T,Alloc> data_allocator;
    typedef reverse_iterator<const_iterator,value_type,const_reference,difference_type> _const_reverse_iterator;
    typedef reverse_iterator<iterator,value_type,reference,difference_type> _reverse_iterator;

protected:
    iterator start;
    iterator finish;
    iterator end_of_storage;
    void insert_aux(iterator position,const T& v);
    void deallocate()
    {
        if(start) data_allocator::deallocate(start,end_of_storage-start);
    }

    void fill_initialize(size_type n,const T& value)
    {
        start = allocate_and_fill(n,value);
        finish = start+n;
        end_of_storage = finish;
    }
    iterator allocate_and_fill(size_type n,const T& value)
    {
        iterator start = data_allocator::allocate(n);
        uninitialized_fill_n(start,n,value);
        return start;
    }

    iterator allocate_and_copy(size_type n,const_iterator first,const_iterator last)
    {
        iterator start = data_allocator::allocate(n);
        uninitialized_copy(first,last,start);
        return start;
    }
public:
    iterator begin(){return start;}
    const_iterator begin()const{return start;}
    iterator end(){return finish;}
    const_iterator end()const{return finish;}
    _reverse_iterator rbegin(){return _reverse_iterator(end());}
    _const_reverse_iterator rbegin()const{return _const_reverse_iterator(end());}
    _reverse_iterator rend(){return _reverse_iterator(begin());}
    _const_reverse_iterator rend()const{return _const_reverse_iterator(begin());}
    size_type max_size()const{return size_type (size_t(-1)/sizeof(value_type));}
    size_type size()const{return size_type(end()-begin());}
    bool empty()const{return start==finish;}
    size_type capacity()const{return size_type(end_of_storage-start);}
    reference operator[](size_type n){return *(begin()+n);}
    const_reference operator[](size_type n)const{return *(begin()+n);}
    vector<T,Alloc>& operator=(const vector<T,Alloc>& v);
    reference front(){return *begin();}
    const_reference front()const{return *begin();}
    reference back(){return *(end()-1);}
    const_reference back()const{return *(end()-1);}
    iterator insert(iterator position,const T& t);
    void insert(iterator position,const_iterator first,const_iterator last);
    void insert (iterator pos, size_type n, const T& x);
    void insert (iterator pos, int n, const T& x) {
        insert(pos, (size_type) n, x);
    }
    void insert (iterator pos, long n, const T& x) {
        insert(pos, (size_type) n, x);
    }
    iterator insert(iterator position)
    {
        return insert(position,T());
    }

    void push_back(const T& value)
    {
        if(finish!=end_of_storage){
            construct(finish,value);
            ++finish;
        }else
            insert_aux(finish,value);

    }

    void pop_back(){
        destroy(finish);
        --finish;
    }

    iterator erase(iterator position)
    {
        if(position!=finish-1)
            copy(position+1,finish,position);
        --finish;
        destroy(finish);
        return position;
    }

    iterator erase(iterator first,iterator last)
    {
        iterator i = copy(last,finish,first);
        destroy(i,finish);
        finish = i;
        return first;
    }

    void resize(size_type new_size,const T& t)
    {
        if(new_size<size())
            erase(begin()+new_size,end());
        else
            insert(end(),new_size-size(),t);
    }

    void resize(size_type new_size){ resize(new_size,T());}

    void clear(){ erase(begin(),end());}

public:
    vector():start(nullptr),finish(nullptr),end_of_storage(nullptr){}
    vector(size_type n,const T& value){ fill_initialize(n,value);}
    vector(int n,const T& value){ fill_initialize(n,value);}
    vector(long n,const T& value){ fill_initialize(n,value);}
    explicit vector(size_type n){ fill_initialize(n,T());}
    vector(const_iterator first,const_iterator last);
    vector(const vector<T,Alloc>& v){
        start = allocate_and_copy(v.end()-v.begin(),v.begin(),v.end());
        finish = start+(v.end()-v.begin());
        end_of_storage = finish;
    }
    ~vector()
    {
        destroy(start,finish);
        deallocate();
    }

};

template<typename T>
ptrdiff_t* difference_type(T*)
{
    return nullptr;
}

template<typename T>
T* value_type(T*)
{
    return nullptr;
}

template<typename T,typename Alloc>
void vector<T,Alloc>::insert_aux(iterator position,const T& v) {
    if (finish != end_of_storage) {
        construct(finish, *(finish - 1));
        ++finish;
        T v_copy = v;
        copy_backward(position, finish - 2, finish - 1);
        *position = v_copy;
    } else {
        const size_type old_size = size();
        const size_type len = old_size == 0 ? 1 : old_size * 2;
        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = uninitialized_copy(start, position, new_start);
        new(new_finish)T(v);
        ++new_finish;
        new_finish = uninitialized_copy(position, finish, new_finish);
        destroy(start, finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = start + len;
    }
}

template<typename T,typename Alloc>
vector<T,Alloc>::vector(const_iterator first,const_iterator last)
{
    size_type n = 0;
    n = distance(first,last);
    start = allocate_and_copy(n,first,last);
    finish = start+(last-first);
    end_of_storage = finish;
}

template<typename T,typename Alloc>
vector<T,Alloc>& vector<T,Alloc>::operator=(const vector<T, Alloc>& v)
{
    iterator tmp;
    if(this!=&v){
        if(capacity()<v.size()){
            tmp = allocate_and_copy(v.end()-v.begin(),v.begin(),v.end());
            destroy(start,finish);
            deallocate();
            start = tmp;
            end_of_storage = start+v.size();
        }else if(size()>=v.size()){
            tmp = copy(v.begin(),v.end(),start);
            destroy(tmp,finish);
        } else{
            copy(v.begin(),v.begin()+size(),start);
            uninitialized_copy(v.begin()+size(),v.end(),start+size());
        }
        finish = start+v.size();
    }
    return *this;
}

template<typename T,typename Alloc>
typename vector<T,Alloc>::iterator vector<T,Alloc>::insert(iterator position, const T &t)
{
    if(finish!=end_of_storage && position==finish){
        construct(position,t);
        ++finish;
    }else
        insert_aux(position,t);
    return position;
}

template<typename T,typename Alloc>
void vector<T,Alloc>::insert(iterator position, const_iterator first,const_iterator last)
{
    if(first==last)
        return;
    const size_type n = distance(first,last);
    if((end_of_storage - finish)>=n){
        const size_type elems_after = finish - position;
        iterator old_finish = finish;
        if(elems_after > n){
            uninitialized_copy(finish-n,finish,finish);
            finish += n;
            copy_backward(position,old_finish-n,old_finish);
            copy(first,last,position);
        }else{
            uninitialized_copy(position,finish,finish+(n-elems_after));
            uninitialized_copy(last-(n-elems_after),last,finish);
            copy(first,first+elems_after,position);
            finish += n;
        }

    }else{
        size_type old_size = size();
        size_type len = old_size+std::max(old_size,n);
        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        try {
            new_finish = uninitialized_copy(start,position,new_finish);
            new_finish = uninitialized_copy(first,last,new_finish);
            new_finish = uninitialized_copy(position,finish,new_finish);
        }catch (...){
            destroy(new_start,new_finish);
            data_allocator ::deallocate(new_start,len);
        }
        destroy(start,finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = start+len;
    }
}

template<typename T,typename Alloc>
void vector<T,Alloc>::insert(iterator position,size_type n,const T &t)
{
    if(n<=0) return;
    if(size_type(end_of_storage-finish)>=n){
        T t_copy = t;
        const size_type elems_after = finish-position;
        iterator old_finish = finish;
        if(elems_after>n){
            uninitialized_copy(finish-n,finish,finish);
            finish += n;
            copy_backward(position,old_finish-n,old_finish);
            fill(position,position+n,t_copy);
        }else{
            uninitialized_copy(finish-elems_after,finish,finish+(n-elems_after));
            uninitialized_fill_n(finish,n-elems_after,t_copy);
            fill(position,position+elems_after,t_copy);
            finish += n;
        }

    }else{
        size_type old_size = capacity();
        size_type len = old_size+std::max(old_size,n);
        iterator new_start = data_allocator ::allocate(len);
        iterator new_finish = new_start;
        try {
            new_finish = uninitialized_copy(start,position,new_start);
            new_finish = uninitialized_fill_n(new_finish,n,t);
            new_finish = uninitialized_copy(position,finish,new_finish);
        }catch (...){
            destroy(new_start,new_finish);
            data_allocator ::deallocate(new_start,len);
        }
        destroy(start,finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = start+len;
    }
}

template<typename T,typename Alloc>
inline bool operator==(const vector<T,Alloc>& v1,const vector<T,Alloc>& v2)
{
    return v1.size()==v2.size() && equal(v1.begin(),v1.begin(),v2.begin());
}

template<typename T,typename Alloc>
bool operator<(const vector<T,Alloc>& v1,const vector<T,Alloc>& v2)
{
    return lexicographical_compare(v1.begin(),v1.end(),v2.begin(),v2.end());
}

__FRANK_NAMESPACE_END
#endif //STL_FRANK_VECTOR_H
