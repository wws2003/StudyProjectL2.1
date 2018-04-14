//
//  IntSortingNetworkGPUImpl.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 2/17/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "IntSortingNetworkGPUImpl.h"
#include "SortingNetworkImpl.cpp"
#include "IKernelArgumentContainer.h"
#include "SimplePrototypedCLEngine.h"

template class SortingNetworkImpl<int>;

IntSortingNetworkGPUImpl::IntSortingNetworkGPUImpl(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                                   const WorkDims& workDims,
                                                   cl_device_type deviceType)
: m_programName("/Users/wws2003/neo-c++/BitonicSortOpenCL/BitonicSortOpenCL/bitonic_swap2.cl"),
m_kernelName("bitonic_swap_int1"),
m_pElementBuffer(NullPtr),
m_pSimpleExecutorFactory(pSimpleExecutorFactory),
m_executingDims(workDims),
m_deviceType(deviceType){
}

IntSortingNetworkGPUImpl::~IntSortingNetworkGPUImpl() {
    if (m_pElementBuffer != NullPtr) {
        freePtr(m_pElementBuffer);
    }
}

void IntSortingNetworkGPUImpl::set(const ElementList<int>& elements) {
    SortingNetworkImpl<int>::set(elements);
    if (m_pElementBuffer != NullPtr) {
        freePtr(m_pElementBuffer);
    }
    m_pElementBuffer = new IntBuffer(elements, true);
}

void IntSortingNetworkGPUImpl::swap(int swapDistance,
                                    int sortOrderKeptSwapCnt,
                                    SortOrder firstSortOrder) {
    // Parameter setting
    IntBuffer kernelSwapDistance(swapDistance);
    IntBuffer kernelOrderKeptSwapCnt(sortOrderKeptSwapCnt);
    IntBuffer isFirstOrderAscending((firstSortOrder == SortOrder::ASC) ? 1 : 0);
    HostBufferSourcePtr pKernelInOutBuffer = m_pElementBuffer;
    
    // Create CL engine
    ConstHostBufferSources inputs({&kernelSwapDistance,
        &kernelOrderKeptSwapCnt,
        &isFirstOrderAscending});
    ParamTypes paramTypes({PT_CONSTANT, PT_CONSTANT, PT_CONSTANT, PT_GLOBAL_INOUT});
    
    HostBufferSources outputs({pKernelInOutBuffer});
    OutputParamIndices outputParamsIndices({3});
    
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
}

void IntSortingNetworkGPUImpl::collect(ElementList<int>& elements) const {
    // Copy from internal element buffer
    elements.clear();
    HostBuffer hostBuffer;
    m_pElementBuffer->toHostOutputBuffer(hostBuffer);
    // Copy to output parameters
    int* dataArray = (int*)hostBuffer.m_data;
    elements.insert(elements.begin(), dataArray, dataArray + hostBuffer.m_arraySize);
}