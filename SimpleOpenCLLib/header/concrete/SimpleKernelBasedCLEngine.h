//
//  SimpleKernelBasedCLEngine.h
//  SimpleOpenCLLib
//
//  Created by wws2003 on 4/22/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#ifndef SimpleKernelBasedCLEngine_h
#define SimpleKernelBasedCLEngine_h

#include "common.h"
#include "ISimpleCLExecutor.h"
#include "ISimpleCLExecutorFactory.h"
#include "GenericBuffer.hpp"

/**
 * Class for abstract program-based CL engine, working with fixed program, mutable parameters paradigm
 */
class SimpleKernelBasedCLEngine {
public:
    SimpleKernelBasedCLEngine(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                std::string programSrcPath,
                                std::string kernelName,
                                const WorkDims& executingWorkDims,
                                const ParamTypes& paramTypes,
                                const OutputParamIndices& outputParamsIndices,
                                cl_device_type deviceType);
    
    virtual ~SimpleKernelBasedCLEngine();
    
    /**
     * Execute current kernel program against given parameters
     * @param inputs
     * @param outputs
     * @param pTimeSpec
     */
    void executeCLKernelForResult(ConstHostBufferSources inputs,
                                  HostBufferSources outputs,
                                  TimeSpec* pTimeSpec);
    
protected:
    // Below protected methods are subjects for overriden by subclass
    /**
     * Init program prototype
     * @param kernelName
     * @param outputParamsIndices Indexes of output params in the param vector
     * @param programPrototype Program prototype to be initialized
     */
    virtual void initProgramPrototype(std::string kernelName, const OutputParamIndices& outputParamsIndices, ProgramPrototype& programPrototype);
    
    /**
     * Release initialized prototype
     */
    void releaseProgramPrototype(ProgramPrototype& programPrototype);
    
    /**
     * Init host input params from input buffers
     * @param inputs
     * @param outputs
     * @param hostInputParams
     */
    virtual void initCLInputParams(const ConstHostBufferSources& inputs,
                                   const HostBufferSources& outputs,
                                   HostInputParams& hostInputParams);
    
    /**
     * Init host output params from output buffers
     * @param outputs
     * @param hostOutputParams
     */
    virtual void initCLOutputParams(HostBufferSources outputs,
                                    HostOutputParams& hostOutputParams);
    
    virtual void releaseHostParams(HostInputParams& hostInputParams,
                                   HostOutputParams& hostOutputParams);
    
    virtual void onResultReady(const HostOutputParams& outputs);
    
private:
    // CL properties
    std::string m_kernelName;
    WorkDims m_executingWorkDims;
    ParamTypes m_paramTypes;
    
    ProgramPrototype m_programPrototype;
    
    SimpleCLExecutorFactoryPtr m_pSimpleExecutorFactory;
    SimpleCLExecutorPtr m_pCLExecutor;
};

#endif /* SimpleKernelBasedCLEngine_h */
