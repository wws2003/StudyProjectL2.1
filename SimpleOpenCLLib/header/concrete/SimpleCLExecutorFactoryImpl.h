//
//  SimpleCLExecutorFactoryImpl.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/3/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef SimpleCLExecutorFactoryImpl_h
#define SimpleCLExecutorFactoryImpl_h

#include "ISimpleCLExecutorFactory.h"
#include "ISimpleCLProgramFactory.h"

class SimpleCLExecutorFactoryImpl: public ISimpleCLExecutorFactory {
public:
    SimpleCLExecutorFactoryImpl(SimpleCLProgramFactoryPtr pSimpleCLProgramFactory);
    
    virtual ~SimpleCLExecutorFactoryImpl(){};
    
    //Implement
    SimpleCLExecutorPtr getSimpleCLExecutor(cl_device_type deviceType);
    
private:
    void setupPlatform();
    
    SimpleCLProgramFactoryPtr m_pSimpleCLProgramFactory;
    
    cl_platform_id m_firstAvailablePlatform;
};

#endif /* SimpleCLExecutorFactoryImpl_hpp */
