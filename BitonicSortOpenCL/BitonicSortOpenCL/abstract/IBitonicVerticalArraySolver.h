//
//  IBitonicVerticalArraySolver.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/14/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef IBitonicVerticalArraySolver_h
#define IBitonicVerticalArraySolver_h

#include "Common.h"
#include "BitonicVerticalArrayInfo.h"
#include "BitonicVerticalArrayData.h"

/**
* Interface to carry out sorting operations against one bitonic vertical array data
*/

template<typename T>
class IBitonicVerticalArraySolver {
public:
    IBitonicVerticalArraySolver(){};
    virtual ~IBitonicVerticalArraySolver(){};
    
    /**
     * Store elements into internal data structure
     * @param data
     */
    virtual void accept(const BitonicVerticalArrayData<T>& data) = 0;
    
    /**
     * Solve (compare-and-swap given vertical array) against stored data, 
     * practically update the internal data
     * @param bitonicVerticalArrayInfo
     */
    virtual void solve(const BitonicVerticalArrayInfo<T>& bitonicVerticalArrayInfo) = 0;
};


#endif /* IBitonicVerticalArraySolver_h */
