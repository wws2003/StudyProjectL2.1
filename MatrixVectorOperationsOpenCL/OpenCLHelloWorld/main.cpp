//
//  main.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 1/11/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "SimpleCLExecutorFactoryImpl.h"
#include "SimpleCLProgramFactoryImpl.h"
#include "SequentialVectorAdder.h"
#include "CLVectorAdder.h"
#include "CLLocalizedVectorInnerMultiplierImpl.h"
#include "SequentialVectorInnerMultiplierImpl.h"
#include "CLVectorInnerMultiplierImpl.h"
#include "SequentialMatrixMultiplierImpl.h"
#include "CLMatrixMultiplierImpl.h"
#include "CLLocalizedMatrixMultiplierImpl.h"
#include <iostream>

#define ARRAY_SIZE  (131072)
#define LOCAL_SIZE  (512)
#define SMALL_ARRAY_SIZE (512)
#define SMALL_LOCAL_SIZE (16)

//-----------For vector sum---------------------//
void evalVectorAdders();

void initParamsByHost(FloatBuffer *pParamSet1, FloatBuffer *pParamSet2, FloatBuffer* pResultsSet);

FloatVectorAdderPtr getGPUVectorAdder(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory);

FloatVectorAdderPtr getCPUVectorAdder(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory);

FloatVectorAdderPtr getSequentialVectorAdder();

void evaluateAdder(FloatVectorAdderPtr pVectorAdder,
                   const FloatBuffer& aBuff,
                   const FloatBuffer& bBuff,
                   FloatBuffer& cBuff,
                   std::string message);

//-----------For vector inner product---------------------//
void evalVectorMultipliers();

void initParamsByHost(FloatBuffer *pParamSet1, FloatBuffer *pParamSet2, float* pInnerProduct);

FloatVectorInnerMultiplierPtr getGPUVectorMultiplier(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory, bool localized);

FloatVectorInnerMultiplierPtr getCPUVectorMultiplier(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory, bool localized);

FloatVectorInnerMultiplierPtr getSequentialVectorMultiplier();

void evaluateVectorInnerMultiplier(FloatVectorInnerMultiplierPtr pVectorInnerMultiplier,
                   const FloatBuffer& aBuff,
                   const FloatBuffer& bBuff,
                   float& result,
                   std::string message);

//-----------For matrix product---------------------//
void evalMatrixMultipliers();

void initParamsByHost(FloatBuffer& factor1,
                      FloatBuffer& factor2,
                      int nDim,
                      int pDim,
                      int mDim,
                      FloatBuffer& product);

FloatMatrixMultiplierPtr getGPUMatrixMultiplier(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory, bool localized);

FloatMatrixMultiplierPtr getCPUMatrixMultiplier(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory, bool localized);

FloatMatrixMultiplierPtr getSequentiaMatrixMultiplier();


void evaluateMatrixMultiplier(FloatMatrixMultiplierPtr pMatrixMultiplier,
                        const FloatBuffer& aBuff,
                        const FloatBuffer& bBuff,
                        int nDim,
                        int pDim,
                        int mDim,
                        FloatBuffer& product,
                        std::string message);

//-----------Misc----------------------//
void printTimeSpec(std::string prefixMessage, TimeSpec& timeSpec);

int main(int argc, const char * argv[]) {
    // insert code here...
    
    std::cout << "Hello, World!\n";
    
    std::cout << "\n=======Evaluating vector adder=======\n";
    evalVectorAdders();
    
    std::cout << "\n=======Evaluating vector multiplier=======\n";
    evalVectorMultipliers();
    
    std::cout << "\n=======Evaluating matrix multiplier=======\n";
    evalMatrixMultipliers();
    
    return 0;
}


