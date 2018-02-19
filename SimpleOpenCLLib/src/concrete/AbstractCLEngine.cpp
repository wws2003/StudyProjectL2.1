//
//  AbstractCLEngine.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/6/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "AbstractCLEngine.h"

AbstractCLEngine::AbstractCLEngine(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory) {
    m_pSimpleExecutorFactory = pSimpleExecutorFactory;
}

void AbstractCLEngine::executeCLKernelForResult(std::string programSrcPath,
                              std::string kernelName,
                              const WorkDims& executingWorkDims,
                              cl_device_type deviceType,
                              TimeSpec* pTimeSpec) {
    
    ProgramPrototype programPrototype;
    initProgramPrototype(kernelName, programPrototype);
    
    SimpleCLExecutorPtr pCLExecutor = m_pSimpleExecutorFactory->getSimpleCLExecutor(deviceType);
    pCLExecutor->loadProgram(programSrcPath.data(), programPrototype);
    
    HostInputParams hostInputParams;
    initCLInputParams(hostInputParams);
    
    submission_t submission = pCLExecutor->submitOnce(kernelName.data(), executingWorkDims, hostInputParams);
    
    HostOutputParams hostOutputParams;
    initCLOutputParams(hostOutputParams);
    
    pCLExecutor->getResult(submission, hostOutputParams, pTimeSpec);
    
    onResultReady(hostOutputParams);
    
    releaseHostParams(hostInputParams, hostOutputParams);
    
    releaseProgramPrototype(programPrototype);
    delete pCLExecutor;
}

void AbstractCLEngine::onResultReady(const HostOutputParams& outputs) {
    //In almost cases, do nothing since output has been prepared by sub-class and therefore should be accessible from sub-class
}

void AbstractCLEngine::releaseProgramPrototype(ProgramPrototype& programPrototype) {
    for (ProgramPrototype::iterator kIter = programPrototype.begin(); kIter != programPrototype.end(); ) {
        KernelPrototypePtr pKernelPrototype = kIter->second;
        delete pKernelPrototype;
        kIter = programPrototype.erase(kIter);
    }
}

