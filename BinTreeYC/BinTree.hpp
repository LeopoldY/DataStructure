#pragma once

#include <climits>
#include "BinNode.hpp"
#include "../Vector/MyVector.hpp"
#include "../Queue/Queue.hpp"

#define print(x) std::cout << x << std::endl

typedef enum { prev, midd, post, level} TravForm; //遍历方式

template <typename T>
class BinTree { //二叉树模板类
protected:
    int _size; //规模
    BinNodePosi(T) _root; //根节点
    virtual int updateHeight(BinNodePosi(T)x); //更新节点x的高度
    void updateHeightAbove(BinNodePosi(T)x); //更新节点x及其祖先的高度
public:
    BinTree() : _size(0), _root(nullptr) {} //构造函数
    ~BinTree() { if (0 < _size) remove(_root); } //析构函数
    BinNodePosi(T) createBinTree(vectorYC<T>& nodes);

    int size() const { return _size; } //规模
    bool empty() const { return !_root; } //判空
    BinNodePosi(T) root() const { return _root; } //树根
    BinNodePosi(T) insertAsRoot(T const &e); //插入根节点
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const &e); //e作为x的左孩子(原无)插入
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const &e); //e作为x的右孩子(原无)插入
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &); //T作为x左子树接入
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &); //T作为x右子树接入
    int remove(BinNodePosi(T)x); //初除以位置x处节点为根的子树，返回诠子树原先的规模
    BinTree<T> *secede(BinNodePosi(T)x); //将子树x从弼前树中摘除，幵将其转换为一棵独立子树
    template<typename VST>
    void travLevel(VST &visit) { if (_root) _root->travLevel(visit); } //层次遍历
    template<typename VST>
    void travPre(BinNodePosi(T) x, const VST& visit); //先序遍历
    template<typename VST>
    void travIn(VST &visit) { if (_root) _root->travIn(visit); } //中序遍历
    template <typename VST> //元素类型、操作器
    void travIn_R(BinNodePosi(T) x, VST& visit) { //二叉树中序遍历算法(递归版)
        if (!x) return;
        travIn_R(x->lChild, visit);
        visit(x->data);
        travIn_R(x->rChild, visit);
    }
    template<typename VST>
    void travPost(BinNodePosi(T) x, VST& visit); //后序遍历
// 比较器、判等器(各列其一，其余自行补充)
    bool operator<(BinTree<T> const &t) { return _root && t._root && lt(_root, t._root); }

    bool operator==(BinTree<T> const &t) { return _root && t._root && (_root == t._root); }

// 交换左右子树
    void swap(BinNodePosi(T) t);
// 最小深度求解
    int minDepth(BinNodePosi(T) root) {
        if(root == nullptr)
            return 0;
        int leftHeight, rightHeight;
        if(root->lChild && root->rChild) {
            leftHeight = minDepth(root->lChild);
            rightHeight = minDepth(root->rChild);
        }
        else if(root->lChild && !root->rChild) {
            leftHeight = minDepth(root->lChild);
            rightHeight = INT_MAX;
        }
        else if(!root->lChild && root->rChild) {
            leftHeight = INT_MAX;
            rightHeight = minDepth(root->rChild);
        }
        else {
            leftHeight = rightHeight = 0;
        }
        return std::min(leftHeight, rightHeight)+1;
    }

};

template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) //更新节点x高度
{ return x->height = 1 + std::max(stature(x->lChild), stature(x->rChild)); } //具体觃则因树丌同而异

template <typename T> //更新x及祖先的高度
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) {
    while (x && x->height != updateHeight(x)) {
        updateHeight(x);
        x = x->parent;
    }
} //可优化:一旦高度没有变化，即可终止

//节点插入
template <typename T> BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)
{ _size = 1; return _root = new BinNode<T>(e); } //将e弼作根节点揑入空癿二叉树

template <typename T> BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)
{ _size++; x->insertAsLC(e); updateHeightAbove(x); return x->lChild; } //e揑入为x癿左孩子

template <typename T> BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{ _size++; x->insertAsRC(e); updateHeightAbove(x); return x->rChild; } //e揑入为x癿右孩子

//子树接入
template <typename T> //二叉树子树接入算法:将S当作节点x的左子树接入，S本身置空
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S) { //x->lChild == nullptr
    if (x->lChild == S->_root) x->lChild->parent = x; //接入
    _size += S->_size;
    updateHeightAbove(x); //更新全树规模与x所有祖先的高度
    S->_root = nullptr;
    S->_size = 0;
    std::memory_order::release(S);
    S = nullptr;
    return x; //释放原树，返回接入位置
}

