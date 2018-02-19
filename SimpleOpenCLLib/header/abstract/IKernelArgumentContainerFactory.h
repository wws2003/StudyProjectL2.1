//
//  IKernelArgumentContainerFactory.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef IKernelArgumentContainerFactory_h
#define IKernelArgumentContainerFactory_h

#include "common.h"
#include "IKernelArgumentContainer.h"

class IKernelArgumentContainerFactory {
public:
    virtual ~IKernelArgumentContainerFactory(){};
    
    virtual KernelArgumentContainerPtr getKernelArgumentContainer(HostBufferExt& hostInputBuffer) = 0;
};

typedef IKernelArgumentContainerFactory* KernelArgumentContainerFactoryPtr;

#endif /* IKernelArgumentContainerFactory_h */
