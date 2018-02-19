//
//  IVectorAdder.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/5/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef IVectorAdder_h
#define IVectorAdder_h

#include "common.h"
#include "GenericBuffer.hpp"

template<typename T>
class IVectorAdder {
public:
    virtual ~IVectorAdder(){};
    
    /*
     - Term 1: Vector v1
     - Term 2: Vector v2 (both vector 's size = size)
     - Sum: Vector sum
     - Suppose all array has been allocated
     */
    virtual void addVector(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, GenericBuffer<T>& sum, TimeSpec* pTimeSpec) = 0;
    
    /*
     - Term 1: Vector v1
     - Term 2: Vector v2 (both vector 's size = size)
     - Sum: Vector sum
     - Suppose all array has been allocated
     */
    virtual void verifyResults(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, const GenericBuffer<T>& sum) = 0;
    
};

template<typename T>
using VectorAdderPtr = IVectorAdder<T>* ;

typedef VectorAdderPtr<float> FloatVectorAdderPtr;

typedef VectorAdderPtr<int> IntVectorAdderPtr;

#endif /* IVectorAdder_h */
