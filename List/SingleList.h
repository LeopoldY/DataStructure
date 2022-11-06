//
//  MyList.hpp
//  Homework7
//
//  Created by 杨程 on 2022/10/17.
//

#pragma once

#include "SingleListNode.h"

template <typename T>
class SingleList { //列表模板类
private:
    
    int _size; ListNodePosi(T) header; //规模、头哨兵

protected:
    
    void init(); //列表创建时的初始化
    int clear() {
        int oldSize = _size;
        while (0 < _size)
            remove(header->succ);
        return oldSize;
    }//清除所有节点
    void copyNodes(ListNodePosi(T) p, int n){ //p合法，且至少有n-1个真后继节点
        init();
        while(n--) {
            insertAsLast(p->data);
            p = p->succ;
        }
    } //复制列表中自位置p起的n顷
    void merge(ListNodePosi(T)&, int, SingleList<T>&, ListNodePosi(T), int); //有序列表区间归并
    void mergeSort(ListNodePosi(T)&, int); //对从p开始连续的n个节点归并排序
    void selectionSort(ListNodePosi(T), int); //对从p开始连续的n个节点选择排序
    void insertionSort(ListNodePosi(T), int); //对从p开始连续的n个节点插入排序

public:
    
    // MARK:构造函数
    SingleList() { init(); } //默认
    SingleList(SingleList<T> const& L) { copyNodes(L.first(), L._size); } //整体复制列表L
    SingleList(SingleList<T> const& L, Rank r, int n) { copyNodes(L[r], n); }//复制列表L中自第r项起的n项
    SingleList(ListNodePosi(T) p, int n) { copyNodes(p, n); } //复制列表中自位置p起的n项
    ~SingleList() {
        clear();
        delete header;
    } //释放(包含头、尾哨兵在内的)所有节点
    
    //只读访问接口
    ListNodePosi(T) head() const { return header;}
    Rank size() const { return _size; } //规模
    bool empty() const { return _size <= 0; } //判空
    T& operator[](Rank r) const; //重载，支持循秩访问(效率低)
    ListNodePosi(T) first() const { return header->succ; } //首节点位置
    ListNodePosi(T) last() const {
        ListNodePosi(T) h = header;
        while (h->succ != NULL) {
            h = h->succ;
        }
        return h;
    } //末节点位置
    bool valid(ListNodePosi(T) p) { //判断位置p是否对外合法
        return p && (header != p);
    } //将头、尾节点等同于NULL
    int disordered() const; //判断列表是否已排序
//    ListNodePosi(T) find(T const& e) const { //无序列表查找
//        return find(e, _size, trailer);
//    }
//    ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const {
//        while (0 < n--) //对亍p癿最近癿n个前驱，从右向左
//            if (e == (p = p->pred)->data) return p; //逐个比对，直至命中戒范围越界
//        return NULL;
//    } //无序区间查找
//    ListNodePosi(T) search(T const& e) const { //有序列表查找
//        return search(e, _size, trailer);
//    }
//    ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const; //有序区间查找
//    ListNodePosi(T) selectMax(ListNodePosi(T) p, int n); //在p及其前n-1个后继中选出最大者
//    ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); } //整体最大者 可写访问接口
    ListNodePosi(T) insertAsFirst(T const& e) {
        _size++;
        return header->insertAsSucc(e);
    } //将e当作首节点插入
    ListNodePosi(T) insertAsLast(T const& e){
        _size++;
        return last()->insertAsSucc(e);
    }//将e当作末节点插入
    ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e) {
        _size++;
        return p->insertAsPred(e);
    } //将e当作p的前驱插入
    ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const& e) {
        _size++;
        return p->insertAsSucc(e);
    } //将e当作p的后继插入
    T remove(ListNodePosi(T) p) {
        T e = p->data; //备份待删除节点数据(假定T类型可直接赋值)
        ListNodePosi(T) h = header;
        while (h->succ != p) {
            h = h->succ;
        }
        h->succ = p->succ;
        delete p; _size--; //释放节点，更新觃模
        return e;
    } //删除合法位置p处的节点,返回被删除节点
//    void merge(SingleList<T>& L) { merge(first(), _size, L, L.first(), L._size); } //全列表归并
//    void sort(ListNodePosi(T) p, int n); //列表区间排序
//    void sort() { sort(first(), _size); } //列表整体排序
//    int deduplicate(); //无序去重
//    int uniquify(); //有序去重
    void reverse() {
        ListNodePosi(T) curr = header->succ;
        ListNodePosi(T) prev = nullptr;
        while (curr != nullptr) {
            ListNodePosi(T) next = curr->succ;
            curr->succ = prev;
            prev = curr;
            curr = next;
        }
        header->succ = prev;
    }// 前后倒置(习题)
    
    //遍历
    void traverse(void (*)(T&)); //遍历，依次实施visit操作(函数指针，只读或局部性修改)
    
    template <typename VST> //操作器
    void traverse(VST&); //遍历，依次实施visit操作(函数对象，可全局性修改)
};//List


template <typename T>
void SingleList<T>::init() {
    header = new SingleListNode<T>;
    header->succ = NULL;
    _size = 0;
}

template <typename T> //重载下标操作符(效率低，需慎用)
T& SingleList<T>::operator[](Rank r) const { //assert: 0 <= r < size
    ListNodePosi(T) p = first();
    while (0 < r--) p = p->succ;
    return p->data;
}
