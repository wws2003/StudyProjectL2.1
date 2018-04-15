//
//  IntHybridBitonicVerticalArraySolver.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/14/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "IntHybridBitonicVerticalArraySolver.h"

IntHybridBitonicVerticalArraySolver::IntHybridBitonicVerticalArraySolver(BitonicVerticalArraySolverPtr<int> pGPUSolver) : m_pGPUSolver(pGPUSolver){
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
                                                   bitonicVerticalArray.m_swapBlockSize >> 1,
                                                   bitonicVerticalArray.m_sortingDepth - 1,
                                                   bitonicVerticalArray.m_pElementList);
    }
}

bool IntHybridBitonicVerticalArraySolver::canDelegateToGPU(const BitonicVerticalArray<int>& bitonicVerticalArray) const {
    // TODO Implement
    return false;
}

void IntHybridBitonicVerticalArraySolver::compareAndSwapFirstColumn(const BitonicVerticalArray<int>& bitonicVerticalArray) const {
    
    ElementList<int>& elementList = *(bitonicVerticalArray.m_pElementList);
    size_t elementCnt = bitonicVerticalArray.m_pElementList->size();
    
    // Compare and swap block by block, from higher indexes to lower indexes
    SortOrder currentSortOrder = bitonicVerticalArray.m_sortOrder;
    size_t currentHigherIndex = 0;
    size_t currentLowerIndex = currentHigherIndex + bitonicVerticalArray.m_swapBlockSize;
    size_t swapDistance = bitonicVerticalArray.m_swapBlockSize >> 1;
    
    while (currentLowerIndex < elementCnt) {
        // Sort and swap current block
        for (size_t i = currentLowerIndex; i <= currentHigherIndex - swapDistance; i++) {
            size_t lowerIndex = currentLowerIndex + i;
            size_t higherIndex = currentHigherIndex + i;
            HOST_COMPARE_AND_SWAP(elementList[lowerIndex], elementList[higherIndex], currentSortOrder);
        }
        // Next block
        currentLowerIndex = currentHigherIndex + 1;
        currentLowerIndex = currentHigherIndex + bitonicVerticalArray.m_swapBlockSize;
        currentSortOrder = (currentSortOrder == SortOrder::ASC) ? SortOrder::DESC : SortOrder::ASC;
    }
}