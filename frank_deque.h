//
// Created by book on 2023/6/21.
//

#ifndef STL_FRANK_DEQUE_H
#define STL_FRANK_DEQUE_H
#include "frank.h"

__FRANK_NAMESPACE_BEGIN
using namespace frank;

inline size_t __deque_buf_size(size_t n,size_t sz){
    return n!=0?n:(sz<512?size_t(512/sz):size_t(1));
}

template<typename T,typename Ref,typename Ptr,size_t BufSiz>
struct __deque_iterator
{
    typedef __deque_iterator<T,T&,T*,BufSiz> iterator;
    typedef __deque_iterator<T,const T&,const T*,BufSiz> const_iterator;
    typedef __deque_iterator<T,Ref,Ptr,BufSiz> self;

    typedef random_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef T** map_pointer;

    static inline size_t buf_size(){return __deque_buf_size(BufSiz, sizeof(T));}

    T* first;
    T* last;
    T* cur;
    map_pointer node;

    __deque_iterator():first(nullptr),last(nullptr),cur(nullptr),node(nullptr){}
    __deque_iterator(T* x,map_pointer y):first(*y),last(*y+buf_size()),cur(x),node(y){}
    __deque_iterator(const iterator& x):first(x.first),last(x.last),cur(x.cur),node(x.node){}

    reference operator*()const{return *cur;}
    pointer operator->()const{return &(operator*());}
    void set_node(map_pointer new_node){
        node = new_node;
        first = *new_node;
        last = first+ difference_type(buf_size());
    }

    self& operator++(){
        ++cur;
        if(cur==last){
            set_node(node+1);
            cur = first;
        }
        return *this;
    }

    self operator++(int){
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--(){
        if(cur==first){
            set_node(node-1);
            cur = last;
        }
        --cur;
        return *this;
    }

    self operator--(int){
        self tmp = *this;
        --*this;
        return tmp;
    }

    self& operator+=(difference_type n){
        difference_type offset = n+(cur-first);
        if(offset>=0 && offset< difference_type(buf_size()))
            cur += n;
        else{
            difference_type node_offset =
                    offset>0?offset/ difference_type(buf_size())
                    :-((-offset-1)/ difference_type(buf_size()))-1;
            set_node(node+node_offset);
            cur = first+(offset- difference_type(buf_size())*node_offset);
        }
        return *this;
    }

    self operator+(difference_type n)const
    {
        self tmp = *this;
        return tmp+=n;
    }

    self& operator-=(difference_type n){
        return operator+=(-n);
    }

    self operator-(difference_type n)const{
        self tmp = *this;
        return tmp-=n;
    }

    reference operator[](difference_type n)const{
        if(n<last-cur){
            return *(*this+n);
        }
        return *(last-1);
    }

    bool operator==(const self& x)const{
        return cur == x.cur;
    }

    bool operator!=(const self& x)const{
        return !(*this==x);
    }

    bool operator<(const self& x)const{
        return (node==x.node)?(cur<x.cur):(node<x.node);
    }

    difference_type operator-(const self& x)const{
        return difference_type(buf_size())*(node-x.node-1)+(cur-first)+(x.last-x.cur);
    }

};

template<typename T,typename Ref,typename Ptr,size_t BufSiz>
inline random_iterator_tag iterator_category(const __deque_iterator<T,Ref,Ptr,BufSiz>&){
    return random_iterator_tag();
}

template<typename T,typename Ref,typename Ptr,size_t BufSiz>
inline T* value_type(const __deque_iterator<T,Ref,Ptr,BufSiz>&){
    return nullptr;
}

template<typename T,typename Ref,typename Ptr,size_t BufSiz>
inline ptrdiff_t* difference_type(const __deque_iterator<T,Ref,Ptr,BufSiz>&){
    return nullptr;
}


template<typename T,typename Alloc = alloc,size_t BufSiz = 0 >
class deque
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef __deque_iterator<T,reference,pointer,BufSiz> iterator;

