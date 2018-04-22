
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

IntGPUBitonicVerticalArraySolver::IntGPUBitonicVerticalArraySolver(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                                                   const WorkDims& executingWorkDims)
: IBitonicVerticalArraySolver<int> (),
m_pElementBuffer(NullPtr),
m_localBuffer(NULL, executingWorkDims.getLocalSize(0) * executingWorkDims.getLocalSize(1), true) {
    
    ParamTypes paramTypes({PT_CONSTANT,
        PT_CONSTANT,
        PT_CONSTANT,
        PT_LOCAL,
        PT_GLOBAL_INOUT});
    
    OutputParamIndices outputParamsIndices({4});
    
    m_pCLEngine = new SimpleKernelBasedCLEngine(pSimpleExecutorFactory,
                                                "/Users/wws2003/neo-c++/BitonicSortOpenCL/BitonicSortOpenCL/bitonic_verticalarray_sort.cl",
                                                "bitonic_vertical_array_solve",
                                                executingWorkDims,
                                                paramTypes,
                                                outputParamsIndices,
                                                CL_DEVICE_TYPE_GPU);
}

IntGPUBitonicVerticalArraySolver::~IntGPUBitonicVerticalArraySolver() {
    if (m_pElementBuffer != NullPtr) {
        freePtr(m_pElementBuffer);
    }
    delete m_pCLEngine;
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

    // Create CL engine
    ConstHostBufferSources inputs({&sortOrder,
        &orderKeptBlockSize,
        &sortBufferSize,
        &m_localBuffer});
    
    HostBufferSources outputs({m_pElementBuffer});
    
    m_pCLEngine->executeCLKernelForResult(inputs, outputs, NULL);
}
