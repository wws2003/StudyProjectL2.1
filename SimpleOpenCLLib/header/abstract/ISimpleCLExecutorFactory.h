//
//  ISimpleCLExecutorFactory.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 3/29/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef ISimpleCLExecutorFactory_h
#define ISimpleCLExecutorFactory_h

#include "ISimpleCLExecutor.h"

class ISimpleCLExecutorFactory {
public:
    virtual ~ISimpleCLExecutorFactory(){};
    
    virtual SimpleCLExecutorPtr getSimpleCLExecutor(cl_device_type deviceType) = 0;
};

typedef  ISimpleCLExecutorFactory* SimpleCLExecutorFactoryPtr;

#endif /* ISimpleCLExecutorFactory_h */
