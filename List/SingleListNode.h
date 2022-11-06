//
//  ListNode.h
//  Homework7
//
//  Created by 杨程 on 2022/10/17.
//

#pragma once

typedef int Rank; //秩
#define ListNodePosi(T) SingleListNode<T>*//列表节点位置

template <typename T>
struct SingleListNode { //列表节点模板类(以单向链表形式实现)
// 成员
    T data; ListNodePosi(T) succ; //数值、后继
// 构造函数
    SingleListNode() {} //针对header构造
    SingleListNode( T e, ListNodePosi(T) s = NULL)
        : data(e), succ(s) {} //默认构造器
// 操作接口
    ListNodePosi(T) insertAsPred(T const& e); //紧靠当前节点之前插入新节点
    ListNodePosi(T) insertAsSucc(T const& e) {
        ListNodePosi(T) x = new SingleListNode(e, succ);
        this->succ = x;
        return x;
    }//紧靠当前节点之后插入新节点
    
};
