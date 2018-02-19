//
//  SimpleCLProgramImpl.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/2/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "SimpleCLProgramImpl.h"
#include "Util.h"
#include <sstream>

SimpleCLProgramImpl::SimpleCLProgramImpl(cl_context context,
                                         cl_program builtProgram,
                                         KernelArgumentContainerFactoryPtr pKernelArgumentContainerFactory,
                                         const ProgramPrototype& programPrototype) {
    m_context = context;
    m_builtProgram = builtProgram;
    m_pKernelArgumentContainerFactory = pKernelArgumentContainerFactory;
    m_programPrototype = programPrototype;
}

SimpleCLProgramImpl::~SimpleCLProgramImpl() {
    releaseKernelExecutables();
    releaseProgram();
}

//Any void function would throw exception if doesn't successfully operated

//Implement
cl_kernel SimpleCLProgramImpl::getKernelExecutable(const submission_t& submission,
                                                   const char* kernelName,
                                                   const HostInputParams& inputParams) {
    //Firstly release kernel resource
    releaseKernelExecutable(submission);
    
    cl_kernel kernel = getKernelFromName(kernelName);
    
    m_kernelExecutableMap[submission] = getKernelExecutable(kernel, kernelName, inputParams);
    
    return kernel;
}

//Implement
void SimpleCLProgramImpl::getKernelExecutableOutputBufferParams(const submission_t& submission,
                                                                cl_command_queue commandQueue,
                                                                HostOutputParams& outputParams) const {
    
    unsigned int paramIndex = 0;
    
    KernelExecutableMap::const_iterator kIter = m_kernelExecutableMap.find(submission);
    if (kIter != m_kernelExecutableMap.end()) {
        KernelExecutablePtr pKernelExecutableMap = kIter->second;
        size_t nbOutputParams = pKernelExecutableMap->m_prototype.m_outputParamIndices.size();
        
        for (unsigned int i = 0; i < nbOutputParams; i++) {
            unsigned int outputParamIndex = pKernelExecutableMap->m_prototype.m_outputParamIndices[i];
            KernelArgumentContainerPtr pKernelArgumentContainer = pKernelExecutableMap->m_argumentContainers[outputParamIndex];
            
            //Read to output params values from kernel output argument
            pKernelArgumentContainer->readKernelArgument(commandQueue, outputParams[paramIndex].m_data, outputParams[paramIndex].m_arraySize *  outputParams[paramIndex].m_elementSize);
            
            paramIndex++;
        }
    }
}

//Implement
void SimpleCLProgramImpl::releaseKernelExecutable(const submission_t& submission) {
    
    KernelExecutableMap::const_iterator kIter = m_kernelExecutableMap.find(submission);
    
    if (kIter != m_kernelExecutableMap.end()) {
        KernelExecutablePtr pKernelExecutableMap = kIter->second;
        delete pKernelExecutableMap;
        m_kernelExecutableMap.erase(kIter);
    }
}

//--------------------MARK: Below are private methods--------------------//

KernelExecutablePtr SimpleCLProgramImpl::getKernelExecutable(cl_kernel kernel,
                                                             const char* kernelName,
                                                             const HostInputParams& inputParams) {
    //Prepare memory buffer, set arguments
    KernelArgumentContainers kernelArgumentContainers;
    createKernelBuffersForInputParams(inputParams, kernelArgumentContainers);
    setKernelBufferArguments(kernel, kernelArgumentContainers);
    
    //Get kernel prototype
    std::string kernelNameStr(kernelName);
    KernelPrototypePtr pKernelProtoType = m_programPrototype[kernelNameStr];
    
    KernelExecutablePtr pKernelExecutable = KernelExecutablePtr(new KernelExecutable(kernel, *pKernelProtoType, kernelArgumentContainers));
    
    return pKernelExecutable;
}

void SimpleCLProgramImpl::createKernelBuffersForInputParams(const HostInputParams& inputParams,
                                   KernelArgumentContainers& kernelArgumentContainers) {
    
    kernelArgumentContainers.clear();
    
    for (unsigned int i = 0; i < inputParams.size(); i++) {
        HostBufferExt hostInputBufferParam = inputParams[i];
        KernelArgumentContainerPtr pKernelArgumentContainer = m_pKernelArgumentContainerFactory->getKernelArgumentContainer(hostInputBufferParam);
        kernelArgumentContainers.push_back(pKernelArgumentContainer);
    }
}

void SimpleCLProgramImpl::setKernelBufferArguments(cl_kernel kernel, const KernelArgumentContainers& kernelArgumentContainers) {
    
    cl_int errNum;
    
    for (unsigned int i = 0; i < kernelArgumentContainers.size(); i++) {
        KernelArgumentContainerPtr pKernelArgumentContainer = kernelArgumentContainers[i];
        void* pData;
        size_t size;
        pKernelArgumentContainer->createKernelArgument(m_context, &pData, &size);
        errNum = clSetKernelArg(kernel, i, size, pData);
        if (errNum != CL_SUCCESS) {
            logAndThrow("Can not create kernel arguments");
        }
    }
}

cl_kernel SimpleCLProgramImpl::getKernelFromName(const char* kernelName) {
    cl_kernel kernel = clCreateKernel(m_builtProgram, kernelName, NULL);
    if (kernel == NULL) {
        logAndThrow("Can not create kernel " + std::string(kernelName));
    }
    return kernel;
}

void SimpleCLProgramImpl::releaseProgram() {
    clReleaseProgram(m_builtProgram);
}

void SimpleCLProgramImpl::releaseKernelExecutables() {
    for (KernelExecutableMap::iterator kIter = m_kernelExecutableMap.begin(); kIter != m_kernelExecutableMap.end(); ) {
        delete kIter->second;
        kIter = m_kernelExecutableMap.erase(kIter);
    }
}