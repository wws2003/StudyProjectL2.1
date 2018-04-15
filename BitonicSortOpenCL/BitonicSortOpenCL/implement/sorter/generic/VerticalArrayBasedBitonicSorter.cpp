//
//  VerticalArrayBasedBitonicSorter.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/14/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "VerticalArrayBasedBitonicSorter.h"

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
    
    // First assign output elements to the input ones
    outElements = inElements;
    
    // Initialize vertical arrays
    std::vector<BitonicVerticalArray<T>> verticalArrays;
    
    size_t verticalArraySwapBlockSize = 2;
    int sortingDepth = 1;
    
    while (verticalArraySwapBlockSize <= elementCnt) {
        // Vertical array would have depth 1, 2, 4 ..., log2(elementCnt)
        // Order-kept elements count also matches to swap block size (2, 4, 8...)
        BitonicVerticalArray<T> verticalArray(sortOrder,
                                              verticalArraySwapBlockSize,
                                              verticalArraySwapBlockSize,
                                              sortingDepth,
                                              &outElements);
        
        verticalArrays.emplace_back(verticalArray);
        
        verticalArraySwapBlockSize <<= 1;
        sortingDepth++;
    }
    
    // Solve the problem sequentially in the order of insertion
    for (BitonicVerticalArray<T> verticalArray : verticalArrays) {
        m_pSolver->solve(verticalArray);
    }
    
    // Collect results not needed since output elements effectively modified by solver
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