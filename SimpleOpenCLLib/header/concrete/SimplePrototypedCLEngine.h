//
//  SimplePrototypedCLEngine.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/6/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef SimplePrototypedCLEngine_h
#define SimplePrototypedCLEngine_h

#include "common.h"
#include "AbstractCLEngine.h"

//For kernel functions take some buffers, and write results into some other buffers

class SimplePrototypedCLEngine: public AbstractCLEngine {
public:
    SimplePrototypedCLEngine(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                             ConstHostBufferSources inputs,
                             HostBufferSources outputs,
                             ParamTypes paramTypes,
                             OutputParamIndices outputParamsIndices);
    
    virtual ~SimplePrototypedCLEngine(){};
    
protected:
    //Implement
    void initProgramPrototype(std::string kernelName, ProgramPrototype& programPrototype);
    
    //Implement
    void initCLInputParams(HostInputParams& hostInputParams);
    
    //Implement
    void initCLOutputParams(HostOutputParams& outputs);
    
    //Implement
    void releaseHostParams(HostInputParams& hostInputParams, HostOutputParams& hostOutputParams);
    
private:
    ConstHostBufferSources m_inputs;
    HostBufferSources m_outputs;
    ParamTypes m_paramTypes;
    OutputParamIndices m_outputParamsIndices;
};

#endif /* SimplePrototypedCLEngine_hpp */
