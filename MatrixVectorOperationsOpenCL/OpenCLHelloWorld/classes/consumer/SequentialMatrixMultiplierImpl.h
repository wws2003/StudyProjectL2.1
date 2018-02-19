//
//  SequentialMatrixMultiplierImpl.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef SequentialMatrixMultiplierImpl_h
#define SequentialMatrixMultiplierImpl_h

#include "IMatrixMultiplier.h"

template<typename T>
class SequentialMatrixMultiplierImpl : public IMatrixMultiplier<T> {
public:
    virtual ~SequentialMatrixMultiplierImpl(){};
    
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
    
};

#endif /* SequentialMatrixMultiplierImpl_hpp */
