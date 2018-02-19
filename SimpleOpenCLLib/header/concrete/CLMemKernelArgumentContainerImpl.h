//
//  CLMemKernelArgumentContainerImpl.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/6/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef CLMemKernelArgumentContainerImpl_h
#define CLMemKernelArgumentContainerImpl_h

#include "IKernelArgumentContainer.h"
#include "common.h"

class CLMemKernelArgumentContainerImpl: public IKernelArgumentContainer {
public:
    
    CLMemKernelArgumentContainerImpl(void* data, size_t size, cl_mem_flags memFlags);
    virtual ~CLMemKernelArgumentContainerImpl();
    
    //Implement
    void createKernelArgument(cl_context context, void** pData, size_t* pSize);
    
    //Implement
    void readKernelArgument(cl_command_queue commandQueue, void* pData, size_t size);
    
private:
    void* m_data;
    size_t m_size;
    cl_mem_flags m_memFlags;
    cl_mem m_mem;
};


#endif /* CLMemKernelArgumentContainerImpl_hpp */
