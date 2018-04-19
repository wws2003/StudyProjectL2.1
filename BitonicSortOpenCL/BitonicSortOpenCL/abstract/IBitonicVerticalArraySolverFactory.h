//
//  IBitonicVerticalArraySolverFactory.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/16/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef IBitonicVerticalArraySolverFactory_h
#define IBitonicVerticalArraySolverFactory_h

#include "Common.h"
#include "IBitonicVerticalArraySolver.h"

/**
 * Class to create object of BitonicVerticalArraySolver from elements
 */
template<typename T>
class IBitonicVerticalArraySolverFactory {
public:
    IBitonicVerticalArraySolverFactory(){};
    virtual ~IBitonicVerticalArraySolverFactory(){};
    
    /**
     * Create the BitonicVerticalArraySolver instance from elements
     * @param elements
     */
    virtual BitonicVerticalArraySolverPtr create(const ElementList<T>& elements) = 0;
};

#endif /* IBitonicVerticalArraySolverFactory_h */
