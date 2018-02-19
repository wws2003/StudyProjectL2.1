//
//  SequentialVectorInnerMultiplierImpl.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "SequentialVectorInnerMultiplierImpl.h"
#include "Util.h"
#include <cassert>

template<typename T>
void SequentialVectorInnerMultiplierImpl<T>::innerMultiply(const GenericBuffer<T>& term1,
                                                   const GenericBuffer<T>& term2,
                                                   T& innerProduct,
                                                   TimeSpec* pTimeSpec) {
    {
        ScopeTimer scpTimer(pTimeSpec);
        innerProduct = 0;
        for (unsigned int i = 0; i < term1.size(); i++) {
            innerProduct += term1[i] * term2[i];
        }
    }

}

template<typename T>
void SequentialVectorInnerMultiplierImpl<T>::verifyResults(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, const T& innerProduct) {
    T correctInnerProduct = 0;
    for (unsigned int i = 0; i < term1.size(); i++) {
        correctInnerProduct += term1[i] * term2[i];
    }
    assert(correctInnerProduct == innerProduct);
}

template class SequentialVectorInnerMultiplierImpl<float>;
template class SequentialVectorInnerMultiplierImpl<int>;