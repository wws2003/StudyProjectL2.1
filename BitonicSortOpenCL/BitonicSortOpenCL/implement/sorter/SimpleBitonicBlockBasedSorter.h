//
//  SimpleBitonicBlockBasedSorter.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 3/25/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef SimpleBitonicBlockBasedSorter_h
#define SimpleBitonicBlockBasedSorter_h

#include "Common.h"
#include "ISorter.h"
#include "BitonicBlock.h"
#include "IBitonicBlockSolver.h"

template<typename T>
class SimpleBitonicBlockBasedSorter: public ISorter<T> {
public:
    
    SimpleBitonicBlockBasedSorter(BitonicBlockSolverPtr<T> pBitonicBlockSolver);
    
    virtual ~SimpleBitonicBlockBasedSorter(){};
    
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
    
    BitonicBlockSolverPtr<T> m_pBitonicBlockSolver;
};

#endif /* SimpleBitonicBlockBasedSorter_h */
