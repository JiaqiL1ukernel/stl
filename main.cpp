#include <iostream>
#include <new>
#include "frank_alloc.h"
#include <ext/pool_allocator.h>
#include <list>
#include "frank_vector.h"
static int alloc_sum = 0;
static int alloc_count = 0;
void *operator new(size_t size){
    alloc_sum += size;
    alloc_count++;
    return malloc(size);
}

template<typename T>
using listPool = std::list<T,__gnu_cxx::__pool_alloc<T>>;

class Pa{
private:
    int _age;
public:
    Pa(int age = 0):_age(age){}
    Pa(Pa& p):_age(p._age){std::cout<<"using none const version\n";}
    Pa(const Pa& p):_age(p._age){std::cout<<"using const version\n";}

};
#include <vector>
#include <list>
#include "frank_algobase.h"
void print_arr(int *arr,int n)
{
    while(n>0)
    {
        std::cout<<arr[3-n]<<"\n";
        n--;
    }
}

using namespace frank;
template<typename T>
void print_vector(vector<T>& v)
{
    int n = 0;
    for(;n<v.size();n++){

        std::cout<<"v["<<n<<"]="<<v[n]<<"  ";
    }
    std::cout<<'\n';
    std::cout<<"size: "<<v.size()<<" front: "<<v.front()<<" back: "<<v.back()<<std::endl;
    std::cout<<"capacity: "<<v.capacity()<<std::endl;
}
struct Student
{
        int id;
        const char *name;
        Student(int _id=0,const char *_name = "dsd"):id(_id),name(_name){}
        friend std::ostream & operator<<(std::ostream& os,const Student& s){
            return os<<"{id: "<<s.id<<" name: "<<s.name<<"}\n";
        }
};
#include "frank_list.h"
template<typename T>
void show_list(const list<T>& l)
{
    typename list<T>::const_iterator i = l.begin();
    int num = 0;
    for(;i!=l.end();++i)
    {
        std::cout<<"#"<<num++<<":"<<*i<<"  ";
    }
}

#include "frank_deque.h"
template<typename T,typename Alloc,size_t BufSiz>
void show_deque(const deque<T,Alloc,BufSiz>& x){

    typename deque<T,Alloc,BufSiz>::const_iterator i = x.begin();
    int count = 0;
//    if(i!=x.end())
//        std::cout<<"has elems\n";
    for(;i!=x.end();++i,++count)
        std::cout<<"#"<<count<<":"<<*i<<"  ";

}

#include "frank_stack.h"
#include "frank_queue.h"
#include "frank_heap.h"
#include <vector>
#include "frank_algo.h"
struct binary_op
{
    bool operator()(int x,int y){return x == y;}
};

struct pred
{
    bool operator()(int x){return x>3;}
};
int main() {
    using namespace frank;

    int a[] = {1,2,3,3,5,6,7,8};
    int b[] = {3,4,5};
    vector<int> arr1(&a[0],&a[8]);
    vector<int> arr2(&b[0],&b[3]);
    vector<int>::iterator new_end,i;
    new_end = partition(arr1.begin(),arr1.end(),pred());
    for(i = arr1.begin();i != arr1.end();++i){
        std::cout<<*i<<" ";
    }

}
