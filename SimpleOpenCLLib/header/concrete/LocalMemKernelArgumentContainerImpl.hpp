//
//  LocalMemKernelArgumentContainerImpl.hpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/6/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef LocalMemKernelArgumentContainerImpl_h
#define LocalMemKernelArgumentContainerImpl_h

#include "IKernelArgumentContainer.h"

class LocalMemKernelArgumentContainerImpl : public IKernelArgumentContainer {
public:
    LocalMemKernelArgumentContainerImpl(size_t arraySize, size_t elementSize) {
        m_arraySize = arraySize;
        m_elementSize = elementSize;
    };
    
    virtual ~LocalMemKernelArgumentContainerImpl(){};
    
    //Implement
    void createKernelArgument(cl_context context, void** pData, size_t* pSize) {
        *pData = NULL;
        *pSize = m_arraySize * m_elementSize;
    };
    
    //Implement
    void readKernelArgument(cl_command_queue commandQueue, void* pData, size_t size) {
        //Do nothing
    };
    
private:
    size_t m_arraySize;
    size_t m_elementSize;
};


#endif /* LocalMemKernelArgumentContainerImpl_h */
