//
//  MyVector.hpp
//  VthHomework
//
//  Created by 杨程 on 2022/9/26.
//

#pragma once

typedef int Rank;
#define DEFAULT_CAPACITY 3

template<typename T>
class vectorYC {
private:
    Rank _size; int _capacity; T* _elem;
    
    void copyFrom(T const* A, Rank lo, Rank hi);
    void merge(Rank lo, Rank mi, Rank hi);
    void mergeSort(Rank lo, Rank hi);
    
public:
    vectorYC(int c = DEFAULT_CAPACITY, int s = 0 ,T v =0){
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }
    vectorYC(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
    vectorYC(T const* A, Rank n) { copyFrom(A, 0, n); }
    vectorYC(vectorYC<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }
    vectorYC(vectorYC<T> const& V) { copyFrom(V._elem, 0, V._size); }

    ~vectorYC() { delete [] _elem; }

    Rank size() const { return _size; }
    
    T& operator[](Rank r) const;
    vectorYC<T> & operator=(vectorYC<T> const&);
    
    void traverse(void (*visit)(T&)) {
        for (int i = 0; i < _size; i++) {
            visit(_elem[i]);
        }
    }
    template <typename VST> void traverse(const VST& visit) {
        for (int i = 0; i < _size; i++) {
            visit(_elem[i]);
        }
    };
    
    void printElem(T& e) {
        std::cout << e;
    }
    
};

#include "CopyFrom.hpp"
#include "Merge.hpp"
#include "MergeSort.hpp"

template <typename T>
T& vectorYC<T>::operator[](Rank r) const { return _elem[r]; }

template <typename T>
vectorYC<T>& vectorYC<T>::operator=(vectorYC<T> const& V ) {
    if (_elem) delete [] _elem;
    copyFrom(V._elem, 0, V.size());
    return *this;
}

template <typename T>
struct Show_V {
    virtual void operator()(T& e)const {
        std::cout << e << ", ";
    }
};

template <typename T> //遍历输出所有元素
void show(vectorYC<T> V){
    std::cout << '[';
    Show_V<T> temp;
    V.traverse(temp);
    /*
     教材中
        V.travers(Show<T>());
     这种写法是有问题的，它用了一个临时对象的引用传入参数，而使用一个销毁了的临时对象的引用是灾难性的。
    */
    std::cout << ']' << std::endl;
}

/* 修改方法：
 1.
 Show<T> a;
 V.traverse(a);

 2.使用const VST&去接这个临时对象
 //常对象调用常函数
 template<class T>
 struct Show {
     virtual void operator()(T& e)const { std::cout << e << " "; }
 };

 template<class VST> void traverse(const VST& visit);

 template<class T>template<class VST>
 void vectorYC<T>::traverse(const VST& visit) {
     for (int i = 0; i < _size; i++) {
         visit(elem[i]);
     }
 }

 3.最右值引用
 template<class VST> void traverse(VST&& visit);

 template<class T>template<class VST>
 void vectorYC<T>::traverse(VST&& visit) {
     for (int i = 0; i < _size; i++) {
         visit(elem[i]);
     }
 }
 */