    typedef __deque_iterator<T,const_reference,const_pointer,BufSiz> const_iterator;
    typedef reverse_iterator<const_iterator,T,const_reference,difference_type> __const_reverse_iterator;
    typedef reverse_iterator<iterator,T,reference,difference_type> __reverse_iterator;

protected:
    typedef pointer* map_pointer;
    typedef simple_alloc<T,Alloc> data_allocator;
    typedef simple_alloc<T*,Alloc> map_allocator;
    static size_type buf_size(){return __deque_buf_size(BufSiz,sizeof(T));}
    static size_type initial_buf_size(){return 8;}

protected:
    iterator start;
    iterator finish;
    map_pointer map;
    size_type map_size;

public:
    iterator begin(){return start;}
    const_iterator begin()const{return start;}
    iterator end(){return finish;}
    const_iterator end()const{return finish;}

    __reverse_iterator rbegin(){return __reverse_iterator(end());}
    __const_reverse_iterator rbegin()const{return __reverse_iterator(end());}
    __reverse_iterator rend(){return __reverse_iterator(begin());}
    __const_reverse_iterator rend()const{return __const_reverse_iterator(begin());}

    reference operator[](size_type n){return start[difference_type(n)];}
    const_reference operator[](size_type n)const{return start[difference_type(n)];}

    reference front(){return *start;}
    const_reference front()const{return *start;}
    reference back(){
        iterator tmp = finish;
        return *--tmp;
    }
    const_reference back()const{
        iterator tmp = finish;
        return *--tmp;
    }

    size_type size()const{return finish-start;}
    size_type max_size()const{return size_t(-1);}

    bool empty()const{return start==finish;}

protected:
    pointer allocate_node(){return data_allocator::allocate(buf_size());}
    void deallocate_node(pointer p){data_allocator::deallocate(p,buf_size());}
    void destroy_node_at_front(iterator start_before);
    void destroy_node_at_back(iterator finish_after);
    void create_map_and_nodes(size_type num_elems);
    void fill_initialize(size_type n,const_reference x);
    void destroy_map_and_nodes(){
        for(map_pointer cur=start.node;cur<=finish.node;++cur){
            deallocate_node(*cur);
        }
        map_allocator::deallocate(map,map_size);
    }

public:
    deque(){ create_map_and_nodes(0);}
    deque(const deque& x){
        create_map_and_nodes(x.finish-x.start);
        try {
            uninitialized_copy(x.start,x.finish,start);
        }catch (...){
            destroy_map_and_nodes();
        }
    }
    deque(size_type n,const_reference x){ fill_initialize(n,x);}
    deque(int n,const_reference x){ fill_initialize(n,x);}
    deque(long n,const_reference x){ fill_initialize(n,x);}
    explicit deque(size_type n){ fill_initialize(n,T());}

    deque(const_iterator first,const_iterator last){
        create_map_and_nodes(last-first);
        try {
            uninitialized_copy(first,last,start);
        }catch(...){
            destroy_map_and_nodes();
        }
    }

    deque(const_pointer first,const_pointer last){
        create_map_and_nodes(last-first);
        try{
            uninitialized_copy(first,last,start);
        }catch(...){
            destroy_map_and_nodes();
        }
    }

    ~deque(){
        destroy(start,finish);
        destroy_map_and_nodes();
    }

    deque& operator=(const deque& x){
        if(&x == this)return *this;
        const size_type len = size();
        if(len>=x.size()){
            erase(copy(x.start,x.finish,start),finish);
        }else{
            const_iterator mid = x.start + len;
            copy(x.start,mid,start);
            insert(finish,mid,x.finish);
        }
        return *this;
    }

    void swap(deque& x)
    {
        swap(start,x.start);
        swap(finish,x.finish);
        swap(map_size,x.map_size);
        swap(map,x.map);
    }

    void push_back(const_reference x){
        if(finish.cur!=finish.last-1){
            construct(finish.cur,x);
            ++finish.cur;
        } else
            push_back_aux(x);
    }

    void push_front(const_reference x){
        if(start.cur!=start.first)
            construct(--start.cur,x);
        else
            push_front_aux(x);
    }

    void pop_back(){
        if(finish.cur != finish.first)
            destroy(--finish.cur);
        else
            pop_back_aux();
    }

