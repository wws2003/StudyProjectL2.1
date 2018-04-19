
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
#include <exception>

IntGPUBitonicVerticalArraySolver::IntGPUBitonicVerticalArraySolver(
                                                                   SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                                                   WorkDims executingDims) : IBitonicVerticalArraySolver<int>(),
m_programName("/Users/wws2003/neo-c++/BitonicSortOpenCL/BitonicSortOpenCL/bitonic_verticalarray_sort.cl"),
m_kernelName("bitonic_vertical_array_solve"),
m_pSimpleExecutorFactory(pSimpleExecutorFactory),
m_executingDims(executingDims),
m_pElementBuffer(NullPtr),
m_localBuffer(NULL, executingDims.getLocalSize(0) * executingDims.getLocalSize(1), true) {
    
}

IntGPUBitonicVerticalArraySolver::~IntGPUBitonicVerticalArraySolver() {
    if (m_pElementBuffer != NullPtr) {
        freePtr(m_pElementBuffer);
    }
}

void IntGPUBitonicVerticalArraySolver::accept(const BitonicVerticalArrayData<int>& data) {
    if (m_pElementBuffer != NullPtr) {
        freePtr(m_pElementBuffer);
    }
    m_pElementBuffer = new IntBuffer(data.m_data, data.m_size);
}

void IntGPUBitonicVerticalArraySolver::solve(const BitonicVerticalArrayInfo<int>& info) {
    
    // Validation check
    if (m_pElementBuffer == NullPtr) {
        throw std::invalid_argument("Internal data empty");
    }
    
    // Parameter setting
    IntBuffer sortOrder(info.m_sortOrder);
    IntBuffer orderKeptBlockSize((int)info.m_orderKeptBlockSize);
    IntBuffer sortBufferSize((int)info.m_swapBlockSize);
    IntBuffer sortingDepth(info.m_sortingDepth);

    // Create CL engine
    ConstHostBufferSources inputs({&sortOrder,
        &orderKeptBlockSize,
        &sortBufferSize,
        &sortingDepth,
        &m_localBuffer});
    
    HostBufferSources outputs({m_pElementBuffer});
    
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
}
