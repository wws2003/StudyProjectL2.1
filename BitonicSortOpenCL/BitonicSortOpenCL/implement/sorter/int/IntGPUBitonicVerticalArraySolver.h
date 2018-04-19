//
//  IntGPUBitonicVerticalArraySolver.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/15/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef IntGPUBitonicVerticalArraySolver_h
#define IntGPUBitonicVerticalArraySolver_h

#include "IBitonicVerticalArraySolver.h"
#include "Common.h"
#include "common.h"
#include "WorkDims.h"
#include "AbstractCLEngine.h"

class IntGPUBitonicVerticalArraySolver : public IBitonicVerticalArraySolver<int> {
public:
    IntGPUBitonicVerticalArraySolver(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                                WorkDims executingDims);
    
    virtual ~IntGPUBitonicVerticalArraySolver();
    
    /**
     * @Override
     */
    virtual void accept(const BitonicVerticalArrayData<int>& data);
    
    /**
     * @Override
     */
    virtual void solve(const BitonicVerticalArrayInfo<int>& info);
    
private:
    /**
     * Collect elements into the buffer working with GPU from inpur elements
     * @param elementBuffer The buffer working with GPU
     * @param elements      Output elements
     */
    void collectToInoutBuffer(IntBuffer& elementBuffer, const ElementList<int>& elements) const;
    
    /**
     * Collect elements in the buffer working with GPU into output elements
     * @param elementBuffer The buffer working with GPU
     * @param elements      Output elements
     */
    void collectFromInoutBuffer(const IntBuffer& elementBuffer, ElementList<int>& elements) const;
    
    // CL services and configurations
    const std::string m_programName;
    const std::string m_kernelName;
    SimpleCLExecutorFactoryPtr m_pSimpleExecutorFactory;
    WorkDims m_executingDims;
    
    // Internal-used buffer
    IntBuffer m_localBuffer;
    IntBuffer* m_pElementBuffer;
};

#endif /* IntGPUBitonicVerticalArraySolver_h */
