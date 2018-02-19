//
//  ISimpleCLProgram.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/2/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef ISimpleCLProgram_h
#define ISimpleCLProgram_h

#include "common.h"

class ISimpleCLProgram {
public:
    virtual ~ISimpleCLProgram(){};
    
    //Any void function would throw exception if doesn't successfully operated
    
    //Setup and return a kernel object avaialable to be enqueued
    virtual cl_kernel getKernelExecutable(const submission_t& submission,
                                        const char* kernelName,
                                        const HostInputParams& inputParams) = 0;
    
    //Find and return buffers (memory objects) dedicated for the output of the kernel function
    //submitted by submission
    virtual void getKernelExecutableOutputBufferParams(const submission_t& submission,
                                                       cl_command_queue commandQueue,
                                                       HostOutputParams& outputParams) const = 0;
    
    //Find and release resources (memory objects...) bound to the kernel function
    //submitted by submission
    virtual void releaseKernelExecutable(const submission_t& submission) = 0;
};

typedef ISimpleCLProgram* SimpleCLProgramPtr;

#endif /* ISimpleCLProgram_h */
