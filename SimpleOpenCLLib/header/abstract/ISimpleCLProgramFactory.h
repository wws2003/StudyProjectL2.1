//
//  ISimpleCLProgramFactory.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/2/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef ISimpleCLProgramFactory_h
#define ISimpleCLProgramFactory_h

#include "common.h"
#include "ISimpleCLProgram.h"

class ISimpleCLProgramFactory {
public:
    virtual ~ISimpleCLProgramFactory(){};
    
    virtual SimpleCLProgramPtr getSimpleCLProgram(cl_context context,
                                                  cl_device_id deviceToBuildOn,
                                                  const char* clSourceFilePath,
                                                  const ProgramPrototype& programPrototype) = 0;
};

typedef ISimpleCLProgramFactory* SimpleCLProgramFactoryPtr;

#endif /* SimpleCLProgramFactory_h */
