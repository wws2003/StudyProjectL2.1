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

IntSortingNetworkGPUImpl::IntSortingNetworkGPUImpl(const ElementList<int>& elements,
                                                   SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                                   const WorkDims& workDims,
                                                   cl_device_type deviceType)
: SortingNetworkImpl<int>(elements),
m_elementBuffer(elements, true),
m_pSimpleExecutorFactory(pSimpleExecutorFactory),
m_executingDims(workDims),
m_deviceType(deviceType) {
}

void IntSortingNetworkGPUImpl::swap(int swapDistance,
                                    int sortOrderKeptSwapCnt,
                                    SortOrder firstSortOrder) {
    // Program and kernel setting
    std::string srcProgramPath = "/Users/wws2003/neo-c++/BitonicSortOpenCL/BitonicSortOpenCL/bitonic_swap2.cl";
    std::string kernelName = "bitonic_swap_int1";

    // Parameter setting
    IntBuffer kernelSwapDistance(swapDistance);
    IntBuffer kernelOrderKeptSwapCnt(sortOrderKeptSwapCnt);
    IntBuffer isFirstOrderAscending((firstSortOrder == SortOrder::ASC) ? 1 : 0);
    HostBufferSourcePtr pKernelInOutBuffer = &m_elementBuffer;
    
    // Create CL engine
    ConstHostBufferSources inputs({&kernelSwapDistance, &kernelOrderKeptSwapCnt, &isFirstOrderAscending});
    HostBufferSources outputs({pKernelInOutBuffer});
    ParamTypes paramTypes({PT_CONSTANT, PT_CONSTANT, PT_CONSTANT, PT_GLOBAL_INOUT});
    OutputParamIndices outputParamsIndices({3});
    CLEnginePtr pCLEngine = CLEnginePtr(new SimplePrototypedCLEngine(m_pSimpleExecutorFactory,
                                                                     inputs,
                                                                     outputs,
                                                                     paramTypes,
                                                                     outputParamsIndices));
    
    // Excute kernel for result
    // TODO Properly handle timeSpec
    TimeSpec timeSpec;
    pCLEngine->executeCLKernelForResult(srcProgramPath, kernelName, m_executingDims, m_deviceType, &timeSpec);
    
    delete pCLEngine;
}

void IntSortingNetworkGPUImpl::collect(ElementList<int>& elements) const {
    // Copy from internal element buffer
    elements.clear();
    HostBuffer hostBuffer;
    m_elementBuffer.toHostOutputBuffer(hostBuffer);
    // Copy to output parameters
    int* dataArray = (int*)hostBuffer.m_data;
    elements.insert(elements.begin(), dataArray, dataArray + hostBuffer.m_arraySize + 1);
}
