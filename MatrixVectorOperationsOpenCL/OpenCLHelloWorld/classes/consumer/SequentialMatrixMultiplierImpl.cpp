//
//  SequentialMatrixMultiplierImpl.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/7/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "SequentialMatrixMultiplierImpl.h"
#include "Util.h"
#include <cassert>

template<typename T>
void SequentialMatrixMultiplierImpl<T>::multiplyMatrix(const GenericBuffer<T>& factor1,
                    const GenericBuffer<T>& factor2,
                    int nDim,
                    int pDim,
                    int mDim,
                    GenericBuffer<T>& product,
                    TimeSpec* pTimeSpec) {
    //Factor1 n * p
    //Factor2 p * m
    //Product n * m
    
    {
        ScopeTimer scpTimer(pTimeSpec);
        
        for (unsigned int row1 = 0; row1 < nDim; row1 ++) {
            for (unsigned int col2 = 0; col2 < mDim; col2++) {
                T innerProduct = 0;
                
                //product[row1][col2]
                unsigned int targetIndex = row1 * mDim + col2;
                
                for (unsigned int col1 = 0; col1 < pDim; col1++) {
                    unsigned int row2 = col1;
                    //product[row1][col2] += factor1[row1][col1] + factor2[row2][col2]
                    innerProduct += factor1[row1 * pDim + col1] * factor2[row2 * mDim + col2];
                }
                product.set(targetIndex, innerProduct);
            }
        }
    }
}

template<typename T>
void SequentialMatrixMultiplierImpl<T>::verifyResults(const GenericBuffer<T>& factor1,
                   const GenericBuffer<T>& factor2,
                   int nDim,
                   int pDim,
                   int mDim,
                   GenericBuffer<T>& product) {
    
    for (unsigned int row1 = 0; row1 < nDim; row1 ++) {
        for (unsigned int col2 = 0; col2 < mDim; col2++) {
            T innerProduct = 0;
            
            //product[row1][col2]
            unsigned int targetIndex = row1 * mDim + col2;
            
            for (unsigned int col1 = 0; col1 < pDim; col1++) {
                unsigned int row2 = col1;
                //product[row1][col2] += factor1[row1][col1] + factor2[row2][col2]
                innerProduct += factor1[row1 * pDim + col1] * factor2[row2 * mDim + col2];
            }
            assert(innerProduct == product[targetIndex]);
        }
    }
}

template class SequentialMatrixMultiplierImpl<float>;
template class SequentialMatrixMultiplierImpl<int>;