    void pop_front(){
        if(start.cur != start.last-1){
            destroy(start.cur);
            ++start.cur;
        }
        else
            pop_front_aux();
    }


protected:
    void push_back_aux(const_reference x);
    void push_front_aux(const_reference x);
    void pop_back_aux();
    void pop_front_aux();
    iterator insert_aux(iterator pos,const_reference x);
    void insert_aux(iterator pos,size_type n,const_reference x);
    void insert_aux(iterator position,const_iterator first,const_iterator last,size_type n);
    void insert_aux(iterator pos,const value_type* first,const value_type* last,size_type n);

    iterator reserve_elements_at_front(size_type n){
        difference_type vancacies = start.first - start.cur;
        if(vancacies < n)
            new_elements_at_front(n-vancacies);
        return start-n;
    }

    iterator reserve_elements_at_back(size_type n){
        difference_type vancancies = finish.last - finish.cur-1;
        if(n > vancancies)
            new_elements_at_back(n-vancancies);
        return finish+n;
    }

    void new_elements_at_front(size_type n);

    void new_elements_at_back(size_type n);
public:
    iterator insert(iterator pos,const_reference x){
        if(pos.cur==start.cur){
            push_front(x);
            return start;
        }else if(pos.cur == finish.cur){
            push_back(x);
            return finish-1;
        }else
            return insert_aux(pos,x);
    }

    iterator insert(iterator pos){return insert(pos,T());}

    void insert(iterator pos,size_type n,const_reference x);

    void insert(iterator pos,int n,const_reference x){ insert(pos,n,x);}

    void insert(iterator pos,long n,const_reference x){ insert(pos,n,x);}

    void insert(iterator pos,const_iterator first,const_iterator last);

    void insert(iterator pos,const value_type* first,const value_type* last);

    iterator erase(iterator pos);

    iterator erase(iterator first,iterator last);

    void resize(size_type new_size,const_reference x){
        size_type len = size();
        if(new_size < len)
            erase(start+new_size,finish);
        else
            insert(finish,new_size-len,x);
    }

    void resize(size_type new_size){ resize(new_size, value_type());}

    void clear();

protected:
    void reserve_map_at_back(size_type nodes_to_add = 1){
        if(nodes_to_add+1>map_size-(finish.node-map))
            reallocate_map(nodes_to_add, false);
    }

    void reserve_map_at_front(size_type nodes_to_add = 1){
        if(nodes_to_add+1>start.node-map)
            reallocate_map(nodes_to_add, true);
    }

    void reallocate_map(size_type nodes_to_add,bool add_at_front);
};

template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::insert(iterator pos, const_iterator first,const_iterator last)
{
    size_type n = last - first;
    if(pos.cur == start.cur){
        iterator new_start = reserve_elements_at_front(n);
        try{
            uninitialized_copy(first,last,new_start);
            start = new_start;
        }catch(...){
            destroy_node_at_front(new_start);
        }
    }else if(pos.cur == finish.cur){
        iterator new_finish = reserve_elements_at_back(n);
        try{
            uninitialized_copy(first,last,finish);
            finish = new_finish;
        }catch(...){
            destroy_node_at_back(new_finish);
        }
    }else{
        insert_aux(pos,first,last,n);
    }
}

template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::insert(iterator pos,const value_type* first,const value_type* last)
{
    size_type n = last - first;
    if(pos.cur == start.cur){
        iterator new_start = reserve_elements_at_front(n);
        try{
            uninitialized_copy(first, last, new_start);
            start = new_start;
        }catch(...){
            destroy_node_at_front(new_start);
        }
    }else if(pos.cur == finish.cur){
        iterator new_finish = reserve_elements_at_back(n);
        try{
            uninitialized_copy(first, last, finish);
            finish = new_finish;
        }catch(...){
            destroy_node_at_back(new_finish);
        }
    }else{
        insert_aux(pos,first,last,n);
    }
}

