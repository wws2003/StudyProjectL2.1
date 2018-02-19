//
//  CLVectorAdder.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/6/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef CLVectorAdder_h
#define CLVectorAdder_h

#include "IVectorAdder.h"
#include "AbstractCLEngine.h"

template<typename T>
class CLVectorAdder: public IVectorAdder<T> {
public:
    CLVectorAdder(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                  const WorkDims& executingWorkDims,
                  cl_device_type deviceType);
    
    virtual ~CLVectorAdder(){};
    
    //Implement
    void addVector(const GenericBuffer<T>& term1,
                   const GenericBuffer<T>& term2,
                   GenericBuffer<T>& sum,
                   TimeSpec* pTimeSpec);
    
    //Implement
    void verifyResults(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, const GenericBuffer<T>& sum);
    
private:
    SimpleCLExecutorFactoryPtr m_pSimpleExecutorFactory;
    WorkDims m_workDims;
    cl_device_type m_deviceType;
};

#endif /* CLVectorAdder_hpp */
