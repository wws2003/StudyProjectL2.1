//
//  VerticalArrayBasedBitonicSorter.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/14/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "VerticalArrayBasedBitonicSorter.h"
#include <algorithm>

template<typename T>
VerticalArrayBasedBitonicSorter<T>::VerticalArrayBasedBitonicSorter(BitonicVerticalArraySolverPtr<T> pSolver) : m_pSolver(pSolver) {
}

template<typename T>
void VerticalArrayBasedBitonicSorter<T>::sort(const ElementList<T>& inElements,
          ElementList<T>& outElements,
          SortOrder sortOrder) const {
    size_t elementCnt = inElements.size();
    
    // Check validation
    validateElementsCount(elementCnt);
    
    // Initialize vertical arrays info
    std::vector<BitonicVerticalArrayInfo<T>> verticalArrays;
    
    size_t verticalArraySwapBlockSize = 2;
    int sortingDepth = 1;
    
    while (verticalArraySwapBlockSize <= elementCnt) {
        // Vertical array would have depth 1, 2, 4 ..., log2(elementCnt)
        // Order-kept elements count also matches to swap block size (2, 4, 8...)
        BitonicVerticalArrayInfo<T> verticalArray(sortOrder,
                                              verticalArraySwapBlockSize,
                                              verticalArraySwapBlockSize,
                                              sortingDepth);
        
        verticalArrays.emplace_back(verticalArray);
        
        verticalArraySwapBlockSize <<= 1;
        sortingDepth++;
    }
    
    // Create vertical array data
    T* internalData = new T[elementCnt];
    std::copy(inElements.begin(), inElements.end(), internalData);
    BitonicVerticalArrayData<T> data(internalData, elementCnt);
    
    // Prepare solver
    m_pSolver->accept(data);
    
    // Solve the problem sequentially in the order of insertion
    for (BitonicVerticalArrayInfo<T> verticalArray : verticalArrays) {
        m_pSolver->solve(verticalArray);
    }
    
    // Collect results
    outElements.clear();
    outElements.insert(outElements.begin(), data.m_data, data.m_data + data.m_size);
    
    // Free vertical array data
    freePtr(data.m_data);
}


template<typename T>
void VerticalArrayBasedBitonicSorter<T>::validateElementsCount(size_t elementCnt) const {
    while(elementCnt > 1) {
        if (elementCnt % 2 != 0) {
            throw std::invalid_argument("Currently only accept list having number of elements is power of 2");
        }
        elementCnt /= 2;
    }
}