template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::insert(iterator pos,size_type n,const_reference x)
{
    if(pos.cur == start.cur){
        iterator new_start = reserve_elements_at_front(n);
        uninitialized_fill(new_start,start,x);
        start = new_start;
    }else if(pos.cur == finish.cur){
        iterator new_finish = reserve_elements_at_back(n);
        uninitialized_fill(finish,new_finish,x);
        finish = new_finish;
    }else
        insert_aux(pos,n,x);
}

template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::destroy_node_at_front(iterator start_before)
{
    for(map_pointer node=start_before.node;node<start.node;++node)
    {
        deallocate_node(*node);
    }
}


template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::destroy_node_at_back(iterator finish_after)
{
    for(map_pointer node=finish_after.node;node>finish.node;--node)
    {
        deallocate_node(*node);
    }
}


template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::new_elements_at_front(size_type new_elements)
{
    size_type new_node = (new_elements+buf_size()-1)/buf_size();
    reserve_map_at_front(new_node);
    size_type  i;
    try {
        for(i=1;i<=new_node;++i)
            *(start.node-i) = allocate_node();
    }catch(...){
        for(size_type j=1;j<i;++j)
            deallocate_node(*(start.node-j));
    }
}

template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::new_elements_at_back(size_type new_elements)
{
    size_type new_node = (new_elements+buf_size()-1)/buf_size();
    reserve_map_at_back(new_node);
    size_type i;
    try {
        for(i=1;i<=new_node;++i)
            *(finish.node+i) = allocate_node();
    }catch(...){
        for(size_type j=1;j<i;++j)
            deallocate_node(*(finish.node+j));
    }
}


template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::reallocate_map(size_type nodes_to_add, bool add_at_front)
{
    size_type old_num_nodes = finish.node-start.node+1;
    size_type new_num_nodes = old_num_nodes + nodes_to_add;
    map_pointer new_nstart;
    if(map_size > 2*new_num_nodes){
        new_nstart = map + (map_size-new_num_nodes)/2 + (add_at_front?nodes_to_add:0);
        if(new_nstart < start.node){
            copy(start.node,finish.node+1,new_nstart);
        }else{
            copy_backward(start.node,finish.node+1,new_nstart+old_num_nodes);
        }
    }else{
        size_type new_map_size = map_size + max(map_size,nodes_to_add)+2;
        map_pointer new_map = map_allocator::allocate(new_map_size);
        new_nstart = new_map + (new_map_size-new_num_nodes)/2 + (add_at_front?nodes_to_add:0);
        copy(start.node,finish.node+1,new_nstart);
        map_allocator::deallocate(start.node,old_num_nodes);
        map = new_map;
        map_size = new_map_size;
    }
    start.set_node(new_nstart);
    finish.set_node(new_nstart+old_num_nodes-1);
}

template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::insert_aux(iterator pos,const_iterator first,
                                       const_iterator last,size_type n)
{
    const difference_type elems_before = pos - start;
    size_type length = size();
    if(elems_before < length/2){
        iterator new_start = reserve_elements_at_front(n);
        iterator old_start = start;
        pos = start + elems_before;
        try{
            if(elems_before >= difference_type(n)){
                iterator start_n = start + difference_type(n);
                uninitialized_copy(start,start_n,new_start);
                start = new_start;
                copy(start_n,pos,old_start);
                copy(first,last,pos- difference_type(n));
            }else{
                const_iterator mid = first + (difference_type(n) - elems_before);
                __uninitialized_copy_copy(start,pos,first,mid,new_start);
                start = new_start;
                copy(mid,last,old_start);
            }
        }catch(...){
            destroy_node_at_front(new_start);
        }
    }else{
        iterator new_finish = reserve_elements_at_back(n);
        iterator old_finish = finish;
        const difference_type elems_after = length - elems_before;
        pos = finish - elems_after;
        try{
            if(elems_after >= difference_type(n)){
                iterator finish_n = finish - difference_type(n);
                uninitialized_copy(finish_n,finish,finish);
                finish = new_finish;
                copy_backward(pos,finish_n,old_finish);
                copy(first,last,pos);
            }else{
                const_iterator mid = first + elems_after;
                __uninitialized_copy_copy(mid,last,pos,finish,finish);
                finish = new_finish;
                copy(first,mid,pos);
            }
        }catch(...){
            destroy_node_at_back(new_finish);
        }
    }
}

