//
//  CLVectorAdder.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/6/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "CLVectorAdder.h"
#include "SimplePrototypedCLEngine.h"
#include <cassert>
#include <iostream>

template<typename T>
CLVectorAdder<T>::CLVectorAdder(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory, const WorkDims& executingWorkDims, cl_device_type deviceType) : m_workDims(executingWorkDims) {
    
    m_pSimpleExecutorFactory = pSimpleExecutorFactory;
    m_deviceType = deviceType;
}

template<typename T>
void CLVectorAdder<T>::addVector(const GenericBuffer<T>& term1,
                                 const GenericBuffer<T>& term2,
                                 GenericBuffer<T>& sum,
                                 TimeSpec* pTimeSpec) {
    
    std::string srcProgramPath = "/Users/wws2003/neo-c++/MatrixVectorOperationsOpenCL/OpenCLHelloWorld/kernel/HelloWorld.cl";
    std::string kernelName = "addition_kernel";
    
    ConstHostBufferSourcePtr pTerm1 = &term1;
    ConstHostBufferSourcePtr pTerm2 = &term2;
    HostBufferSourcePtr pSum = &sum;
    
    CLEnginePtr pCLEngine = CLEnginePtr(new SimplePrototypedCLEngine(m_pSimpleExecutorFactory,
    {pTerm1, pTerm2},
    {pSum},
    {PT_GLOBAL_IN, PT_GLOBAL_IN, PT_GLOBAL_OUT},
    {2}));
    
    pCLEngine->executeCLKernelForResult(srcProgramPath, kernelName, m_workDims, m_deviceType, pTimeSpec);
    
    delete pCLEngine;
}

template<typename T>
void CLVectorAdder<T>::verifyResults(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, const GenericBuffer<T>& sum) {
    
    for (int i = 0; i < term1.size(); i++) {
        assert(sum[i] == term1[i] + term2[i]);
    }
}

template class CLVectorAdder<float>;
template class CLVectorAdder<int>;