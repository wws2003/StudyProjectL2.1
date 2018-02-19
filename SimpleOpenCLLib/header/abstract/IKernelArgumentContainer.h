//
//  IKernelArgumentContainer.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/6/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef IKernelArgumentContainer_h
#define IKernelArgumentContainer_h

#include "common.h"

class IKernelArgumentContainer {
public:
    virtual ~IKernelArgumentContainer(){};
    
    virtual void createKernelArgument(cl_context context, void** pData, size_t* pSize) = 0;
    
    virtual void readKernelArgument(cl_command_queue commandQueue, void* pData, size_t size) = 0;
};

typedef IKernelArgumentContainer* KernelArgumentContainerPtr;

typedef std::vector<KernelArgumentContainerPtr> KernelArgumentContainers;

#endif /* IKernelArgumentContainer_h */
