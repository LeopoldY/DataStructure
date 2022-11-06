//
//  Merge.hpp
//  VthHomework
//
//  Created by 杨程 on 2022/9/28.
//

#pragma once

template <typename T>
void vectorYC<T>::mergeSort(Rank lo, Rank hi) {
    if (hi - lo < 2) {
        return;
    }
    int mi = (lo + hi) >> 1;
    mergeSort(lo, mi); mergeSort(mi, hi); merge(lo, mi, hi);
}
