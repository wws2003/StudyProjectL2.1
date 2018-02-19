//
//  IMatrixMultiplier.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef IMatrixMultiplier_h
#define IMatrixMultiplier_h

#include "common.h"
#include "GenericBuffer.hpp"

template<typename T>
class IMatrixMultiplier {
public:
    virtual ~IMatrixMultiplier(){};
    
    //Factor 1: Matrix m1, size = nDim * pDim
    //Factor 2: Matrix m2, size = pDim * mDim
    //Product: Matrix product
    //Suppose all array has been allocated
    virtual void multiplyMatrix(const GenericBuffer<T>& factor1,
                                const GenericBuffer<T>& factor2,
                                int nDim,
                                int pDim,
                                int mDim,
                                GenericBuffer<T>& product,
                                TimeSpec* pTimeSpec) = 0;
    
    virtual void verifyResults(const GenericBuffer<T>& factor1,
                               const GenericBuffer<T>& factor2,
                               int nDim,
                               int pDim,
                               int mDim,
                               GenericBuffer<T>& product) = 0;
    
};

template<typename T>
using MatrixMultiplierPtr = IMatrixMultiplier<T>* ;

typedef MatrixMultiplierPtr<float> FloatMatrixMultiplierPtr;

typedef MatrixMultiplierPtr<int> IntMatrixMultiplierPtr;



#endif /* IMatrixAdder_h */
