//
//  IVectorInnerMultiplier.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef IVectorInnerMultiplier_h
#define IVectorInnerMultiplier_h

#include "GenericBuffer.hpp"

template<typename T>
class IVectorInnerMultiplier {
public:
    virtual ~IVectorInnerMultiplier(){};
    
    /*
     - Term 1: Vector v1
     - Term 2: Vector v2 (both vector 's size = size)
     - InnerProduct scalar value: innerProduct
     - Suppose all array has been allocated
     */
    virtual void innerMultiply(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, T& innerProduct, TimeSpec* pTimeSpec) = 0;
    
    /*
     - Term 1: Vector v1
     - Term 2: Vector v2 (both vector 's size = size)
     - InnerProduct scalar value: innerProduct
     - Suppose all array has been allocated
     */
    virtual void verifyResults(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, const T& innerProduct) = 0;
    
};

template<typename T>
using VectorInnerMultiplierPtr = IVectorInnerMultiplier<T>* ;

typedef VectorInnerMultiplierPtr<float> FloatVectorInnerMultiplierPtr;

typedef VectorInnerMultiplierPtr<int> IntVectorInnerMultiplierPtr;



#endif /* IVectorInnerMultiplier_h */
