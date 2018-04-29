//
//  IntSortingNetworkGPUImpl.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 2/17/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include "IntSortingNetworkGPUImpl.h"
#include "../generic/SortingNetworkImpl.cpp"
#include "IKernelArgumentContainer.h"
#include "SimplePrototypedCLEngine.h"

template class SortingNetworkImpl<int>;

IntSortingNetworkGPUImpl::IntSortingNetworkGPUImpl(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                                   const WorkDims& workDims,
                                                   cl_device_type deviceType)
: m_pElementBuffer(NullPtr) {
    
    ParamTypes paramTypes({PT_CONSTANT, PT_CONSTANT, PT_CONSTANT, PT_GLOBAL_INOUT});
    
    OutputParamIndices outputParamsIndices({3});
    
    m_pCLEngine = new SimpleKernelBasedCLEngine(pSimpleExecutorFactory,
                                                "/Users/wws2003/neo-c++/BitonicSortOpenCL/BitonicSortOpenCL/bitonic_swap2.cl",
                                                "bitonic_swap_int1",
                                                workDims,
                                                paramTypes,
                                                outputParamsIndices,
                                                CL_DEVICE_TYPE_GPU);
}

IntSortingNetworkGPUImpl::~IntSortingNetworkGPUImpl() {
    if (m_pElementBuffer != NullPtr) {
        freePtr(m_pElementBuffer);
    }
    delete m_pCLEngine;
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
    
    HostBufferSources outputs({pKernelInOutBuffer});
    
    // Excute kernel for result
    m_pCLEngine->executeCLKernelForResult(inputs, outputs, NULL);
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