//-----------For vector sum---------------------//
void evalVectorAdders() {
    FloatBuffer aBuff(ARRAY_SIZE, true), bBuff(ARRAY_SIZE, true), cBuff(ARRAY_SIZE, true);
    initParamsByHost(&aBuff, &bBuff, &cBuff);
    
    SimpleCLProgramFactoryPtr pProgramFactory = SimpleCLProgramFactoryPtr(new SimpleCLProgramFactoryImpl());
    SimpleCLExecutorFactoryPtr pSimpleExecutorFactory = SimpleCLExecutorFactoryPtr(new SimpleCLExecutorFactoryImpl(pProgramFactory));
    
    FloatVectorAdderPtr pGPUVectorAdder = getGPUVectorAdder(pSimpleExecutorFactory);
    evaluateAdder(pGPUVectorAdder, aBuff, bBuff, cBuff, "GPU Adder");
    
    initParamsByHost(&aBuff, &bBuff, &cBuff);
    FloatVectorAdderPtr pCPUVectorAdder = getCPUVectorAdder(pSimpleExecutorFactory);
    evaluateAdder(pCPUVectorAdder, aBuff, bBuff, cBuff, "CPU Adder");
    
    initParamsByHost(&aBuff, &bBuff, &cBuff);
    FloatVectorAdderPtr pSequentialVectorAdder = getSequentialVectorAdder();
    evaluateAdder(pSequentialVectorAdder, aBuff, bBuff, cBuff, "Sequential Adder");
    
    delete pSequentialVectorAdder;
    delete pCPUVectorAdder;
    delete pGPUVectorAdder;
    
    delete pSimpleExecutorFactory;
    delete pProgramFactory;
}

void initParamsByHost(FloatBuffer *pParamSet1, FloatBuffer *pParamSet2, FloatBuffer* pResultsSet) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        pParamSet1->set(i, i);
        pParamSet2->set(i, i * 2);
        pResultsSet->set(i, 0);
    }
}

FloatVectorAdderPtr getGPUVectorAdder(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory) {
    cl_device_type deviceType = CL_DEVICE_TYPE_GPU;
    
    WorkDims executingWorkDims(ARRAY_SIZE, LOCAL_SIZE);
    
    return FloatVectorAdderPtr(new CLVectorAdder<float>(pSimpleExecutorFactory, executingWorkDims, deviceType));
}

FloatVectorAdderPtr getCPUVectorAdder(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory) {
    cl_device_type deviceType = CL_DEVICE_TYPE_CPU;
    
    WorkDims executingWorkDims(ARRAY_SIZE, 1);
    
    return FloatVectorAdderPtr(new CLVectorAdder<float>(pSimpleExecutorFactory, executingWorkDims, deviceType));
}

FloatVectorAdderPtr getSequentialVectorAdder() {
    return FloatVectorAdderPtr(new SequentialVectorAdder<float>());
}

void evaluateAdder(FloatVectorAdderPtr pVectorAdder,
                   const FloatBuffer& aBuff,
                   const FloatBuffer& bBuff,
                   FloatBuffer& cBuff,
                   std::string message) {
    TimeSpec timeSpec;
    pVectorAdder->addVector(aBuff, bBuff, cBuff, &timeSpec);
    pVectorAdder->verifyResults(aBuff, bBuff, cBuff);
    printTimeSpec(message, timeSpec);
}

//-----------For vector inner product---------------------//

void evalVectorMultipliers() {
    FloatBuffer aBuff(ARRAY_SIZE, true), bBuff(ARRAY_SIZE, true);
    float result;
    initParamsByHost(&aBuff, &bBuff, &result);
    
    SimpleCLProgramFactoryPtr pProgramFactory = SimpleCLProgramFactoryPtr(new SimpleCLProgramFactoryImpl());
    SimpleCLExecutorFactoryPtr pSimpleExecutorFactory = SimpleCLExecutorFactoryPtr(new SimpleCLExecutorFactoryImpl(pProgramFactory));
    
    FloatVectorInnerMultiplierPtr pGPUInnerMultiplier = getGPUVectorMultiplier(pSimpleExecutorFactory, false);
    evaluateVectorInnerMultiplier(pGPUInnerMultiplier, aBuff, bBuff, result, "GPU Inner Multiplier");
    
    FloatVectorInnerMultiplierPtr pGPULocalizedInnerMultiplier = getGPUVectorMultiplier(pSimpleExecutorFactory, true);
    evaluateVectorInnerMultiplier(pGPULocalizedInnerMultiplier, aBuff, bBuff, result, "GPU Localized Inner Multiplier");
    
    FloatVectorInnerMultiplierPtr pCPUInnerMultiplier = getCPUVectorMultiplier(pSimpleExecutorFactory, false);
    evaluateVectorInnerMultiplier(pCPUInnerMultiplier, aBuff, bBuff, result, "CPU Inner Multiplier");
    
    FloatVectorInnerMultiplierPtr pCPULocalizedInnerMultiplier = getCPUVectorMultiplier(pSimpleExecutorFactory, true);
    evaluateVectorInnerMultiplier(pCPULocalizedInnerMultiplier, aBuff, bBuff, result, "CPU Localized Inner Multiplier");
    
    FloatVectorInnerMultiplierPtr pSequentialMultiplier = getSequentialVectorMultiplier();
    evaluateVectorInnerMultiplier(pSequentialMultiplier, aBuff, bBuff, result, "Sequential Multiplier");
    
    delete pSequentialMultiplier;
    delete pGPULocalizedInnerMultiplier;
    delete pCPUInnerMultiplier;
    delete pCPULocalizedInnerMultiplier;
    delete pGPUInnerMultiplier;
    
    delete pSimpleExecutorFactory;
    delete pProgramFactory;
}

