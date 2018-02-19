//
//  CLVectorInnerMultiplierImpl.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef CLVectorInnerMultiplierImpl_h
#define CLVectorInnerMultiplierImpl_h

#include "IVectorInnerMultiplier.h"
#include "ISimpleCLExecutorFactory.h"

template<typename T>
class CLVectorInnerMultiplierImpl: public IVectorInnerMultiplier<T> {
public:
    CLVectorInnerMultiplierImpl(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                  const WorkDims& executingWorkDims,
                  cl_device_type deviceType);
    
    virtual ~CLVectorInnerMultiplierImpl(){};
    
    //Implement
    void innerMultiply(const GenericBuffer<T>& term1,
                       const GenericBuffer<T>& term2,
                       T& innerProduct,
                       TimeSpec* pTimeSpec);
    
    //Implement
    void verifyResults(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, const T& innerProduct);
    
private:
    SimpleCLExecutorFactoryPtr m_pSimpleExecutorFactory;
    WorkDims m_workDims;
    cl_device_type m_deviceType;
};

#endif /* CLVectorInnerMultiplierImpl_hpp */
