#include <iostream>

#include "BinTree.hpp"
#include "Solution.hpp"
#include "../Vector/MyVector.hpp"

int main() {
    Solution solution;
    int a[5] = {3,9,20,15,7};
    int b[5] = {9,3,15,20,7};
    vectorYC<int> preOrder(a,5);
    vectorYC<int> inOrder(b, 5);
    BinNodePosi(int) root;
    root = solution.BuildBinTree(preOrder, inOrder);

    return 0;
}
