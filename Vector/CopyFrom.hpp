//
//  CopyFrom.hpp
//  VthHomework
//
//  Created by 杨程 on 2022/9/28.
//

#pragma once

template <typename T>
void vectorYC<T>::copyFrom(T const* A, Rank lo, Rank hi) {
    _elem = new T[_capacity = 2 * (hi - lo)]; _size = 0;
    while (lo < hi)
        _elem[_size++] = A[lo++];
}
