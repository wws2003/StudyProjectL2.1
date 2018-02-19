//
//  KernelExecutable.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/4/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef KernelExecutable_h
#define KernelExecutable_h

#include "common.h"
#include "IKernelArgumentContainer.h"
#include <string>
#include <map>

class KernelExecutable {
public:
    KernelExecutable(cl_kernel kernel,
                     KernelPrototype prototype,
                     const KernelArgumentContainers& argumentContainers);
    
    virtual ~KernelExecutable();
    
    cl_kernel m_kernel;
    KernelPrototype m_prototype;
    KernelArgumentContainers m_argumentContainers;
};

typedef KernelExecutable* KernelExecutablePtr;

#endif /* KernelExecutable_hpp */
