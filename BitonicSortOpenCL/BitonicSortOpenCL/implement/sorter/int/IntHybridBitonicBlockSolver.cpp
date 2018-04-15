//
//  IntHybridBitonicBlockSolver.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 3/25/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "IntHybridBitonicBlockSolver.h"
#include "SimplePrototypedCLEngine.h"
#include "Util.h"
#include <iostream>

IntHybridBitonicBlockSolver::IntHybridBitonicBlockSolver(
                                                         SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                                         size_t maxWorkGroupSize) : IBitonicBlockSolver<int>(),
m_programName("/Users/wws2003/neo-c++/BitonicSortOpenCL/BitonicSortOpenCL/bitonic_sort2.cl"),
m_kernelName("bitonic_sort_local1"),
m_pSimpleExecutorFactory(pSimpleExecutorFactory),
m_executingDims({maxWorkGroupSize}, {maxWorkGroupSize}),
m_maxWorkGroupSize(maxWorkGroupSize),
m_localBuffer(maxWorkGroupSize, true) {
    
    m_solveFuncMap[MACHINE::DEVICE] = [this](const BitonicBlock<int>& bitonicBlock) {
        solveByGPU(bitonicBlock);
    };
    m_solveFuncMap[MACHINE::HOST] = [this](const BitonicBlock<int>& bitonicBlock) {
        solveByCPU(bitonicBlock);
    };
}

void IntHybridBitonicBlockSolver::solve(const BitonicBlock<int>& bitonicBlock) const {
    // Break down bitonic block to swap operation and sub-blocks.
    // When sub-block size collapses to m_maxWorkGroupSize, use GPU sort
    MACHINE machine = detectProperDeviceTypeToSolve(bitonicBlock.m_lowerIndex,
                                                bitonicBlock.m_higherIndex,
                                                bitonicBlock.m_sortingDepth);
    
    m_solveFuncMap.find(machine)->second(bitonicBlock);
}

MACHINE IntHybridBitonicBlockSolver::detectProperDeviceTypeToSolve(size_t startIndex, size_t endIndex, int sortingDepth) const {
    // TODO Implement more properly
    return (endIndex - startIndex + 1 == m_maxWorkGroupSize) ? MACHINE::DEVICE : MACHINE::HOST;
}

void IntHybridBitonicBlockSolver::solveByCPU(const BitonicBlock<int>& bitonicBlock) const {
    if (bitonicBlock.m_sortingDepth < 1) {
        return;
    }
    
    // Swap
    ElementList<int>& elementList = *(bitonicBlock.m_pElementList);
    size_t elementCnt = bitonicBlock.m_higherIndex - bitonicBlock.m_lowerIndex + 1;
    size_t swapDistance = elementCnt >> 1;
    for(size_t index = bitonicBlock.m_lowerIndex; index + swapDistance <= bitonicBlock.m_higherIndex; index++) {
        size_t higherIndex = index + swapDistance;
        bool toSwap = (bitonicBlock.m_sortOrder == SortOrder::ASC) ? (elementList[higherIndex] < elementList[index]) : (elementList[index] < elementList[higherIndex]);
        if (toSwap) {
            HOST_SWAP(elementList[index], elementList[higherIndex]);
        }
    }
    
    // Recursively solve sub-blocks if needed
    if (bitonicBlock.m_sortingDepth <= 1) {
        return;
    }
    
    size_t subElementCnt = elementCnt >> 1;
    BitonicBlock<int> lowerSubBlock(bitonicBlock.m_sortOrder,
                                    bitonicBlock.m_lowerIndex,
                                    bitonicBlock.m_lowerIndex + subElementCnt - 1,
                                    bitonicBlock.m_sortingDepth - 1,
                                    bitonicBlock.m_pElementList);
    this->solve(lowerSubBlock);
    
    BitonicBlock<int> higherSubBlock(bitonicBlock.m_sortOrder,
                                    bitonicBlock.m_lowerIndex + subElementCnt,
                                    bitonicBlock.m_higherIndex,
                                    bitonicBlock.m_sortingDepth - 1,
                                    bitonicBlock.m_pElementList);
    this->solve(higherSubBlock);
}

void IntHybridBitonicBlockSolver::solveByGPU(const BitonicBlock<int>& bitonicBlock) const {
    // Parameter setting
    IntBuffer itemCount((int)(bitonicBlock.m_higherIndex - bitonicBlock.m_lowerIndex + 1));
    IntBuffer globalOffset(0);
    IntBuffer sortDirection((bitonicBlock.m_sortOrder == SortOrder::ASC) ? 0 : 1);
    
    // Copy values to m_inoutBuffer (do not copy the whole input but the segment to sort only)
    IntBuffer inoutBuffer(m_localBuffer.size(), true);
    collectToInoutBuffer(inoutBuffer, bitonicBlock.m_lowerIndex, *bitonicBlock.m_pElementList);
    
    // Create CL engine
    ConstHostBufferSources inputs({&itemCount,
        &sortDirection,
        &globalOffset,
        &m_localBuffer});
    
    HostBufferSources outputs({&inoutBuffer});
    
    ParamTypes paramTypes({PT_CONSTANT,
        PT_CONSTANT,
        PT_CONSTANT,
        PT_LOCAL,
        PT_GLOBAL_INOUT});
    
    OutputParamIndices outputParamsIndices({4});
    
    CLEnginePtr pCLEngine = CLEnginePtr(new SimplePrototypedCLEngine(m_pSimpleExecutorFactory,
                                                                     inputs,
                                                                     outputs,
                                                                     paramTypes,
                                                                     outputParamsIndices));
    
    // Excute kernel for result
    // TODO Properly handle timeSpec
    TimeSpec timeSpec;
    {
        ScopeTimer scopeTimer(&timeSpec);
        pCLEngine->executeCLKernelForResult(m_programName, m_kernelName, m_executingDims, CL_DEVICE_TYPE_GPU, NULL);
    }
    
    
    // DEBUG
    std::cout << "1,One block of " << m_maxWorkGroupSize << " has solving time (in ms)," << (timeSpec.tv_sec * 1e3 + timeSpec.tv_nsec / 1e6) << "\n";
    
    delete pCLEngine;

    // Collect results from inout buffer
    TimeSpec timeSpec2;
    {
        ScopeTimer scopeTimer(&timeSpec2);
        collectFromInoutBuffer(inoutBuffer, bitonicBlock.m_lowerIndex, *bitonicBlock.m_pElementList);
    }
    // DEBUG
    std::cout << "2,Collect block of " << m_maxWorkGroupSize << " takes time (in ms)," << (timeSpec2.tv_sec * 1e3 + timeSpec2.tv_nsec / 1e6) << "\n";
}

void IntHybridBitonicBlockSolver::collectToInoutBuffer(IntBuffer& inoutBuffer, size_t offset, const ElementList<int>& elements) const {
    // Assume the size of element buffer fixed and memory initialized
    size_t eleCount = inoutBuffer.size();
    for (size_t index = 0; index < eleCount; index++) {
        inoutBuffer.set(index, elements[offset + index]);
    }
}


void IntHybridBitonicBlockSolver::collectFromInoutBuffer(const IntBuffer& inoutBuffer, size_t offset, ElementList<int>& elements) const {
    // Copy from internal element buffer
    size_t eleCount = inoutBuffer.size();
    for (size_t index = 0; index < eleCount; index++) {
        elements[offset + index] = inoutBuffer[index];
    }
}
