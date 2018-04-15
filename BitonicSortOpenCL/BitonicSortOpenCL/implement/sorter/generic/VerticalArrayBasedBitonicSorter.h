//
//  VerticalArrayBasedBitonicSorter.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/14/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef VerticalArrayBasedBitonicSorter_h
#define VerticalArrayBasedBitonicSorter_h

#include "Common.h"
#include "IBitonicVerticalArraySolver.h"
#include "ISorter.h"

template<typename T>
class VerticalArrayBasedBitonicSorter : public ISorter<T> {
public:
    VerticalArrayBasedBitonicSorter(BitonicVerticalArraySolverPtr<T> pSolver);
    
    virtual ~VerticalArrayBasedBitonicSorter(){};
    
    /**
     * @Override
     * Conduct sort on a sorting network (which must contain a power of two elements)
     * @param pSortingNetwork
     * @param sortOrder
     */
    virtual void sort(const ElementList<T>& inElements,
                      ElementList<T>& outElements,
                      SortOrder sortOrder) const;
    
private:
    /**
     * Validate size of elements (only accept power of 2)
     * @param elementCnt
     */
    void validateElementsCount(size_t elementCnt) const;
    
    BitonicVerticalArraySolverPtr<T> m_pSolver;
};


#endif /* VerticalArrayBasedBitonicSorter_h */
