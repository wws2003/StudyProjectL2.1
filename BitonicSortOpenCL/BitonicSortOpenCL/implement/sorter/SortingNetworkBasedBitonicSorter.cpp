//
//  SortingNetworkBasedBitonicSorter.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 3/17/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "SortingNetworkBasedBitonicSorter.h"

template<typename T>
SortingNetworkBasedBitonicSorter<T>::SortingNetworkBasedBitonicSorter(SortingNetworkPtr<T> pSortingNetwork) : ISorter<T>(), m_pSortingNetwork(pSortingNetwork){
}

template<typename T>
void SortingNetworkBasedBitonicSorter<T>::sort(const ElementList<T>& inElements,
          ElementList<T>& outElements,
          SortOrder sortOrder) const {
    // Set elements into sorting network
    m_pSortingNetwork->set(inElements);
    
    size_t elementCnt = m_pSortingNetwork->size();
    
    // Validate parameter first
    this->validateElementsCount(elementCnt);
    
    // Non-recursive approach: BM2 -> BM4 -> BM8 -> ..-> BM(n)
    int stageElementCnt = 2;
    while(stageElementCnt <= elementCnt) {
        int currentStateElementCnt = stageElementCnt;
        // BM2 = Swap
        // BM4 = Swap and then BM2
        // BM8 = Swap and then BM4 and then BM2
        // ...
        while(currentStateElementCnt >= 2) {
            m_pSortingNetwork->swap(currentStateElementCnt / 2,
                                  stageElementCnt / 2,
                                  sortOrder);
            currentStateElementCnt /= 2;
        }
        stageElementCnt *= 2;
    }

    // Collect results
    m_pSortingNetwork->collect(outElements);
}

template<typename T>
void SortingNetworkBasedBitonicSorter<T>::validateElementsCount(size_t elementCnt) const {
    while(elementCnt > 1) {
        if (elementCnt % 2 != 0) {
            throw std::invalid_argument("Currently only accept list having number of elements is power of 2");
        }
        elementCnt /= 2;
    }
}