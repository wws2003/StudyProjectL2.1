//
//  IntHybridBitonicVerticalArraySolver.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/14/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef IntHybridBitonicVerticalArraySolver_h
#define IntHybridBitonicVerticalArraySolver_h

#include "IBitonicVerticalArraySolver.h"

class IntHybridBitonicVerticalArraySolver : public IBitonicVerticalArraySolver<int> {
public:
    IntHybridBitonicVerticalArraySolver(size_t maxWorkGroupSize, BitonicVerticalArraySolverPtr<int> pGPUSolver);
    
    virtual ~IntHybridBitonicVerticalArraySolver(){};
    
    /**
     *@Override
     */
    virtual void solve(const BitonicVerticalArray<int>& bitonicVerticalArray) const;

private:
    /**
    * Check if solving problem can delegate to GPU
    */
    bool canDelegateToGPU(const BitonicVerticalArray<int>& bitonicVerticalArray) const;
    
    /**
    * Carry out the compare and swap on the first column of given vertical array
    */
    void compareAndSwapFirstColumn(const BitonicVerticalArray<int>& bitonicVerticalArray) const;
    
    // Max work group size (to detect swap block size can delegate to GPU solver)
    size_t m_maxWorkGroupSize;
    
    // Solver by GPU
    BitonicVerticalArraySolverPtr<int> m_pGPUSolver;
};

#endif /* IntHybridBitonicVerticalArraySolver_h */
