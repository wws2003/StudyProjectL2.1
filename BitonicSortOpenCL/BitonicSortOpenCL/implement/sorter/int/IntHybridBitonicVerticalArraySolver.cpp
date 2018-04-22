//
//  IntHybridBitonicVerticalArraySolver.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/14/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "IntHybridBitonicVerticalArraySolver.h"
#include <exception>

IntHybridBitonicVerticalArraySolver::IntHybridBitonicVerticalArraySolver(size_t maxWorkGroupSize, BitonicVerticalArraySolverPtr<int> pGPUSolver) : m_maxWorkGroupSize(maxWorkGroupSize), m_pGPUSolver(pGPUSolver), m_dataBuffer(NullPtr, 0){
}

void IntHybridBitonicVerticalArraySolver::accept(const BitonicVerticalArrayData<int>& data)  {
    // Store data buffer
    m_dataBuffer.m_data = data.m_data;
    m_dataBuffer.m_size = data.m_size;
    
    // Share data buffer to GPU solver
    m_pGPUSolver->accept(data);
}

void IntHybridBitonicVerticalArraySolver::solve(const BitonicVerticalArrayInfo<int>& info) {
    // Validation check
    if (m_dataBuffer.m_size < 1) {
        throw std::invalid_argument("Internal data is empty");
    }
    // Do the job
    doSolve(info);
}

void IntHybridBitonicVerticalArraySolver::doSolve(const BitonicVerticalArrayInfo<int>& info) const {
    
    // Check if need anymore work
    if (info.m_sortingDepth < 1) {
        return;
    }
    
    if (this->canDelegateToGPU(info)) {
        // Check then delegate to GPU solver if possible
        m_pGPUSolver->solve(info);
    } else {
        // Do the job myself
        // 1. Carry out first column
        this->compareAndSwapFirstColumn(info);
        // 2. Create sub vertical array
        BitonicVerticalArrayInfo<int> subVerticalArrayInfo(info.m_sortOrder,
                                                   info.m_orderKeptBlockSize,
                                                   info.m_swapBlockSize >> 1,
                                                   info.m_sortingDepth - 1);
        // 3. Do the job
        this->doSolve(subVerticalArrayInfo);
    }
}

bool IntHybridBitonicVerticalArraySolver::canDelegateToGPU(const BitonicVerticalArrayInfo<int>& info) const {
    //return 256 <= info.m_swapBlockSize && info.m_swapBlockSize <= m_maxWorkGroupSize;
    return info.m_swapBlockSize == m_maxWorkGroupSize;
}

void IntHybridBitonicVerticalArraySolver::compareAndSwapFirstColumn(const BitonicVerticalArrayInfo<int>& info) const {
    
    int* data = m_dataBuffer.m_data;
    size_t elementCnt = m_dataBuffer.m_size;
    
    // Compare and swap block by block, from lower indexes to higher indexes
    size_t swapDistance = info.m_swapBlockSize >> 1;
    
    SortOrder currentSortOrder = info.m_sortOrder;
    SortOrder reversedSortOrder = HOST_REVERSED_ORDER(currentSortOrder);
    size_t currentLowerIndex = 0;
    
    while (currentLowerIndex < elementCnt) {
        // Max lower index that can be target of compare-and-swap
        size_t maxLowerIndex = currentLowerIndex + swapDistance - 1;
        // Sort and swap current block
        for (size_t lowerIndex = currentLowerIndex; lowerIndex <= maxLowerIndex; lowerIndex++) {
            size_t higherIndex = lowerIndex + swapDistance;
            HOST_COMPARE_AND_SWAP(data[lowerIndex], data[higherIndex], currentSortOrder);
        }
        // Next block
        currentLowerIndex += info.m_swapBlockSize;
        currentSortOrder = (currentLowerIndex  / info.m_orderKeptBlockSize) % 2 == 0 ? info.m_sortOrder : reversedSortOrder;
    }
}