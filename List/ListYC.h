//
//  ListYC.hpp
//  Homework8
//
//  Created by 杨程 on 2022/10/18.
//

#pragma once

#include <iostream>
#include "ListNode.h"

enum SortForm {
    mergeSort, // 0
    selectionSort, // 1
    insertionSort, // 2
};

template <typename T>
class ListYC { //列表模板类
private:
// 规模、头哨兵、尾哨兵
    int _size; ListNodePosi(T) header; ListNodePosi(T) trailer;
protected:
    void init();
    int clear(); //清除所有节点
    void copyNodes(ListNodePosi(T), int); //复制列表中自位置p起的n项
    void merge(ListNodePosi(T)&, int, ListYC<T>&, ListNodePosi(T), int); //有序列表匙间弻幵
    void mergeSort(ListNodePosi(T)&, int); //对从p开始连续的n个节点归并排序
    void insertionSort(ListNodePosi(T), int); //对从p开始连续的n个节点插入排序
public:
// 构造函数
    ListYC() { init(); } //默认
    ListYC(ListYC<T> const& L) { copyNodes(L.first(), L._size); } //整体复制列表L
    ListYC(ListYC<T> const& L, Rank r, int n) { copyNodes(L[r], n); } //复制列表L中自第r项起的n项
    ListYC(ListNodePosi(T) p, int n) { copyNodes(p, n); } //复制列表中自位置p起的n项
// 析构函数
    ~ListYC() { clear(); delete header; delete trailer; } //释放(包含头、尾哨兵在内的)所有节点

// 只读访问接口
    Rank size() const { return _size; } //规模
    bool empty() const { return _size <= 0; } //判空
    T& operator[](Rank r) const; //重载，支持循秩访问(效率低)
    ListNodePosi(T) head() const { return header;} // 头哨兵位置
    ListNodePosi(T) tail() const { return header;} // 尾哨兵位置
    ListNodePosi(T) first() const; //首节点位置
    ListNodePosi(T) last() const { return trailer->pred; } //末节点位置
    bool valid(ListNodePosi(T) p) //判断位置p是否对外合法
    { return p && (trailer != p) && (header != p); } //将头、尾节点等同于NULL
    int disordered() const; //判断列表是否已排序
    ListNodePosi(T) findByIndex(int); // 通过下标寻找节点
    ListNodePosi(T) find(T const& e) const //无序列表查找
    { return find(e, _size, trailer); }
    ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; //无序区间查找
    ListNodePosi(T) search(T const& e) const //有序列表查找
    { return search(e, _size, trailer); }
    ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const; //有序区间查找
    ListNodePosi(T) selectMax(ListNodePosi(T) p, int n); //在p及其前n-1个后继中选出最大者
    ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); } //整体最大者 可写访问接口
    ListNodePosi(T) insertAsFirst(T const& e); //将e作首节点插入
    ListNodePosi(T) insertAsLast(T const& e); //将e作末节点插入
    ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e); //将e作p的前驱插入
    ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const& e); //将e作p的后继插入
    T remove(ListNodePosi(T) p); //初除合法位置p处的节点,返回被删除节点
    void merge(ListYC<T>& L) { merge(first(), size(), L, L.first(), L._size); } //全列表归并
    void sort(ListNodePosi(T) p, int n, SortForm sortForm); //列表区间排序
    void sort(SortForm sortForm); //列表整体排序
    void selectionSort(ListNodePosi(T), int); //对从p开始连续的n个节点选择排序
    int deduplicate(); //无序去重
    int uniquify(); //有序去重
    void reverse(); //前后倒置(习题)
    void swapNodes(ListNodePosi(T), ListNodePosi(T));
// 遍历
    void traverse(void (*)(T&));
    template <typename VST> //操作器
    void traverse(VST&);

// 输出
    void printData();
};

// 默认初始化
template <typename T>
void ListYC<T>::init() {
    header = new ListNode<T>; //创建头哨兵节点
    trailer = new ListNode<T>; //创建尾哨兵节点
    header->succ = trailer; header->pred = nullptr;
    trailer->pred = header; trailer->succ = nullptr;
    _size = 0;
}

template <typename T> //重载下标操作符，以通过秩直接访问列表节点(效率低)
T& ListYC<T>::operator[](Rank r) const { //assert: 0 <= r < size
    ListNodePosi(T) p = first(); //从首节点出发
    while (0 < r--) p = p->succ; //正数第r个节点即是
    return p->data; //目标节点，返回回其中所存元素
}

template <typename T> //在无序列表内节点p(可能是trailer)的n个(真)前驱中，找到等于e的最后者
ListNodePosi(T) ListYC<T>::find(T const& e, int n, ListNodePosi(T) p) const { //0<=n<=rank(p)<_size
    while (0 < n--) //对于p的最近的n个前驱，从右向左
        if (e == (p = p->pred)->data) return p; //逐个比对，直至命中戒范围越界
    return nullptr; //p越出左边界意味着区间内不含e，查找失败
} // 失败时，返回NULL

// 插入
template <typename T> ListNodePosi(T) ListYC<T>::insertAsFirst(T const& e)
{ _size++; return header->insertAsSucc(e); } //e作首节点插入

template <typename T> ListNodePosi(T) ListYC<T>::insertAsLast(T const& e)
{ _size++; return trailer->insertAsPred(e); } //e作末节点插入

