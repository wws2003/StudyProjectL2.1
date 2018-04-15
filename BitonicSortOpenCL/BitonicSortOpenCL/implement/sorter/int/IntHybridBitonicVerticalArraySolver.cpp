//
//  IntHybridBitonicVerticalArraySolver.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/14/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "IntHybridBitonicVerticalArraySolver.h"

IntHybridBitonicVerticalArraySolver::IntHybridBitonicVerticalArraySolver(size_t maxWorkGroupSize, BitonicVerticalArraySolverPtr<int> pGPUSolver) : m_maxWorkGroupSize(maxWorkGroupSize), m_pGPUSolver(pGPUSolver){
}

void IntHybridBitonicVerticalArraySolver::solve(const BitonicVerticalArray<int>& bitonicVerticalArray) const {
    
    // Check if need anymore work
    if (bitonicVerticalArray.m_sortingDepth < 1) {
        return;
    }
    
    if (this->canDelegateToGPU(bitonicVerticalArray)) {
        // Check then delegate to GPU solver if possible
        m_pGPUSolver->solve(bitonicVerticalArray);
    } else {
        // Do the job myself
        // 1. Carry out first column
        this->compareAndSwapFirstColumn(bitonicVerticalArray);
        // 2. Create sub vertical array
        BitonicVerticalArray<int> subVerticalArray(bitonicVerticalArray.m_sortOrder,
                                                   bitonicVerticalArray.m_orderKeptBlockSize,
                                                   bitonicVerticalArray.m_swapBlockSize >> 1,
                                                   bitonicVerticalArray.m_sortingDepth - 1,
                                                   bitonicVerticalArray.m_pElementList);
        // 3. Do the job
        this->solve(subVerticalArray);
    }
}

bool IntHybridBitonicVerticalArraySolver::canDelegateToGPU(const BitonicVerticalArray<int>& bitonicVerticalArray) const {
    return bitonicVerticalArray.m_swapBlockSize == m_maxWorkGroupSize;
}

void IntHybridBitonicVerticalArraySolver::compareAndSwapFirstColumn(const BitonicVerticalArray<int>& bitonicVerticalArray) const {
    
    ElementList<int>& elementList = *(bitonicVerticalArray.m_pElementList);
    size_t elementCnt = elementList.size();
    
    // Compare and swap block by block, from lower indexes to higher indexes
    size_t swapDistance = bitonicVerticalArray.m_swapBlockSize >> 1;
    
    SortOrder currentSortOrder = bitonicVerticalArray.m_sortOrder;
    SortOrder reversedSortOrder = HOST_REVERSED_ORDER(currentSortOrder);
    size_t currentLowerIndex = 0;
    
    while (currentLowerIndex < elementCnt) {
        // Max lower index that can be target of compare-and-swap
        size_t maxLowerIndex = currentLowerIndex + swapDistance - 1;
        // Sort and swap current block
        for (size_t lowerIndex = currentLowerIndex; lowerIndex <= maxLowerIndex; lowerIndex++) {
            size_t higherIndex = lowerIndex + swapDistance;
            HOST_COMPARE_AND_SWAP(elementList[lowerIndex], elementList[higherIndex], currentSortOrder);
        }
        // Next block
        currentLowerIndex += bitonicVerticalArray.m_swapBlockSize;
        currentSortOrder = (currentLowerIndex  / bitonicVerticalArray.m_orderKeptBlockSize) % 2 == 0 ? bitonicVerticalArray.m_sortOrder : reversedSortOrder;
    }
}