template <typename T> //二叉树子树接入算法:将S当作节点x的右子树接入，S本身置空
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S) { //x->rChild == nullptr
    if (x->rChild == S->_root) x->rChild->parent = x; //接入
    _size += S->_size;
    updateHeightAbove(x); //更新全树规模与x所有祖先的高度
    S->_root = nullptr;
    S->_size = 0;
//    release(S);
    std::memory_order::release(S);
    S = nullptr;
    return x; //释放原树，返回接入位置
}

//子树删除
template <typename T> //初除二叉树中位置x处癿节点及其后代，迒回被初除节点癿数值
int BinTree<T>::remove(BinNodePosi(T) x) { //assert: x为二叉树中癿合法位置
    FromParentTo(*x) = nullptr; //切断来自父节点癿指针
    updateHeightAbove(x->parent); //更新祖先高度
    int n = removeAt(x);
    _size -= n;
    return n; //初除子树x，更新觃模，迒回初除节点总数
}

template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除除节点的数值
static int removeAt(BinNodePosi(T) x) { //assert: x为二叉树中的合法位置
    if (!x) return 0; //递归基:空树
    int n = 1 + removeAt(x->lChild) + removeAt(x->rChild); //递归释放左、右子树
    delete x;
    return n; //释放被摘除节点，并返回初除节点总数
}

//子树分离
template <typename T> //二叉树子树分离算法:将子树x从当前树中摘除，将其封装为一棵独立子树返回
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) { //assert: x为二叉树中的合法位置
    FromParentTo(*x) = nullptr; //切断来自父节点的指针
    updateHeightAbove(x->parent); //更新原树中所有祖先的高度
    BinTree<T> *S = new BinTree<T>;
    S->_root = x;
    x->parent = nullptr; //新树以x为根
    S->_size = x->size();
    _size -= S->_size;
    return S; //更新规模，返回分离出来的子树
}

//交换二叉树所有子树
template<typename T>
void BinTree<T>::swap(BinNode<T> *t) {
    if (t) {
        BinNodePosi(T) temp = t->lChild;
        t->lChild = t->rChild;
        t->rChild = temp;
        swap(t->lChild);
        swap(t->rChild);
    }
}


template<typename T>
static BinNodePosi(T) CreateTree(vectorYC<T> arr, int index) {
    if (index >= arr.size() || arr[index] == -1) return nullptr;
    BinNodePosi(T) node = new BinNode<T>(arr[index]);
    node->lChild = CreateTree(arr, 2*index+1);
    node->rChild = CreateTree(arr, 2*(index+1));

    return node;
}

template<typename T> //传入层次遍历顺序的向量并创建完全二叉树
BinNodePosi(T) BinTree<T>::createBinTree(vectorYC<T> &nodes) {
    _root = CreateTree(nodes, 0);
    return _root;
}

// 遍历 & 输出
template <typename T>
struct Show_BT {
    virtual void operator()(T& e)const {
        std::cout << e << ", ";
    }
};

template <typename T>
template <typename VST> //元素类型、操作器
void BinTree<T>::travPre(BinNodePosi(T) x, const VST& visit) { //二叉树先序遍历算法(递归版)
    if (!x) return;
    visit(x->data);
    travPre(x->lChild, visit);
    travPre(x->rChild, visit);
}

template <typename T>
template <typename VST> //元素类型、操作器
void BinTree<T>::travPost(BinNodePosi(T) x, VST& visit) { //二叉树后序遍历算法(逑弻版)
    if (!x) return;
    travPost(x->lChild, visit);
    travPost(x->rChild, visit);
    visit(x->data);
}

template <typename T> //遍历输出所有元素
void printElemWithTravForm(BinTree<T> tree, TravForm TF){
    Show_BT<T> tempShow;
    switch (TF) {
        case prev:
            print("Pre-order traversal:");
            tree.template travPre(tree.root(), tempShow);
            break;
        case midd:
            print("Mid-order traversal:");
            tree.template travIn_R(tree.root(), tempShow);
            break;
        case post:
            print("Post-order traversal:");
            tree.template travPost(tree.root(), tempShow);
            break;
        case level:
            print("level traversal:");
            tree.template travLevel(tempShow);
            break;
    }
    std::cout << std::endl;
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travLevel(VST& visit) { //二叉树层次遍历算法
    Queue<BinNodePosi(T)> Q; //辅助队列
    Q.enqueue(this); //根节点入队
    while (!Q.empty()) { //在队列再次变空之前，反复迭代
        BinNodePosi(T)x = Q.dequeue();
        visit(x->data); //取出队首节点并访问
        if (HasLChild(*x)) Q.enqueue(x->lChild); //左孩子入队
        if (HasRChild(*x)) Q.enqueue(x->rChild); //右孩子入队
    }
}
