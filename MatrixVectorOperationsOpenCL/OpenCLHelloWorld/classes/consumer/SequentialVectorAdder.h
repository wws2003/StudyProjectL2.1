//
//  SequentialVectorAdder.h
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/5/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#ifndef SequentialVectorAdder_h
#define SequentialVectorAdder_h

#include "IVectorAdder.h"

template<typename T>
class SequentialVectorAdder: public IVectorAdder<T> {
public:
    virtual ~SequentialVectorAdder(){};
    
    //Implement
    void addVector(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, GenericBuffer<T>& sum, TimeSpec* pTimeSpec);
    
    //Implement
    void verifyResults(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, const GenericBuffer<T>& sum);
};

#endif /* SequentialVectorAdder_hpp */
