//
//  SortingNetworkImpl.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 2/11/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "SortingNetworkImpl.h"

template<typename U>
SortingNetworkImpl<U>::SortingNetworkImpl() : SortingNetwork<U>() {
    
    m_sortOrderDetectFunc = [](int sortOrderKeptSwapCnt,
                               int currentStartIndex,
                               SortOrder firstSortOrder,
                               SortOrder reversedSortOrder) {
        return ((currentStartIndex / (2 * sortOrderKeptSwapCnt)) % 2 == 0) ? firstSortOrder : reversedSortOrder;
    };
    
    m_lowerUpperSwapFuncMap[SortOrder::ASC] = [](ElementList<U>& elements,
                                                 unsigned int index1,
                                                 unsigned int index2) {
        U e1 = elements[index1];
        U e2 = elements[index2];
        elements[index1] = std::min(e1, e2);
        elements[index2] = std::max(e1, e2);
    };
    m_lowerUpperSwapFuncMap[SortOrder::DESC] = [](ElementList<U>& elements,
                                                  unsigned int index1,
                                                  unsigned int index2) {
        U e1 = elements[index1];
        U e2 = elements[index2];
        elements[index1] = std::max(e1, e2);
        elements[index2] = std::min(e1, e2);
    };
}

template<typename U>
void SortingNetworkImpl<U>::set(const ElementList<U>& elements) {
    m_elements.clear();
    m_elements.insert(m_elements.end(), elements.begin(), elements.end());
}

template<typename U>
size_t SortingNetworkImpl<U>::size() {
    return m_elements.size();
}

template<typename U>
void SortingNetworkImpl<U>::swap(int swapDistance,
                                 int sortOrderKeptSwapCnt,
                                 SortOrder firstSortOrder) {
    
    size_t elementCnt = size();
    if (elementCnt < 2) {
        return;
    }
    size_t numberOfSwapsInOneBlock = swapDistance;
    size_t numberOfSwapsBlocks = elementCnt / 2 / numberOfSwapsInOneBlock;
    SortOrder reversedSortOrder = (firstSortOrder == SortOrder::ASC) ? SortOrder::DESC : SortOrder::ASC;
    
    // Conduct swaps in blocks
    for(size_t i = 0; i < numberOfSwapsBlocks; i++) {
        // One swap block
        size_t startSwapIndex = i * (2 * numberOfSwapsInOneBlock);
        for(size_t j = 0; j < numberOfSwapsInOneBlock; j++) {
            size_t swapIndex1 = startSwapIndex + j;
            size_t swapIndex2 = swapIndex1 + swapDistance;
            // Conduct one swap
            SortOrder sortOrder = m_sortOrderDetectFunc(sortOrderKeptSwapCnt, swapIndex1, firstSortOrder, reversedSortOrder);
            m_lowerUpperSwapFuncMap[sortOrder](m_elements, swapIndex1, swapIndex2);
        }
    }
}

template<typename U>
void SortingNetworkImpl<U>::collect(ElementList<U>& elements) const {
    elements.clear();
    elements.insert(elements.end(), m_elements.begin(), m_elements.end());
}

template<typename U>
void SortingNetworkImpl<U>::validateElementsCount(size_t elementCnt) const {
    while(elementCnt > 1) {
        if (elementCnt % 2 != 0) {
            throw std::invalid_argument("Currently only accept list having number of elements is power of 2");
        }
        elementCnt /= 2;
    }
}

