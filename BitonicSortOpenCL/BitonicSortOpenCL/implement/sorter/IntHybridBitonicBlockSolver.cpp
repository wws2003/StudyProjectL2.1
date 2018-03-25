//
//  IntHybridBitonicBlockSolver.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 3/25/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "IntHybridBitonicBlockSolver.h"
#include "SimplePrototypedCLEngine.h"

#define HOST_SWAP(a,b) {int aux = a; a = b; b = aux;}

IntHybridBitonicBlockSolver::IntHybridBitonicBlockSolver(
                                                         SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                                         size_t maxWorkGroupSize) : IBitonicBlockSolver<int>(),
m_programName("/Users/wws2003/neo-c++/BitonicSortOpenCL/BitonicSortOpenCL/bitonic_sort2.cl"),
m_kernelName("bitonic_sort_local1"),
m_pSimpleExecutorFactory(pSimpleExecutorFactory),
m_executingDims({maxWorkGroupSize, maxWorkGroupSize}, {maxWorkGroupSize, maxWorkGroupSize}),
m_maxWorkGroupSize(maxWorkGroupSize) {
    
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
    // TODO Implement properly
    
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
    
    // Recursively solve sub-blocks
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
    IntBuffer globalOffset((int)bitonicBlock.m_lowerIndex);
    IntBuffer sortDirection((bitonicBlock.m_sortOrder == SortOrder::ASC) ? 0 : 1);
    
    // TODO Specify proper param
    //HostBufferSourcePtr pKernelInOutBuffer = &elementBuffer;
    HostBufferSourcePtr pKernelInOutBuffer;
    
    // Create CL engine
    ConstHostBufferSources inputs({&itemCount, &sortDirection, &globalOffset});
    ParamTypes paramTypes({PT_CONSTANT, PT_CONSTANT, PT_CONSTANT});
    
    HostBufferSources outputs({pKernelInOutBuffer});
    OutputParamIndices outputParamsIndices({2});
    
    CLEnginePtr pCLEngine = CLEnginePtr(new SimplePrototypedCLEngine(m_pSimpleExecutorFactory,
                                                                     inputs,
                                                                     outputs,
                                                                     paramTypes,
                                                                     outputParamsIndices));
    
    // Excute kernel for result
    // TODO Properly handle timeSpec
    TimeSpec timeSpec;
    pCLEngine->executeCLKernelForResult(m_programName, m_kernelName, m_executingDims, CL_DEVICE_TYPE_GPU, &timeSpec);
    
    delete pCLEngine;

    // TODO Collect results
    
}


void IntHybridBitonicBlockSolver::collect(const IntBuffer& elementBuffer, size_t offset, ElementList<int>& elements) const {
    // Copy from internal element buffer
    elements.clear();
    HostBuffer hostBuffer;
    elementBuffer.toHostOutputBuffer(hostBuffer);
    // Copy to output parameters
    int* dataArray = (int*)hostBuffer.m_data;
    
    for (size_t index = 0; index < hostBuffer.m_arraySize; index++) {
        elements[offset + index] = dataArray[index];
    }
}