template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::insert_aux(iterator pos,const value_type* first,const value_type* last,size_type n)
{
    const difference_type elems_before = pos - start;
    size_type length = size();
    if(elems_before < length/2){
        iterator new_start = reserve_elements_at_front(n);
        iterator old_start = start;
        pos = start + elems_before;
        try{
            if (elems_before >= difference_type(n)) {
                iterator start_n = start + difference_type(n);
                uninitialized_copy(start, start_n, new_start);
                start = new_start;
                copy(start_n, pos, old_start);
                copy(first, last, pos - difference_type(n));
            } else {
                const value_type* mid = first + (difference_type(n)-elems_before);
                __uninitialized_copy_copy(start,pos,first,mid,finish);
                start = new_start;
                copy(mid,last,old_start);
            }
        }catch(...){
            destroy_node_at_front(new_start);
        }
    }else{
        const difference_type elems_after = length - elems_before;
        iterator new_finish = reserve_elements_at_back(n);
        iterator old_finish = finish;
        pos = finish - elems_after;
        try{
            if (elems_after >= difference_type(n)) {
                iterator finish_n = finish - difference_type(n);
                uninitialized_copy(finish_n, finish, finish);
                finish = new_finish;
                copy_backward(pos, finish_n, old_finish);
                copy(first, last, pos);
            } else {
                const value_type *mid = first + elems_after;
                __uninitialized_copy_copy(mid, last, pos, finish, finish);
                finish = new_finish;
                copy(first, mid, pos);
            }
        }catch(...){
            destroy_node_at_back(new_finish);
        }
    }
}


template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::insert_aux(iterator pos,size_type n,const_reference x)
{
    const difference_type elems_before = pos - start;
    size_type length = size();
    value_type x_copy = x;
    if(elems_before < length/2){
        iterator old_start = start;
        iterator new_start = reserve_elements_at_front(n);
        pos = start + elems_before;
        try{
            if (elems_before >= difference_type(n)) {
                iterator start_n = start + n;
                uninitialized_copy(start, start_n, new_start);
                start = new_start;
                copy(start_n, pos, old_start);
                fill(pos - difference_type(n), pos, x_copy);
            } else {
                __uninitialized_copy_fill(start, pos, new_start, start, x_copy);
                start = new_start;
                fill(old_start, pos, x_copy);
            }
        }catch(...){
            destroy_node_at_front(new_start);
        }
    }else{
        iterator new_finish = reserve_elements_at_back(n);
        iterator old_finish = finish;
        const difference_type elems_after = difference_type(length) - elems_before;
        pos = finish - elems_after;
        try{
            if (elems_after > difference_type(n)) {
                iterator finish_n = finish - difference_type(n);
                uninitialized_copy(finish_n, finish, finish);
                finish = new_finish;
                copy_backward(pos, finish_n, old_finish);
                fill(pos, pos + difference_type(n), x_copy);
            } else {
                __uninitialized_fill_copy(finish, pos + difference_type(n),
                                          x_copy,
                                          pos, finish);
                finish = new_finish;
                fill(pos, old_finish, x_copy);
            }
        }catch(...){
            destroy_node_at_back(new_finish);
        }
    }
}

template<typename T,typename Alloc,size_t BufSiz>
typename deque<T,Alloc,BufSiz>::iterator deque<T,Alloc,BufSiz>::insert_aux(iterator pos,const_reference x)
{
    difference_type index = pos - start;
    value_type x_copy = x;
    if(index < size()/2){
        push_front(*start);
        iterator front1 = start;
        ++front1;
        iterator front2 = front1;
        ++front2;
        pos = start+index;
        iterator pos1 = pos;
        ++pos1;
        copy(front2,pos1,front1);
    }else{
        push_back(x);
        iterator back1 = finish;
        --back1;
        iterator back2 = back1;
        --back2;
        pos = start+index;
        copy_backward(pos,back2,back1);
    }
    *pos = x_copy;
    return pos;
}

template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::pop_back_aux()
{
    deallocate_node(finish.first);
    finish.set_node(finish.node-1);
    finish.cur = finish.last-1;
    destroy(finish.cur);
}

