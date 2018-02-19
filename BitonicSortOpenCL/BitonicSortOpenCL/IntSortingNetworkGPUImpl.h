//
//  IntSortingNetworkGPUImpl.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 2/17/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef IntSortingNetworkGPUImpl_h
#define IntSortingNetworkGPUImpl_h

#include "SortingNetworkImpl.h"
#include "common.h"
#include "WorkDims.h"
#include "AbstractCLEngine.h"

class IntSortingNetworkGPUImpl : public SortingNetworkImpl<int> {
public:
    IntSortingNetworkGPUImpl(const ElementList<int>& elements,
                             SimpleCLExecutorFactoryPtr pSimpleExecutorFactory,
                             const WorkDims& workDims,
                             cl_device_type deviceType);
    
    /**
     * @Override
     * Conduct swap though all the network, apply changes into the internal collection
     * @param compareDistance Element[i] will be compared with Element[i + compareDistance]
     * @param sortOrderKeptSwapCnt Number of comparison kept the same order
     * @param compareFunc
     * @param firstSortOrder First comparison order
     */
    virtual void swap(int swapDistance,
                      int sortOrderKeptSwapCnt,
                      SortOrder firstSortOrder);
    
    /**
     * @Override
     * Retrieve back elements into output parameter
     * @param elements
     */
    virtual void collect(ElementList<int>& elements) const;

    
private:
    // CL services and configurations
    SimpleCLExecutorFactoryPtr m_pSimpleExecutorFactory;
    WorkDims m_executingDims;
    cl_device_type m_deviceType;
    
    // Internal-used buffer
    IntBuffer m_elementBuffer;
};

#endif /* IntSortingNetworkGPUImpl_h */
