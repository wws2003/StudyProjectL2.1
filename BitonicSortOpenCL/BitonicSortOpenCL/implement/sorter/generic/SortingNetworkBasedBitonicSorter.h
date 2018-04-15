//
//  SortingNetworkBasedBitonicSorter.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 3/17/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef SortingNetworkBasedBitonicSorter_h
#define SortingNetworkBasedBitonicSorter_h

#include <map>
#include <functional>
#include <utility>
#include "Common.h"
#include "ISorter.h"
#include "SortingNetwork.h"

/*
 * Class to conduct bitonic sort
 */
template<typename T>
class SortingNetworkBasedBitonicSorter : public ISorter<T> {
public:
    SortingNetworkBasedBitonicSorter(SortingNetworkPtr<T> pSortingNetwork);
    virtual ~SortingNetworkBasedBitonicSorter(){};
    
    // Experiment
    
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
    SortingNetworkPtr<T> m_pSortingNetwork;
    
    /**
     * Validate size of elements (only accept power of 2)
     * @param elementCnt
     */
    void validateElementsCount(size_t elementCnt) const;
};

#endif /* SortingNetworkBasedBitonicSorter_h */