template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::pop_front_aux()
{
    destroy(start.cur);
    deallocate_node(start.first);
    start.set_node(start.node+1);
    start.cur = start.first;
}


template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::push_front_aux(const_reference x)
{
    value_type x_copy = x;
    reserve_map_at_front();
    *(start.node-1) = allocate_node();

    try {
        start.set_node(start.node - 1);
        start.cur = start.last - 1;
        construct(start.cur, x_copy);
    }catch (...){
        start.set_node(start.node+1);
        start.cur = start.first;
        deallocate_node(*(start.node-1));
    }
}

template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::push_back_aux(const_reference x)
{
    value_type x_copy = x;
    reserve_map_at_back();
    *(finish.node+1) = allocate_node();
    try{
        construct(finish.cur,x_copy);
        finish.set_node(finish.node+1);
        finish.cur = finish.first;
    }catch(...){
        deallocate_node(*(finish.node+1));
    }

}

template<typename T,typename Alloc,size_t BufSiz>
typename deque<T,Alloc,BufSiz>::iterator deque<T,Alloc,BufSiz>::erase(iterator pos)
{
    const difference_type elems_before = pos - start;
    iterator next = pos;
    ++next;
    if(elems_before < (size()>>1)){
        copy_backward(start,pos,next);
        pop_front();
    }else{
        copy(next,finish,pos);
        pop_back();
    }
    return start + elems_before;
}

template<typename T,typename Alloc,size_t BufSiz>
typename deque<T,Alloc,BufSiz>::iterator deque<T,Alloc,BufSiz>::erase(iterator first,iterator last)
{
    if(first==start && last==finish){
        clear();
        return finish;
    }
    difference_type n = last - first;
    difference_type elems_before = first - start;
    if(elems_before < (size()-n)/2){
        copy_backward(start,first,last);
        iterator new_start = start+n;
        destroy(start,new_start);
        for(map_pointer node=start.node;node<new_start.node;++node){
            deallocate_node(*node);
        }
        start = new_start;
    } else{
        copy(last,finish,first);
        iterator new_finish = finish-n;
        destroy(new_finish,finish);
        for(map_pointer node=new_finish.node+1;node<=finish.node;++node)
            deallocate_node(*node);
        finish = new_finish;
    }
    return start+elems_before;
}

template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::clear()
{
    map_pointer cur = start.node+1;
    for(;cur<finish.node;++cur){
        destroy(*cur,*cur+buf_size());
        deallocate_node(*cur);
    }
    if(start.node==finish.node){
        destroy(start.cur,finish.cur);
    }else{
        destroy(start.cur,start.last);
        destroy(finish.first,finish.cur);
        data_allocator::deallocate(finish.first,buf_size());
    }
    finish = start;
}


template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::create_map_and_nodes(size_type num_elems)
{
    size_type num_nodes = num_elems/buf_size()+1;
    map_size = max(num_nodes+2,initial_buf_size());
    map = map_allocator::allocate(map_size);

    map_pointer nstart = map+(map_size-num_nodes)/2;
    map_pointer nfinish = nstart+num_nodes-1;

    map_pointer cur;
    try {
        for(cur = nstart;cur<=nfinish;++cur){
            *cur = allocate_node();
        }
    }catch(...){
        for(map_pointer n = nstart;n<cur;++n){
            deallocate_node(*n);
        }
        map_allocator::deallocate(map,map_size);
    }
    start.set_node(nstart);
    start.cur = start.first;
    finish.set_node(nfinish);
    finish.cur = finish.first+(num_elems%map_size);
}

template<typename T,typename Alloc,size_t BufSiz>
void deque<T,Alloc,BufSiz>::fill_initialize(size_type n, const_reference x)
{
    create_map_and_nodes(n);
    map_pointer cur;
    for(cur=start.node;cur<finish.node;++cur)
        uninitialized_fill(*cur,*cur+buf_size(),x);
    uninitialized_fill(*cur,finish.cur,x);
}
__FRANK_NAMESPACE_END
#endif //STL_FRANK_DEQUE_H
