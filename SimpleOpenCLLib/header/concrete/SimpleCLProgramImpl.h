//
//  SimpleCLProgramImpl.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/2/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef SimpleCLProgramImpl_h
#define SimpleCLProgramImpl_h

#include "common.h"
#include "ISimpleCLProgram.h"
#include "IKernelArgumentContainerFactory.h"
#include "KernelExecutable.h"
#include <map>
#include <string>

typedef std::map<submission_t, KernelExecutablePtr> KernelExecutableMap;

class SimpleCLProgramImpl : public ISimpleCLProgram {
public:
    
    //Also implicitly transfer owner of builtProgram instance to this SimpleCLProgramImpl instance
    SimpleCLProgramImpl(cl_context context,
                        cl_program builtProgram,
                        KernelArgumentContainerFactoryPtr pKernelArgumentContainerFactory, 
                        const ProgramPrototype& programPrototype);
    
    virtual ~SimpleCLProgramImpl();
    
    //Any void function would throw exception if doesn't successfully operated
    
    //Implement
    cl_kernel getKernelExecutable(const submission_t& submission,
                                  const char* kernelName,
                                  const HostInputParams& inputParams);
    
    //Implement
    void getKernelExecutableOutputBufferParams(const submission_t& submission,
                                               cl_command_queue commandQueue,
                                               HostOutputParams& kernelArgumentContainers) const;
    
    //Implement
    void releaseKernelExecutable(const submission_t& submission);
    
private:
    
    KernelExecutablePtr getKernelExecutable(cl_kernel kernel,
                                            const char* kernelName,
                                            const HostInputParams& inputParams);
    
    void createKernelBuffersForInputParams(const HostInputParams& inputParams,
                                       KernelArgumentContainers& kernelArgumentContainers);
    
    void setKernelBufferArguments(cl_kernel kernel, const KernelArgumentContainers& kernelArgumentContainers);
    
    cl_kernel getKernelFromName(const char* kernelName);
    
    void releaseProgram();
    
    void releaseKernelExecutables();
    
    KernelArgumentContainerFactoryPtr m_pKernelArgumentContainerFactory;
    
    cl_context m_context;
    cl_program m_builtProgram;
    
    ProgramPrototype m_programPrototype;
    
    KernelExecutableMap m_kernelExecutableMap;
};

#endif /* SimpleCLProgramImpl_hpp */
