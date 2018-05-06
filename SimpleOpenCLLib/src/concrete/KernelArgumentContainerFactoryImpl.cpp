//
//  KernelArgumentContainerFactoryImpl.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "KernelArgumentContainerFactoryImpl.h"
#include "LocalMemKernelArgumentContainerImpl.hpp"
#include "NormalBufferKernelArgumentContainerImpl.hpp"
#include "CLMemKernelArgumentContainerImpl.h"
#include "Util.h"

KernelArgumentContainerPtr KernelArgumentContainerFactoryImpl::getKernelArgumentContainer(HostBufferExt& hostInputBuffer) {
    
    KernelArgumentContainerPtr pKernelArgumentContainer = NULL;
    
    ParamType inputParamType = hostInputBuffer.m_paramType;
    
    switch (inputParamType) {
        case PT_GLOBAL_IN:
             pKernelArgumentContainer = KernelArgumentContainerPtr(new CLMemKernelArgumentContainerImpl(hostInputBuffer.m_data, hostInputBuffer.m_arraySize * hostInputBuffer.m_elementSize, computeMemFlags(inputParamType)));
            break;
        case PT_GLOBAL_OUT:
            pKernelArgumentContainer = KernelArgumentContainerPtr(new CLMemKernelArgumentContainerImpl(hostInputBuffer.m_data, hostInputBuffer.m_arraySize * hostInputBuffer.m_elementSize, computeMemFlags(inputParamType)));
            break;
        case PT_GLOBAL_INOUT:
            pKernelArgumentContainer = KernelArgumentContainerPtr(new CLMemKernelArgumentContainerImpl(hostInputBuffer.m_data, hostInputBuffer.m_arraySize * hostInputBuffer.m_elementSize, computeMemFlags(inputParamType)));
            break;
        case PT_CONSTANT:
            pKernelArgumentContainer = KernelArgumentContainerPtr(new NormalBufferKernelArgumentContainerImpl(hostInputBuffer.m_data, hostInputBuffer.m_arraySize * hostInputBuffer.m_elementSize));
            break;
        case PT_LOCAL:
            pKernelArgumentContainer = KernelArgumentContainerPtr(new LocalMemKernelArgumentContainerImpl(hostInputBuffer.m_arraySize, hostInputBuffer.m_elementSize));
            break;
        default:
            break;
    }
    
    return pKernelArgumentContainer;
}

cl_mem_flags KernelArgumentContainerFactoryImpl::computeMemFlags(ParamType paramType) {
    if (paramType == PT_GLOBAL_IN) {
        //TODO More sohisticated solution
        return CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR;
    }
    if (paramType == PT_GLOBAL_OUT) {
        return CL_MEM_READ_WRITE;
    }
    if (paramType == PT_GLOBAL_INOUT) {
        return CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR;
    }
    logAndThrow("No such a proper param type for CL_MEM object");
    return 0;
}