//
//  NormalBufferKernelArgumentContainerImpl.hpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/6/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef NormalBufferKernelArgumentContainerImpl_hpp
#define NormalBufferKernelArgumentContainerImpl_hpp

#include "IKernelArgumentContainer.h"
#include <cstring>

class NormalBufferKernelArgumentContainerImpl : public IKernelArgumentContainer {
public:
    NormalBufferKernelArgumentContainerImpl(void* pData, size_t size) {
        m_pData = pData;
        m_size = size;
    } ;
    virtual ~NormalBufferKernelArgumentContainerImpl(){};
    
    //Implement
    void createKernelArgument(cl_context context, void** pData, size_t* pSize) {
        *pData = m_pData;
        *pSize = m_size;
    };
    
    //Implement
    void readKernelArgument(cl_command_queue commandQueue, void* pData, size_t size) {
        std::memcpy(pData, m_pData, size);
    };
    
private:
    void* m_pData;
    size_t m_size;
};

#endif /* NormalBufferKernelArgumentContainerImpl_hpp */