void initParamsByHost(FloatBuffer *pParamSet1, FloatBuffer *pParamSet2, float* pInnerProduct) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        pParamSet1->set(i, i % 11);
        pParamSet2->set(i, (i * 2) % 7);
    }
    *pInnerProduct = 0;
}


FloatVectorInnerMultiplierPtr getGPUVectorMultiplier(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory, bool localized) {
    
    cl_device_type deviceType = CL_DEVICE_TYPE_GPU;
    
    WorkDims executingWorkDims(ARRAY_SIZE, LOCAL_SIZE);
    
    if (localized) {
        return FloatVectorInnerMultiplierPtr(new CLLocalizedVectorInnerMultiplierImpl<float>(pSimpleExecutorFactory, executingWorkDims, deviceType));
    }
    else {
        return FloatVectorInnerMultiplierPtr(new CLVectorInnerMultiplierImpl<float>(pSimpleExecutorFactory, executingWorkDims, deviceType));
    }
}

FloatVectorInnerMultiplierPtr getCPUVectorMultiplier(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory, bool localized) {
    cl_device_type deviceType = CL_DEVICE_TYPE_CPU;
    
    WorkDims executingWorkDims(ARRAY_SIZE, 1);
    
    if (localized) {
        return FloatVectorInnerMultiplierPtr(new CLLocalizedVectorInnerMultiplierImpl<float>(pSimpleExecutorFactory, executingWorkDims, deviceType));
    }
    else {
        return FloatVectorInnerMultiplierPtr(new CLVectorInnerMultiplierImpl<float>(pSimpleExecutorFactory, executingWorkDims, deviceType));
    }
}

FloatVectorInnerMultiplierPtr getSequentialVectorMultiplier() {
    return FloatVectorInnerMultiplierPtr(new SequentialVectorInnerMultiplierImpl<float>());
}

void evaluateVectorInnerMultiplier(FloatVectorInnerMultiplierPtr pVectorInnerMultiplier,
                        const FloatBuffer& aBuff,
                        const FloatBuffer& bBuff,
                        float& result,
                        std::string message) {
    TimeSpec timeSpec;
    pVectorInnerMultiplier->innerMultiply(aBuff, bBuff, result, &timeSpec);
    pVectorInnerMultiplier->verifyResults(aBuff, bBuff, result);
    printTimeSpec(message, timeSpec);
}

//-----------For matrix product---------------------//
void evalMatrixMultipliers() {
    FloatBuffer aBuff(SMALL_ARRAY_SIZE * SMALL_ARRAY_SIZE, true), bBuff(SMALL_ARRAY_SIZE * SMALL_ARRAY_SIZE, true), result(SMALL_ARRAY_SIZE * SMALL_ARRAY_SIZE, true);
    
    initParamsByHost(aBuff, bBuff, SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE, result);
    
    SimpleCLProgramFactoryPtr pProgramFactory = SimpleCLProgramFactoryPtr(new SimpleCLProgramFactoryImpl());
    SimpleCLExecutorFactoryPtr pSimpleExecutorFactory = SimpleCLExecutorFactoryPtr(new SimpleCLExecutorFactoryImpl(pProgramFactory));
    
    FloatMatrixMultiplierPtr pGPUInnerMultiplier = getGPUMatrixMultiplier(pSimpleExecutorFactory, false);
    evaluateMatrixMultiplier(pGPUInnerMultiplier, aBuff, bBuff, SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE, result, "GPU Inner Multiplier");
    
    FloatMatrixMultiplierPtr pGPULocalizedInnerMultiplier = getGPUMatrixMultiplier(pSimpleExecutorFactory, true);
    evaluateMatrixMultiplier(pGPULocalizedInnerMultiplier, aBuff, bBuff, SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE, result, "GPU Localized Multiplier");
    
    FloatMatrixMultiplierPtr pCPUInnerMultiplier = getCPUMatrixMultiplier(pSimpleExecutorFactory, false);
    //evaluateMatrixMultiplier(pCPUInnerMultiplier, aBuff, bBuff, SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE, result, "CPU Multiplier");
    
    FloatMatrixMultiplierPtr pCPULocalizedInnerMultiplier = getCPUMatrixMultiplier(pSimpleExecutorFactory, true);
    //evaluateMatrixMultiplier(pCPULocalizedInnerMultiplier, aBuff, bBuff, SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE, result, "CPU Localized Multiplier");
    
    FloatMatrixMultiplierPtr pSequentialMultiplier = getSequentiaMatrixMultiplier();
    evaluateMatrixMultiplier(pSequentialMultiplier, aBuff, bBuff, SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE, result, "Sequential Multiplier");
    
    delete pSequentialMultiplier;
    delete pGPULocalizedInnerMultiplier;
    delete pCPUInnerMultiplier;
    delete pCPULocalizedInnerMultiplier;
    delete pGPUInnerMultiplier;
    
    delete pSimpleExecutorFactory;
    delete pProgramFactory;

}

