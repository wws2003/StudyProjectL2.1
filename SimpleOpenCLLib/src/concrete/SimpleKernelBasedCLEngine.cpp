//
//  SimpleKernelBasedCLEngine.cpp
//  SimpleOpenCLLib
//
//  Created by wws2003 on 4/22/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#include "SimpleKernelBasedCLEngine.h"

SimpleKernelBasedCLEngine::SimpleKernelBasedCLEngine(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                                     std::string programSrcPath,
                                                     std::string kernelName,
                                                     const WorkDims& executingWorkDims,
                                                     const ParamTypes& paramTypes,
                                                     const OutputParamIndices& outputParamsIndices,
                                                     cl_device_type deviceType) :
m_pSimpleExecutorFactory(pSimpleExecutorFactory),
m_kernelName(kernelName),
m_executingWorkDims(executingWorkDims),
m_paramTypes(paramTypes) {
    
    // Create executor instance
    m_pCLExecutor = m_pSimpleExecutorFactory->getSimpleCLExecutor(deviceType);
    
    // Initialize prototype for kernel function
    initProgramPrototype(kernelName, outputParamsIndices, m_programPrototype);
    
    // Load program (including build)
    m_pCLExecutor->loadProgram(programSrcPath.data(), m_programPrototype);
}

SimpleKernelBasedCLEngine::~SimpleKernelBasedCLEngine() {
    releaseProgramPrototype(m_programPrototype);
    delete m_pCLExecutor;
}

void SimpleKernelBasedCLEngine::executeCLKernelForResult(ConstHostBufferSources inputs,
                                                         HostBufferSources outputs,
                                                         TimeSpec* pTimeSpec) {
    // Init host input params
    HostInputParams hostInputParams;
    initCLInputParams(inputs, outputs, hostInputParams);
    
    // Submission
    submission_t submission = m_pCLExecutor->submitOnce(m_kernelName.data(), m_executingWorkDims, hostInputParams);
    
    // Init host output params
    HostOutputParams hostOutputParams;
    initCLOutputParams(outputs, hostOutputParams);
    
    // Retrieve result back
    m_pCLExecutor->getResult(submission, hostOutputParams, pTimeSpec);
}

void SimpleKernelBasedCLEngine::initProgramPrototype(std::string kernelName, const OutputParamIndices& outputParamsIndices, ProgramPrototype& programPrototype) {
    programPrototype[kernelName] = KernelPrototypePtr(new KernelPrototype(kernelName, outputParamsIndices));
}

void SimpleKernelBasedCLEngine::releaseProgramPrototype(ProgramPrototype& programPrototype) {
    for (ProgramPrototype::iterator kIter = programPrototype.begin(); kIter != programPrototype.end(); ) {
        KernelPrototypePtr pKernelPrototype = kIter->second;
        delete pKernelPrototype;
        kIter = programPrototype.erase(kIter);
    }
}


void SimpleKernelBasedCLEngine::initCLInputParams(const ConstHostBufferSources& inputs,
                                                  const HostBufferSources& outputs,
                                                  HostInputParams& hostInputParams) {
    unsigned int paramTypeIndex = 0;
    
    // Set input buffers to input params
    size_t inputSize = inputs.size();
    for (unsigned int i = 0; i < inputSize; i++) {
        HostBufferExt inputBuffer;
        inputs[i]->toHostInputBuffer(m_paramTypes[paramTypeIndex++], inputBuffer);
        hostInputParams.emplace_back(inputBuffer);
    }
    // Also set output bufffers to input params so that device can write the result
    size_t outputSize = outputs.size();
    for (unsigned int i = 0; i < outputSize; i++) {
        HostBufferExt inputBuffer;
        outputs[i]->toHostInputBuffer(m_paramTypes[paramTypeIndex++], inputBuffer);
        hostInputParams.emplace_back(inputBuffer);
    }
}

void SimpleKernelBasedCLEngine::initCLOutputParams(HostBufferSources outputs,
                                                   HostOutputParams& hostOutputParams) {
    
    // Only set output bufferes to host outputs
    for (unsigned int i = 0; i < outputs.size(); i++) {
        HostBuffer outputBuffer;
        outputs[i]->toHostOutputBuffer(outputBuffer);
        hostOutputParams.emplace_back(outputBuffer);
    }
}

void SimpleKernelBasedCLEngine::onResultReady(const HostOutputParams& outputs) {
    //In almost cases, do nothing since output has been prepared by sub-class and therefore should be accessible from sub-class
}

void SimpleKernelBasedCLEngine::releaseHostParams(HostInputParams& hostInputParams, HostOutputParams& hostOutputParams) {
    //Do nothing as there is no memory allocation here
}