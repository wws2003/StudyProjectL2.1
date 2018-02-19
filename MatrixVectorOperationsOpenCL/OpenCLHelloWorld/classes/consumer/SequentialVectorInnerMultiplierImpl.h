//
//  SequentialVectorInnerMultiplierImpl.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef SequentialVectorInnerMultiplierImpl_h
#define SequentialVectorInnerMultiplierImpl_h

#include "IVectorInnerMultiplier.h"
template<typename T>
class SequentialVectorInnerMultiplierImpl: public IVectorInnerMultiplier<T> {
public:
    SequentialVectorInnerMultiplierImpl(){};
    
    virtual ~SequentialVectorInnerMultiplierImpl(){};
    
    //Implement
    void innerMultiply(const GenericBuffer<T>& term1,
                       const GenericBuffer<T>& term2,
                       T& innerProduct,
                       TimeSpec* pTimeSpec);
    
    //Implement
    void verifyResults(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, const T& innerProduct);
};


#endif /* SequentialVectorInnerMultiplierImpl_hpp */
