//
//  KernelExecutable.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/4/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "KernelExecutable.h"

KernelExecutable::KernelExecutable(cl_kernel kernel,
                                   KernelPrototype prototype,
                                   const KernelArgumentContainers& argumentContainers) : m_prototype(prototype) {
    m_kernel = kernel;
    m_argumentContainers = argumentContainers;
}

KernelExecutable::~KernelExecutable() {
    //Release kernel
    clReleaseKernel(m_kernel);
    
    //Release buffer objects
    for (KernelArgumentContainers::iterator bIter = m_argumentContainers.begin(); bIter != m_argumentContainers.end(); ) {
        delete *bIter;
        bIter = m_argumentContainers.erase(bIter);
    }
}