void initParamsByHost(FloatBuffer& factor1,
                      FloatBuffer& factor2,
                      int nDim,
                      int pDim,
                      int mDim,
                      FloatBuffer& product) {
    //Factor1 nDim * pDim
    for (unsigned int i = 0; i < nDim * pDim; i++) {
        factor1.set(i, i % 11);
    }
    
    //Factor2 pDim * mDim
    for (unsigned int i = 0; i < pDim * mDim; i++) {
        factor2.set(i, (i * 2) % 7);
    }
    
    //Product nDim * mDim
    for (unsigned int i = 0; i < nDim * mDim; i++) {
        product.set(i, 0);
    }
}

FloatMatrixMultiplierPtr getGPUMatrixMultiplier(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory, bool localized) {
    
    cl_device_type deviceType = CL_DEVICE_TYPE_GPU;
    
    if (localized) {
        WorkDims executingWorkDims({SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE}, {SMALL_LOCAL_SIZE, SMALL_LOCAL_SIZE});
        return FloatMatrixMultiplierPtr(new CLLocalizedMatrixMultiplierImpl<float>(pSimpleExecutorFactory, executingWorkDims, deviceType));
    }
    else {
        WorkDims executingWorkDims({SMALL_ARRAY_SIZE, SMALL_ARRAY_SIZE}, {SMALL_LOCAL_SIZE, SMALL_LOCAL_SIZE});
        return FloatMatrixMultiplierPtr(new CLMatrixMultiplierImpl<float>(pSimpleExecutorFactory, executingWorkDims, deviceType));
    }
}

FloatMatrixMultiplierPtr getCPUMatrixMultiplier(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory, bool localized) {
    cl_device_type deviceType = CL_DEVICE_TYPE_CPU;

    WorkDims executingWorkDims (SMALL_ARRAY_SIZE, 1);
    
    if (localized) {
        return FloatMatrixMultiplierPtr(new CLLocalizedMatrixMultiplierImpl<float>(pSimpleExecutorFactory, executingWorkDims, deviceType));
    }
    else {
        return FloatMatrixMultiplierPtr(new CLMatrixMultiplierImpl<float>(pSimpleExecutorFactory, executingWorkDims, deviceType));
    }
}

FloatMatrixMultiplierPtr getSequentiaMatrixMultiplier() {
    return FloatMatrixMultiplierPtr(new SequentialMatrixMultiplierImpl<float>());
}

void evaluateMatrixMultiplier(FloatMatrixMultiplierPtr pMatrixMultiplier,
                              const FloatBuffer& aBuff,
                              const FloatBuffer& bBuff,
                              int nDim,
                              int pDim,
                              int mDim,
                              FloatBuffer& product,
                              std::string message) {
    TimeSpec timeSpec;
    pMatrixMultiplier->multiplyMatrix(aBuff, bBuff, nDim, pDim, mDim, product, &timeSpec);
    pMatrixMultiplier->verifyResults(aBuff, bBuff, nDim, pDim, mDim, product);
    printTimeSpec(message, timeSpec);
}

//-----------Misc----------------------//
void printTimeSpec(std::string prefixMessage, TimeSpec& timeSpec) {
    std::cout << prefixMessage  << ":\n----" << timeSpec.tv_sec << " s and " << timeSpec.tv_nsec << " ns\n";
}