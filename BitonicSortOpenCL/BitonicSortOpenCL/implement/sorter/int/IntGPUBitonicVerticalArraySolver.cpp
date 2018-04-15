
//
//  IntGPUBitonicVerticalArraySolver.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/15/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "IntGPUBitonicVerticalArraySolver.h"
#include "SimplePrototypedCLEngine.h"
#include "Util.h"

IntGPUBitonicVerticalArraySolver::IntGPUBitonicVerticalArraySolver(
                                                                   SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                                                   WorkDims executingDims) : IBitonicVerticalArraySolver<int>(),
m_programName("/Users/wws2003/neo-c++/BitonicSortOpenCL/BitonicSortOpenCL/bitonic_verticalarray_sort.cl"),
m_kernelName("bitonic_vertical_array_solve"),
m_pSimpleExecutorFactory(pSimpleExecutorFactory),
m_executingDims(executingDims) {
    
}

void IntGPUBitonicVerticalArraySolver::solve(const BitonicVerticalArray<int>& bitonicVerticalArray) const {
    
    // Parameter setting
    IntBuffer sortOrder(bitonicVerticalArray.m_sortOrder);
    IntBuffer orderKeptBlockSize((int)bitonicVerticalArray.m_orderKeptBlockSize);
    IntBuffer sortBufferSize((int)bitonicVerticalArray.m_swapBlockSize);
    IntBuffer sortingDepth(bitonicVerticalArray.m_sortingDepth);

    // Local buffer (2D). Memory not required to allocated
    size_t localSize = m_executingDims.getLocalSize(0) * m_executingDims.getLocalSize(1);
    IntBuffer localBuffer(NULL, localSize, true);
    
    // Copy values to inoutBuffer (2D). Any better solution to avoid create and copy ?
    size_t globalSize = bitonicVerticalArray.m_pElementList->size();
    IntBuffer inoutBuffer(globalSize, true);
    collectToInoutBuffer(inoutBuffer, *bitonicVerticalArray.m_pElementList);
    
    // Create CL engine
    ConstHostBufferSources inputs({&sortOrder,
        &orderKeptBlockSize,
        &sortBufferSize,
        &sortingDepth,
        &localBuffer});
    
    HostBufferSources outputs({&inoutBuffer});
    
    ParamTypes paramTypes({PT_CONSTANT,
        PT_CONSTANT,
        PT_CONSTANT,
        PT_CONSTANT,
        PT_LOCAL,
        PT_GLOBAL_INOUT});
    
    OutputParamIndices outputParamsIndices({5});
    
    CLEnginePtr pCLEngine = CLEnginePtr(new SimplePrototypedCLEngine(m_pSimpleExecutorFactory,
                                                                     inputs,
                                                                     outputs,
                                                                     paramTypes,
                                                                     outputParamsIndices));
    
    // Excute kernel for result
    pCLEngine->executeCLKernelForResult(m_programName, m_kernelName, m_executingDims, CL_DEVICE_TYPE_GPU, NULL);
    
    delete pCLEngine;
    
    // Collect results from inout buffer
    collectFromInoutBuffer(inoutBuffer, *bitonicVerticalArray.m_pElementList);
}

void IntGPUBitonicVerticalArraySolver::collectToInoutBuffer(IntBuffer& inoutBuffer, const ElementList<int>& elements) const {
    // Assume the size of element buffer fixed and memory initialized
    size_t eleCount = inoutBuffer.size();
    for (size_t index = 0; index < eleCount; index++) {
        inoutBuffer.set(index, elements[index]);
    }
}


void IntGPUBitonicVerticalArraySolver::collectFromInoutBuffer(const IntBuffer& inoutBuffer, ElementList<int>& elements) const {
    // Copy from internal element buffer
    size_t eleCount = inoutBuffer.size();
    for (size_t index = 0; index < eleCount; index++) {
        elements[index] = inoutBuffer[index];
    }
}
