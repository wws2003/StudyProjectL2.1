//
//  CLMemKernelArgumentContainerImpl.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/6/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "CLMemKernelArgumentContainerImpl.h"
#include "Util.h"
#include <sstream>

CLMemKernelArgumentContainerImpl::CLMemKernelArgumentContainerImpl(void* data, size_t size, cl_mem_flags memFlags) {
    m_data = data;
    m_size = size;
    m_memFlags = memFlags;
    m_mem = NULL;
}

CLMemKernelArgumentContainerImpl::~CLMemKernelArgumentContainerImpl() {
    if (m_mem != NULL) {
        clReleaseMemObject(m_mem);
    }
}

void CLMemKernelArgumentContainerImpl::createKernelArgument(cl_context context, void** pData, size_t* pSize) {
    cl_int errCode;
    cl_mem memBuffer = clCreateBuffer(context,
                                      m_memFlags,
                                      m_size,
                                      m_memFlags & (CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR) ? m_data : NULL,
                                      &errCode);
    m_mem = memBuffer;
    
    if(memBuffer != NULL) {
        *pData = &m_mem;
        *pSize = sizeof(cl_mem);
    }
    else {
        std::stringstream ss;
        ss << "Can not create kernel buffer by error " << errCode;
        logAndThrow(ss.str());
    }
}

void CLMemKernelArgumentContainerImpl::readKernelArgument(cl_command_queue commandQueue, void* pData, size_t size) {
    cl_int errNum = clEnqueueReadBuffer(commandQueue,
                                        m_mem,
                                        CL_TRUE,
                                        0,
                                        size,
                                        pData,
                                        0,
                                        NULL,
                                        NULL);
    if (errNum != CL_SUCCESS) {
        //TODO Release kernel from current program before terminate
        //m_pCurrentCLProgram->releaseKernelExecutable(submission);
        logAndThrow("Couldn't read result from to host");
    }

}
