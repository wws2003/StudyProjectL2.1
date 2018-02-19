//
//  CLLocalizedMatrixMultiplierImpl.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef CLLocalizedMatrixMultiplierImpl_h
#define CLLocalizedMatrixMultiplierImpl_h

#include "IMatrixMultiplier.h"
#include "ISimpleCLExecutorFactory.h"

template<typename T>
class CLLocalizedMatrixMultiplierImpl: public IMatrixMultiplier<T> {
    
public:
    CLLocalizedMatrixMultiplierImpl(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                           const WorkDims& executingWorkDims,
                           cl_device_type deviceType);
    
    virtual ~CLLocalizedMatrixMultiplierImpl(){};
    
    //Factor 1: Matrix m1, size = nDim * pDim
    //Factor 2: Matrix m2, size = pDim * mDim
    //Product: Matrix product
    //Suppose all array has been allocated
    void multiplyMatrix(const GenericBuffer<T>& factor1,
                        const GenericBuffer<T>& factor2,
                        int nDim,
                        int pDim,
                        int mDim,
                        GenericBuffer<T>& product,
                        TimeSpec* pTimeSpec);
    
    void verifyResults(const GenericBuffer<T>& factor1,
                       const GenericBuffer<T>& factor2,
                       int nDim,
                       int pDim,
                       int mDim,
                       GenericBuffer<T>& product);
    
private:
    SimpleCLExecutorFactoryPtr m_pSimpleExecutorFactory;
    WorkDims m_workDims;
    cl_device_type m_deviceType;
};


#endif /* CLLocalizedMatrixMultiplierImpl_hpp */