template <typename T> ListNodePosi(T) ListYC<T>::insertBefore(ListNodePosi(T) p, T const& e)
{ _size++; return p->insertAsPred(e); } //e作p的前驱插入

template <typename T> ListNodePosi(T) ListYC<T>::insertAfter(ListNodePosi(T) p, T const& e)
{ _size++; return p->insertAsSucc(e); } //e作p的后继插入

// 复制ListNode
template <typename T>
void ListYC<T>::copyNodes(ListNodePosi(T) p, int n) { //p合法，且至少有n-1个真后继节点
    init(); //创建头、尾哨兵节点开始做初始化
    while(n--) { insertAsLast(p->data); p = p->succ; } //将起自p的n项依次作为末节点插入
}

// 删除
template <typename T>
T ListYC<T>::remove(ListNodePosi(T) p) { //删除合法位置p处节点，返回其数值
    T e = p->data; //备份待删除节点的数值(假定T类型可直接赋值)
    p->pred->succ = p->succ; p->succ->pred = p->pred; //后继、前驱
    delete p;
    _size--; //释放节点，更新规模
    return e; //返回备份的数值
}

template <typename T>
int ListYC<T>::clear() { //清空列表
    int oldSize = _size;
    while (0 < _size) remove(header->succ); //反复删除首节点，直至列表为空
    return oldSize;
}

// 翻转链表
template <typename T>
void ListYC<T>::reverse() {
    int lo = 1, hi = _size;
    while (lo < hi) {
        swapNodes(findByIndex(lo++), findByIndex(hi--));
    }
}

// 交换两个节点
template <typename T>
void ListYC<T>::swapNodes(ListNodePosi(T) p1, ListNodePosi(T) p2) { //交换两个节点
    if (p1 == p2) {
        return;
    } else if (p1->succ == p2 || p1->pred == p2){
        ListNodePosi(T) temp = p1->pred;
        ListNodePosi(T) dummytail = p2->succ;

        p1->succ = p2->succ;
        p2->pred = p1->pred;
        p1->pred = p2;
        p2->succ = p1;
        temp->succ = p2;
        dummytail->pred = p1;
    }else if (p1->pred == p2) {
        ListNodePosi(T) temp = p2->pred;
        ListNodePosi(T) dummytail = p1->succ;

        p2->succ = p1->succ;
        p1->pred = p2->pred;
        p2->pred = p1;
        p1->succ = p2;
        temp->succ = p1;
        dummytail->pred = p2;
    } else {
        ListNodePosi(T) prev1 = p1->pred;
        ListNodePosi(T) prev2 = p2->pred;
        ListNodePosi(T) next1 = p1->succ;
        ListNodePosi(T) next2 = p2->succ;

        p1->succ = next2;
        p1->pred = prev2;
        p2->succ = next1;
        p2->pred = prev1;

        prev1->succ = p2;
        next1->pred = p2;
        prev2->succ = p1;
        next2->pred = p1;
    }
}

// 通过下标寻找节点
template <typename T>
ListNodePosi(T) ListYC<T>::findByIndex(int n) {
    ListNodePosi(T) curr = first();
    for (int i = 0; i < n; i++) {
        curr = curr->succ;
    }
    return curr->pred;
}

// 遍历
template <typename T> void ListYC<T>::traverse(void (*visit)(T&)) //利用函数指针机制的遍历
{ for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ) visit(p->data); }

template <typename T> template <typename VST> //元素类型、操作器
void ListYC<T>::traverse(VST& visit) //利用函数对象机制的遍历
{ for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ) visit(p->data); }

// 输出操作器
template <typename T>
struct Show_L {
    virtual void operator()(T& e)const {
        std::cout << e << ", ";
    }
};

// 遍历输出所有元素
template <typename T>
void ListYC<T>::printData() {
    std::cout << '[';
    Show_L<T> temp;
    traverse(temp);
    std::cout << ']' << std::endl;
}

// 选择区间最大值
template <typename T>
ListNodePosi(T) ListYC<T>::selectMax(ListNode<T> *p, int n) {
    ListNodePosi(T) maxP = p;
    for (int i = 0; i < n; i++) {
        if (p->data > maxP->data) { maxP = p; }
        p = p->succ;
    }
    return maxP;
}

template <typename T>
void ListYC<T>::selectionSort(ListNode<T> *p, int n) {
    ListNodePosi(T) head = p->pred; ListNodePosi(T)tail = p;
    for (int i = 0; i < n; i++) { tail = tail->succ; }
    while (1 < n) {
        ListNodePosi(T) max = selectMax(head->succ, n); //找出最大者(歧义时后者优先)
//        insertBefore(tail, remove(max)); //将其移至无序区间末尾(作为有序区间新的首元素)
        swapNodes(max, tail->pred);
        tail = tail->pred; n--;
    }
}

template <typename T>
void ListYC<T>::sort(ListNodePosi(T) p, int n, SortForm sortForm) {
    switch (sortForm) {
        case 0:
//            mergeSort(p, n);
            break;
        case 1:
            selectionSort(p, n);
            break;
        case 2:
//            insertionSort(p, n);
            break;
        default:
            selectionSort(p, n);
            break;
    }
}

template<typename T>
void ListYC<T>::sort(SortForm sortForm) { sort(first(), _size, sortForm); }

template <typename T> ListNodePosi(T) ListYC<T>::first() const {return header->succ;}