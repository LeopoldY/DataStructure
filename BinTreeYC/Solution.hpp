//
// Created by 杨程 on 2022/11/6.
//

#pragma once

#include "BinTree.hpp"
#include "../Vector/MyVector.hpp"

class Solution {
public:
    BinNode<int> * BuildBinTree(vectorYC<int>& preOrder, vectorYC<int>& inOrder);
};


