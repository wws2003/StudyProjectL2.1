//
//  IntBitonicGPUSorter.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 3/17/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "IntBitonicGPUSorter.h"
#include "../../network/generic/SortingNetworkImpl.cpp"
#include "IKernelArgumentContainer.h"
#include "SimplePrototypedCLEngine.h"

IntBitonicGPUSorter::IntBitonicGPUSorter(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                         const WorkDims& workDims,
                                         cl_device_type deviceType) :
m_programName("/Users/wws2003/neo-c++/BitonicSortOpenCL/BitonicSortOpenCL/bitonic_sort.cl"),
m_kernelName("bitonic_sort_global_naive"),
m_pSimpleExecutorFactory(pSimpleExecutorFactory),
m_executingDims(workDims),
m_deviceType(deviceType) {
    
}

void IntBitonicGPUSorter::sort(const ElementList<int>& inElements,
                               ElementList<int>& outElements,
                               SortOrder sortOrder) const {
    
    size_t elementCnt = inElements.size();
    
    // Validate parameter first
    this->validateElementsCount(elementCnt);
    
    // Data buffer for GPU program (used both for input and output)
    IntBuffer elementBuffer(inElements, true);
    
    // Non-recursive approach: BM2 -> BM4 -> BM8 -> ..-> BM(n)
    int stageElementCnt = 2;
    while(stageElementCnt <= elementCnt) {
        int currentStateElementCnt = stageElementCnt;
        // BM2 = Swap
        // BM4 = Swap and then BM2
        // BM8 = Swap and then BM4 and then BM2
        // ...
        
        // Parameter setting
        IntBuffer inc(currentStateElementCnt / 2);
        IntBuffer dir((sortOrder == SortOrder::ASC) ? 1 : 0);
        HostBufferSourcePtr pKernelInOutBuffer = &elementBuffer;
        
        // Create CL engine
        ConstHostBufferSources inputs({&inc, &dir});
        ParamTypes paramTypes({PT_CONSTANT, PT_CONSTANT, PT_GLOBAL_INOUT});
        
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
        pCLEngine->executeCLKernelForResult(m_programName, m_kernelName, m_executingDims, m_deviceType, &timeSpec);
        
        delete pCLEngine;
        
        stageElementCnt *= 2;
    }

    // Collect final results
    collect(elementBuffer, outElements);
}

void IntBitonicGPUSorter::validateElementsCount(size_t elementCnt) const {
    while(elementCnt > 1) {
        if (elementCnt % 2 != 0) {
            throw std::invalid_argument("Currently only accept list having number of elements is power of 2");
        }
        elementCnt /= 2;
    }
}

void IntBitonicGPUSorter::collect(const IntBuffer& elementBuffer, ElementList<int>& elements) const {
    // Copy from internal element buffer
    elements.clear();
    HostBuffer hostBuffer;
    elementBuffer.toHostOutputBuffer(hostBuffer);
    // Copy to output parameters
    int* dataArray = (int*)hostBuffer.m_data;
    elements.insert(elements.begin(), dataArray, dataArray + hostBuffer.m_arraySize);
}

