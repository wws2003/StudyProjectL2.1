//
//  KernelArgumentContainerFactoryImpl.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef KernelArgumentContainerFactoryImpl_h
#define KernelArgumentContainerFactoryImpl_h

#include "IKernelArgumentContainerFactory.h"

class KernelArgumentContainerFactoryImpl : public IKernelArgumentContainerFactory {
public:
    virtual ~KernelArgumentContainerFactoryImpl(){};
    
    //Implement
    KernelArgumentContainerPtr getKernelArgumentContainer(HostBufferExt& hostInputBuffer);
    
private:
    cl_mem_flags computeMemFlags(ParamType paramType);
};

#endif /* KernelArgumentContainerFactoryImpl_hpp */
