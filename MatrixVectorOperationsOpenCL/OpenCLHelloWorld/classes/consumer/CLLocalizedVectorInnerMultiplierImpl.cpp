//
//  CLLocalizedVectorInnerMultiplierImpl.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "CLLocalizedVectorInnerMultiplierImpl.h"
#include "SimplePrototypedCLEngine.h"
#include "Util.h"
#include <cassert>

template<typename T>
CLLocalizedVectorInnerMultiplierImpl<T>::CLLocalizedVectorInnerMultiplierImpl(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory, const WorkDims& executingWorkDims, cl_device_type deviceType) : m_workDims(executingWorkDims) {
    
    m_pSimpleExecutorFactory = pSimpleExecutorFactory;
    m_deviceType = deviceType;
}


template<typename T>
void CLLocalizedVectorInnerMultiplierImpl<T>::innerMultiply(const GenericBuffer<T>& term1,
                                                            const GenericBuffer<T>& term2,
                                                            T& innerProduct,
                                                            TimeSpec* pTimeSpec) {
    
    std::string srcProgramPath = "/Users/wws2003/neo-c++/MatrixVectorOperationsOpenCL/OpenCLHelloWorld/kernel/vector_inner_product.cl";
    std::string kernelName = "localized_inner_product_kernel2";
    
    ConstHostBufferSourcePtr pTerm1 = &term1;
    ConstHostBufferSourcePtr pTerm2 = &term2;
    GenericBuffer<T> local(m_workDims.getLocalSize(0), true);
    
    size_t groupSize = m_workDims.getGlobalSize(0) / m_workDims.getLocalSize(0);
    
    GenericBuffer<T> partialSum(groupSize, true);
    
    CLEnginePtr pCLEngine = CLEnginePtr(new SimplePrototypedCLEngine(m_pSimpleExecutorFactory,
    {pTerm1, pTerm2, &local},
    {&partialSum},
    {PT_GLOBAL_IN, PT_GLOBAL_IN, PT_LOCAL, PT_GLOBAL_OUT},
    {3}));
    
    pCLEngine->executeCLKernelForResult(srcProgramPath, kernelName, m_workDims, m_deviceType, pTimeSpec);
    
    //Sum up for final results, also count time
    TimeSpec start;
    getTime(&start);
    
    innerProduct = 0;
    for (unsigned int i = 0; i < groupSize; i++) {
        innerProduct += partialSum[i];
    }
    
    TimeSpec end;
    getTime(&end);
    
    if (pTimeSpec != NULL) {
        pTimeSpec->tv_sec += end.tv_sec - start.tv_sec;
        pTimeSpec->tv_nsec += end.tv_nsec - start.tv_nsec;
    }
    
    delete pCLEngine;
    
}

template<typename T>
void CLLocalizedVectorInnerMultiplierImpl<T>::verifyResults(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, const T& innerProduct) {
    T correctInnerProduct = 0;
    for (unsigned int i = 0; i < term1.size(); i++) {
        correctInnerProduct += term1[i] * term2[i];
    }
    assert(correctInnerProduct == innerProduct);
}

template class CLLocalizedVectorInnerMultiplierImpl<float>;
template class CLLocalizedVectorInnerMultiplierImpl<int>;