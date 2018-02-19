//
//  BitonicSorter.hpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 2/11/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef BitonicSorter_hpp
#define BitonicSorter_hpp

#include "Common.h"

#include <map>
#include <functional>
#include <utility>
#include "Common.h"
#include "SortingNetwork.h"

/*
 * Class to conduct bitonic sort
 */
template<typename T>
class BitonicSorter {
public:
    BitonicSorter(){};
    virtual ~BitonicSorter(){};
    
    // Experiment
    
    /**
     * Conduct sort on a sorting network (which must contain a power of two elements)
     * @param pSortingNetwork
     * @param sortOrder
     */
    void sort(SortingNetworkPtr<T> pSortingNetwork,
              SortOrder sortOrder) const;
    
private:
    /**
     * Validate size of elements (only accept power of 2)
     * @param elementCnt
     */
    void validateElementsCount(size_t elementCnt) const;
};


#endif /* BitonicSorter_hpp */
