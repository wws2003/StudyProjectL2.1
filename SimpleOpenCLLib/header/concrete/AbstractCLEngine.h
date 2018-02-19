//
//  AbstractCLEngine.hpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/6/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef AbstractCLEngine_h
#define AbstractCLEngine_h

#include "common.h"
#include "ISimpleCLExecutor.h"
#include "ISimpleCLExecutorFactory.h"
#include "GenericBuffer.hpp"

class AbstractCLEngine {
public:
    
    AbstractCLEngine(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory);
    
    virtual ~AbstractCLEngine(){};
    
    void executeCLKernelForResult(std::string programSrcPath,
                                  std::string kernelName,
                                  const WorkDims& executingWorkDims,
                                  cl_device_type deviceType,
                                  TimeSpec* pTimeSpec);
    
protected:
    virtual void initProgramPrototype(std::string kernelName, ProgramPrototype& programPrototype) = 0;
    
    virtual void initCLInputParams(HostInputParams& hostInputParams) = 0;
    
    virtual void initCLOutputParams(HostOutputParams& outputs) = 0;
    
    virtual void releaseHostParams(HostInputParams& hostInputParams, HostOutputParams& hostOutputParams) = 0;
    
    virtual void onResultReady(const HostOutputParams& outputs);
    
private:
    
    void releaseProgramPrototype(ProgramPrototype& programPrototype);
    
    SimpleCLExecutorFactoryPtr m_pSimpleExecutorFactory;

};

typedef AbstractCLEngine* CLEnginePtr;


#endif /* AbstractCLEngine_hpp */
