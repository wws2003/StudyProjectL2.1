//
//  CLLocalizedMatrixMultiplierImpl.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "CLLocalizedMatrixMultiplierImpl.h"
#include "SimplePrototypedCLEngine.h"
#include <cassert>

#define LOCALIZE_2D (1)

template<typename T>
CLLocalizedMatrixMultiplierImpl<T>::CLLocalizedMatrixMultiplierImpl(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                                                    const WorkDims& executingWorkDims,
                                    cl_device_type deviceType) : m_workDims(executingWorkDims) {
    m_pSimpleExecutorFactory = pSimpleExecutorFactory;
    m_deviceType = deviceType;
}

template<typename T>
void CLLocalizedMatrixMultiplierImpl<T>::multiplyMatrix(const GenericBuffer<T>& factor1,
                                               const GenericBuffer<T>& factor2,
                                               int nDim,
                                               int pDim,
                                               int mDim,
                                               GenericBuffer<T>& product,
                                               TimeSpec* pTimeSpec) {
    
#ifdef LOCALIZE_2D
    std::string srcProgramPath = "/Users/wws2003/neo-c++/MatrixVectorOperationsOpenCL/OpenCLHelloWorld/kernel/matrix_multiply_2d.cl";
    std::string kernelName = "mmul_localized_2d2";
    
    IntBuffer nDimBuff(nDim);
    IntBuffer pDimBuff(pDim);
    IntBuffer mDimBuff(mDim);
     
    ConstHostBufferSourcePtr pTerm1 = &factor1;
    ConstHostBufferSourcePtr pTerm2 = &factor2;
    GenericBufferPtr<T> pProduct = &product;
    
    GenericBuffer<T> localA(pDim * m_workDims.getLocalSize(0), true);
    GenericBuffer<T> localB(pDim * m_workDims.getLocalSize(1), true);
     
    CLEnginePtr pCLEngine = CLEnginePtr(new SimplePrototypedCLEngine(m_pSimpleExecutorFactory,
     {&nDimBuff, &pDimBuff, &pDimBuff, pTerm1, pTerm2, &localA, &localB},
     {pProduct},
     {PT_CONSTANT, PT_CONSTANT, PT_CONSTANT, PT_GLOBAL_IN, PT_GLOBAL_IN, PT_LOCAL, PT_LOCAL, PT_GLOBAL_OUT},
     {7}));
     
#else
    std::string srcProgramPath = "/Users/wws2003/neo-c++/MatrixVectorOperationsOpenCL/OpenCLHelloWorld/kernel/matrix_multiply_2d.cl";
    std::string kernelName = "mmul_localized_2d1";
    
    IntBuffer nDimBuff(nDim);
    IntBuffer pDimBuff(pDim);
    IntBuffer mDimBuff(mDim);
    
    ConstHostBufferSourcePtr pTerm1 = &factor1;
    ConstHostBufferSourcePtr pTerm2 = &factor2;
    GenericBufferPtr<T> pProduct = &product;
    GenericBuffer<T> localA(pDim, true);
    //GenericBuffer<T> localB(m_workDims.getGlobalSize(0), true);
    
    CLEnginePtr pCLEngine = CLEnginePtr(new SimplePrototypedCLEngine(m_pSimpleExecutorFactory,
                                                                     {&nDimBuff, &pDimBuff, &pDimBuff, pTerm1, pTerm2, &localA},
                                                                     {pProduct},
                                                                     {PT_CONSTANT, PT_CONSTANT, PT_CONSTANT, PT_GLOBAL_IN, PT_GLOBAL_IN, PT_LOCAL, PT_GLOBAL_OUT},
                                                                     {6}));
#endif
    
    pCLEngine->executeCLKernelForResult(srcProgramPath, kernelName, m_workDims, m_deviceType, pTimeSpec);
    
    delete pCLEngine;
}

template<typename T>
void CLLocalizedMatrixMultiplierImpl<T>::verifyResults(const GenericBuffer<T>& factor1,
                                              const GenericBuffer<T>& factor2,
                                              int nDim,
                                              int pDim,
                                              int mDim,
                                              GenericBuffer<T>& product) {
    for (unsigned int row1 = 0; row1 < nDim; row1 ++) {
        for (unsigned int col2 = 0; col2 < mDim; col2++) {
            T innerProduct = 0;
            
            //product[row1][col2]
            unsigned int targetIndex = row1 * mDim + col2;
            
            for (unsigned int col1 = 0; col1 < pDim; col1++) {
                unsigned int row2 = col1;
                //product[row1][col2] += factor1[row1][col1] + factor2[row2][col2]
                innerProduct += factor1[row1 * pDim + col1] * factor2[row2 * mDim + col2];
            }
            assert(innerProduct == product[targetIndex]);
        }
    }
}


template class CLLocalizedMatrixMultiplierImpl<float>;
template class CLLocalizedMatrixMultiplierImpl<int>;