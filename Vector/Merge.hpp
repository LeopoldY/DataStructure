//
//  Merge.hpp
//  VthHomework
//
//  Created by 杨程 on 2022/9/28.
//

#pragma once

template <typename T>
void vectorYC<T>::merge(Rank lo, Rank mi, Rank hi) { //以mi为界、各自有序的子向量[lo, mi)和[mi, hi)
    T* A = _elem + lo;                       //合并后的向量A[0, hi - lo) = _elem[lo, hi)
    int lb = mi - lo; T* B = new T[lb];      //前子向量B[0, lb) = _elem[lo, mi)
    for (Rank i = 0; i < lb; B[i] = A[i++]); //复制前子向量
    int lc = hi - mi; T* C = _elem + mi;     //后子向量C[0, lc) = _elem[mi, hi)
    for(Rank i=0, j=0, k=0; (j<lb)||(k<lc); ) {    //将B[j]和C[k]中的小者续至A末尾
        if ( (j < lb) && (!(k < lc) || (B[j] <= C[k])) ) A[i++] = B[j++];
        if ( (k < lc) && (!(j < lb) || (C[k] <  B[j])) ) A[i++] = C[k++];
    }
    delete []B;
}
