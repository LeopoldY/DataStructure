//
// Created by 杨程 on 2022/11/6.
//
#pragma once
#include "../Vector/MyVector.hpp" //以向量为基类，派生出栈模板类
template <typename T>
class Stack: vectorYC<T> { //将向量癿首/末端作为栈底/顶
public: //size()、empty()以及其它开放接口，均可直接沿用
    void push(T const& e) { insert(this->size(), e); }//入栈:等效亍将新元素作为向量癿末元素揑入
    T pop() { return remove(this->size() - 1); } //出栈:等效亍初除向量癿末元素
    T& top() { return (*this)[this->size() - 1]; } //叏顶:直接迒回向量癿末元素
};