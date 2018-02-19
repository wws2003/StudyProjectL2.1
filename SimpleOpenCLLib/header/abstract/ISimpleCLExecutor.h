//
//  ISimpleCLExecutor.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 3/29/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef ISimpleCLExecutor_h
#define ISimpleCLExecutor_h

#include <string>
#include "common.h"
#include "WorkDims.h"

//NOTE: Try to make this interface independent on OpenCL paradigm as much as possible

class ISimpleCLExecutor {
public:
    virtual ~ISimpleCLExecutor(){};
    
    //Any void function would throw exception if doesn't successfully operated
    
    /*
     Load program from cl source file. This executor only works with one program at once
     */
    virtual void loadProgram(const char* clSourceFilePath,
                             const ProgramPrototype& programPrototype) = 0;
    
    /*
     Submit one kernel.
     - Kernel function decides how to deal with data buffer in inputParams using the memory flags
     - field included in inputParams
     */
    virtual submission_t submitOnce(const char* kernelName,
                            const WorkDims& workDims,
                            const HostInputParams& inputParams) = 0;
    
    /*
     Retrieve the result of submitted kernel
     - User has to prepare HostOutputParams elements before calling this method
     - If pTimeSpec provided != NULL, set run time value to this parameter
     */
    virtual void getResult(const submission_t& sumission,
                           HostOutputParams& outputs,
                           TimeSpec* pTimeSpec) = 0;
};

typedef ISimpleCLExecutor* SimpleCLExecutorPtr;

#endif /* ISimpleCLExecutor_h */
