//
//  IntHybridBitonicBlockSolver.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 3/25/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef IntHybridBitonicBlockSolver_h
#define IntHybridBitonicBlockSolver_h

#include "Common.h"
#include "IBitonicBlockSolver.h"
#include "common.h"
#include "WorkDims.h"
#include "AbstractCLEngine.h"
#include <functional>
#include <map>

typedef std::function<void (const BitonicBlock<int>&)> SolveFunc;

typedef std::map<MACHINE, SolveFunc> SolveFuncMap;

class IntHybridBitonicBlockSolver: public IBitonicBlockSolver<int> {
public:
    IntHybridBitonicBlockSolver(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                size_t maxWorkGroupSize);
    
    virtual ~IntHybridBitonicBlockSolver(){};
    
    /**
     * @Override
     * Solve the given block
     */
    virtual void solve(const BitonicBlock<int>& bitonicBlock) const;
    
private:
    /**
    * Detect device type to solve the given block
    * @param startIndex   Index of first element (against the main buffer)
    * @param endIndex     Index of last element (against the main buffer)
    * @param sortingDepth Depth of the sorting
    */
    MACHINE detectProperDeviceTypeToSolve(size_t startIndex, size_t endIndex, int sortingDepth) const;
    
    /**
    * Solve the given block by CPU
    * @param bitonicBlock
    */
    void solveByCPU(const BitonicBlock<int>& bitonicBlock) const;
    
    /**
     * Solve the given block by GPU
     * @param bitonicBlock
     */

    void solveByGPU(const BitonicBlock<int>& bitonicBlock) const;
    
    /**
    * Collect elements in the buffer working with GPU into output elements
    * @param elementBuffer The buffer working with GPU
    * @param offset        Offset on the output elements
    * @param elements      Output elements
    */
    void collect(const IntBuffer& elementBuffer, size_t offset, ElementList<int>& elements) const;
    
    // Map of solve func
    SolveFuncMap m_solveFuncMap;

    // CL services and configurations
    const std::string m_programName;
    const std::string m_kernelName;
    SimpleCLExecutorFactoryPtr m_pSimpleExecutorFactory;
    WorkDims m_executingDims;
    size_t m_maxWorkGroupSize;
};

#endif /* IntHybridBitonicBlockSolver_h */
