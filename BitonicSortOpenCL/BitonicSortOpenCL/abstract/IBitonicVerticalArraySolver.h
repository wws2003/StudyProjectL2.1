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
#include "BitonicVerticalArray.h"

/**
* Interface to carry out sorting operations against one bitonic vertical array
*/

template<typename T>
class IBitonicVerticalArraySolver {
public:
    IBitonicVerticalArraySolver(){};
    virtual ~IBitonicVerticalArraySolver(){};
    
    /**
    * Solve (compare-and-swap given vertical array) 
    */
    virtual void solve(const BitonicVerticalArray<T>& bitonicVerticalArray) const = 0;
};


#endif /* IBitonicVerticalArraySolver_h */
