//
//  SimpleCLExecutorFactoryImpl.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/3/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "SimpleCLExecutorFactoryImpl.h"
#include "SimpleCLExecutorImpl.h"
#include "Util.h"
#include <iostream>

SimpleCLExecutorFactoryImpl::SimpleCLExecutorFactoryImpl(SimpleCLProgramFactoryPtr pSimpleCLProgramFactory) {
    m_pSimpleCLProgramFactory = pSimpleCLProgramFactory;
    m_firstAvailablePlatform = NULL;
}

SimpleCLExecutorPtr SimpleCLExecutorFactoryImpl::getSimpleCLExecutor(cl_device_type deviceType) {
    if(m_firstAvailablePlatform == NULL) {
        setupPlatform();
    }
    
    cl_context context;
    cl_int errNum;
    
    //Next try to create an OpenCL context on the first platform available
    cl_context_properties contextProperties[] = {CL_CONTEXT_PLATFORM,
        (cl_context_properties)m_firstAvailablePlatform,
        0};
    
    context = clCreateContextFromType(contextProperties,
                                      deviceType,
                                      NULL,
                                      NULL,
                                      &errNum);

    if (errNum != CL_SUCCESS) {
        logAndThrow("Couldn't create context on the chosen device type");
    }
    
    SimpleCLExecutorImplPtr pExecutor = SimpleCLExecutorImplPtr(new SimpleCLExecutorImpl(context, m_pSimpleCLProgramFactory));
    
    pExecutor->setup();
    
    return pExecutor;
}

void SimpleCLExecutorFactoryImpl::setupPlatform() {
    cl_uint nbPlatforms;
    cl_int errNum = clGetPlatformIDs(1, &m_firstAvailablePlatform, &nbPlatforms);
    if (errNum != CL_SUCCESS || nbPlatforms <= 0) {
        logAndThrow("Can not get platform");
    }
}