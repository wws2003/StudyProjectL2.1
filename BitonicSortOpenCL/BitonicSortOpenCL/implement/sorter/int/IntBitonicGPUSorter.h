//
//  IntBitonicGPUSorter.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 3/17/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef IntBitonicGPUSorter_h
#define IntBitonicGPUSorter_h

#include "common.h"
#include "ISorter.h"
#include "WorkDims.h"
#include "AbstractCLEngine.h"

class IntBitonicGPUSorter : public ISorter<int> {
public:
    IntBitonicGPUSorter(SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                        const WorkDims& workDims,
                        cl_device_type deviceType);
    
    virtual ~IntBitonicGPUSorter(){};
    
    // Experiment
    
    /**
     * @Override
     * Conduct sort against input elements then output to another output elements
     * @param inElements
     * @param outElements
     * @param sortOrder
     */
    void sort(const ElementList<int>& inElements,
              ElementList<int>& outElements,
              SortOrder sortOrder) const;
    
private:
    /**
     * Validate size of elements (only accept power of 2)
     * @param elementCnt
     */
    void validateElementsCount(size_t elementCnt) const;
    
    /**
     * Retrieve back elements into output parameter
     * @param elementBuffer
     * @param elements
     */
    void collect(const IntBuffer& elementBuffer, ElementList<int>& elements) const;
    
    // CL services and configurations
    const std::string m_programName;
    const std::string m_kernelName;
    SimpleCLExecutorFactoryPtr m_pSimpleExecutorFactory;
    WorkDims m_executingDims;
    cl_device_type m_deviceType;
};

#endif /* IntBitonicGPUSorter_h */
