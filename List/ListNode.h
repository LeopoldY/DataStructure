//
//  ListNode.hpp
//  Homework8
//
//  Created by 杨程 on 2022/10/18.
//

#pragma once

typedef int Rank; //秩
#define ListNodePosi(T) ListNode<T>*//列表节点位置

template <typename T> struct ListNode { //列表节点模板类(以双向链表形式实现)
// 成员
    T data; ListNodePosi(T) pred; ListNodePosi(T) succ; //数值、前驱、后继
// 构造函数
    ListNode() = default; //针对header和trailer癿极造
    explicit ListNode( T e, ListNodePosi(T) p = nullptr, ListNodePosi(T) s = nullptr)
            : data(e), pred(p), succ(s) {} //默认构造器
// 操作接口
    ListNodePosi(T) insertAsPred(T const& e); //紧靠当前节点之前插入新节点
    ListNodePosi(T) insertAsSucc(T const& e); //紧靠当前节点之后插入新节点
    void printNodes();

};

template <typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e) {
    ListNodePosi(T) x = new ListNode(e, pred, this); //创建新节点
    pred->succ = x; pred = x; //设置正向链接
    return x; //返回新节点的位置
}

template <typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e) {
    ListNodePosi(T) x = new ListNode(e, this, succ); //创建新节点
    succ->pred = x; succ = x; //设置逆向链接
    return x;
}

template <typename T>
void ListNode<T>::printNodes() {
    ListNodePosi(T) firstNode = this;
    std::cout << '[';
    while (firstNode->succ != nullptr) {
        std::cout << firstNode->data << ", ";
        firstNode = firstNode->succ;
    }
    std::cout << ']' << std::endl;
}
