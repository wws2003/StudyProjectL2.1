//
//  SimplePrototypedCLEngine.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/6/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "SimplePrototypedCLEngine.h"

SimplePrototypedCLEngine::SimplePrototypedCLEngine(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                                      ConstHostBufferSources inputs,
                                                      HostBufferSources outputs,
                                                      ParamTypes paramTypes,
                                                      OutputParamIndices outputParamsIndices) : AbstractCLEngine(pSimpleExecutorFactory) {
    m_inputs = inputs;
    m_outputs = outputs;
    m_paramTypes = paramTypes;
    m_outputParamsIndices = outputParamsIndices;
}

void SimplePrototypedCLEngine::initProgramPrototype(std::string kernelName, ProgramPrototype& programPrototype) {
    programPrototype[kernelName] = KernelPrototypePtr(new KernelPrototype(kernelName, m_outputParamsIndices));
}

void SimplePrototypedCLEngine::initCLInputParams(HostInputParams& hostInputParams) {
    unsigned int paramTypeIndex = 0;
    
    // Set input buffers to input params
    size_t inputSize = m_inputs.size();
    for (unsigned int i = 0; i < inputSize; i++) {
        HostBufferExt inputBuffer;
        m_inputs[i]->toHostInputBuffer(m_paramTypes[paramTypeIndex++], inputBuffer);
        hostInputParams.emplace_back(inputBuffer);
    }
    // Also set output bufffers to input params so that device can write the result
    size_t outputSize = m_outputs.size();
    for (unsigned int i = 0; i < outputSize; i++) {
        HostBufferExt inputBuffer;
        m_outputs[i]->toHostInputBuffer(m_paramTypes[paramTypeIndex++], inputBuffer);
        hostInputParams.emplace_back(inputBuffer);
    }
}

void SimplePrototypedCLEngine::initCLOutputParams(HostOutputParams& outputs) {
    
    // Only set output bufferes to host outputs
    for (unsigned int i = 0; i < m_outputs.size(); i++) {
        HostBuffer outputBuffer;
        m_outputs[i]->toHostOutputBuffer(outputBuffer);
        outputs.push_back(outputBuffer);
    }
}

void SimplePrototypedCLEngine::releaseHostParams(HostInputParams& hostInputParams, HostOutputParams& hostOutputParams) {
    //Do nothing as there is no memory allocation here
}

