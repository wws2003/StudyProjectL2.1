//
//  SimpleCLProgramFactoryImpl.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/3/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef SimpleCLProgramFactoryImpl_h
#define SimpleCLProgramFactoryImpl_h

#include "common.h"
#include "ISimpleCLProgramFactory.h"
#include "IKernelArgumentContainerFactory.h"

class SimpleCLProgramFactoryImpl: public ISimpleCLProgramFactory {
public:
    SimpleCLProgramFactoryImpl();
    
    virtual ~SimpleCLProgramFactoryImpl();
    
    //Implement
    SimpleCLProgramPtr getSimpleCLProgram(cl_context context,
                                          cl_device_id deviceToBuildOn, 
                                          const char* clSourceFilePath,
                                          const ProgramPrototype& programPrototype);
    
private:
    KernelArgumentContainerFactoryPtr m_pKernelArgumentContainerFactory;
    std::string getProgramSource(const char* clSourceFilePath);
};

#endif /* SimpleCLProgramFactoryImpl_hpp */
