//
//  IBitonicBlockSolver.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 3/25/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef IBitonicBlockSolver_h
#define IBitonicBlockSolver_h

#include "BitonicBlock.h"

/**
* Class to carry out the task against given bitonic block
* Sort task is effectively swap the argument block then recursively swap the sub-blocks
*/

template<typename T>
class IBitonicBlockSolver {
public:
    IBitonicBlockSolver(){};
    virtual ~IBitonicBlockSolver(){};
    
    /**
    * Solve the given block
    */
    virtual void solve(const BitonicBlock<T>& bitonicBlock) const = 0;
};


#endif /* IBitonicBlockSolver_h */
