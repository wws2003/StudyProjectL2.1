//
//  SimpleBitonicBlockBasedSorter.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 3/25/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "SimpleBitonicBlockBasedSorter.h"

template<typename T>
SimpleBitonicBlockBasedSorter<T>::SimpleBitonicBlockBasedSorter(BitonicBlockSolverPtr<T> pBitonicBlockSolver) : m_pBitonicBlockSolver(pBitonicBlockSolver){
}

template<typename T>
void SimpleBitonicBlockBasedSorter<T>::sort(const ElementList<T>& inElements,
                                       ElementList<T>& outElements,
                                       SortOrder sortOrder) const
{
    size_t elementCnt = inElements.size();
    
    // Check validation
    validateElementsCount(elementCnt);
    
    // First assign output elements to the input ones
    outElements = inElements;
    
    // Then simply (naively) create all bitonic block at once then assign to the solvers
    // I.e. make no room for task balancing
    std::vector<BitonicBlock<T>> bitonicBlocks;
    bitonicBlocks.reserve(elementCnt - 1);
    
    int bitonicBlockSize = 2;
    int sortingDepth = 1;
    SortOrder reverseSortOrder = (sortOrder == SortOrder::ASC) ? SortOrder::DESC : SortOrder::DESC;
    
    while(bitonicBlockSize <= elementCnt) {
        // (n/2) Block 2, (n/4) Block 4, .... ORDER MATTER !
        size_t startIndex = 0;
        size_t endIndex = bitonicBlockSize - 1;
        // Traverse low to high index
        int step = 0;
        while (endIndex < elementCnt) {
            // Add block to list
            SortOrder blockSortOrder = (step++ % 2) == 0 ? sortOrder : reverseSortOrder;
            bitonicBlocks.emplace_back(blockSortOrder,
                                       startIndex,
                                       endIndex,
                                       sortingDepth,
                                       &outElements);
            // To next block
            startIndex += bitonicBlockSize;
            endIndex += bitonicBlockSize;
        }
        bitonicBlockSize <<= 1;
        sortingDepth++;
    }
    
    // Solve the blocks (also at once)
    for (BitonicBlock<T> bitonicBlock : bitonicBlocks) {
        m_pBitonicBlockSolver->solve(bitonicBlock);
    }
    
    // Collect results not needed since output elements effectively modified by solver
}

template<typename T>
void SimpleBitonicBlockBasedSorter<T>::validateElementsCount(size_t elementCnt) const {
    while(elementCnt > 1) {
        if (elementCnt % 2 != 0) {
            throw std::invalid_argument("Currently only accept list having number of elements is power of 2");
        }
        elementCnt /= 2;
    }
}