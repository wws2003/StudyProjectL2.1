//
//  SequentialVectorAdder.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/5/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "SequentialVectorAdder.h"
#include <cassert>
#include <ctime>
#include <iostream>
#include "Util.h"

template<typename T>
void SequentialVectorAdder<T>::addVector(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, GenericBuffer<T>& sum, TimeSpec* pTimeSpec) {
    
    {
        ScopeTimer scpTimer(pTimeSpec);
        for (unsigned int i = 0; i < term1.size(); i++) {
            sum.set(i, term1[i] + term2[i]);
        }
    }
}

template<typename T>
void SequentialVectorAdder<T>::verifyResults(const GenericBuffer<T>& term1, const GenericBuffer<T>& term2, const GenericBuffer<T>& sum) {
    for (int i = 0; i < term1.size(); i++) {
        assert(sum[i] == term1[i] + term2[i]);
    }
}

template class SequentialVectorAdder<float>;
template class SequentialVectorAdder<int>;
