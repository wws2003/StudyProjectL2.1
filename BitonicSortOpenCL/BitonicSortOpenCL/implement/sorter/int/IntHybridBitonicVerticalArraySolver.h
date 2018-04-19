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
    virtual void accept(const BitonicVerticalArrayData<int>& data);
    
    /**
     *@Override
     */
    virtual void solve(const BitonicVerticalArrayInfo<int>& bitonicVerticalArray);

private:
    
    /**
     * Solve assumpt internal data is valid
     * @param info The info to solve
     */
    virtual void doSolve(const BitonicVerticalArrayInfo<int>& info) const;
    
    /**
    * Check if solving problem can delegate to GPU
    * @param info The info to solve
    */
    bool canDelegateToGPU(const BitonicVerticalArrayInfo<int>& info) const;
    
    /**
    * Carry out the compare and swap on the first column of given vertical array
    * @param info The info to solve
    */
    void compareAndSwapFirstColumn(const BitonicVerticalArrayInfo<int>& info) const;
    
    BitonicVerticalArrayData<int> m_dataBuffer;
    
    // Max work group size (to detect swap block size can delegate to GPU solver)
    size_t m_maxWorkGroupSize;
    
    // Solver by GPU
    BitonicVerticalArraySolverPtr<int> m_pGPUSolver;
};

#endif /* IntHybridBitonicVerticalArraySolver_h */
