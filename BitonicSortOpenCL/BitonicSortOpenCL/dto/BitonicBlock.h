//
//  BitonicBlock.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 3/25/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef BitonicBlock_h
#define BitonicBlock_h

#include "Common.h"

/**
* Struct to represent a sort task used in bitonic sort, with information
* about start index, end index, sort order, operation depth and reference to
* the sorting buffer
*/

template<typename T>
struct BitonicBlock {
public:
    BitonicBlock(SortOrder sortOrder,
                 size_t lowerIndex,
                 size_t higherIndex,
                 int sortingDepth,
                 ElementListPtr<T> pElementList) : m_sortOrder(sortOrder),
    m_lowerIndex(lowerIndex),
    m_higherIndex(higherIndex),
    m_sortingDepth(sortingDepth),
    m_pElementList(pElementList){};
    
    // Order
    SortOrder m_sortOrder;
    // Start index
    size_t m_lowerIndex;
    // End index
    size_t m_higherIndex;
    // Sorting depth
    int m_sortingDepth;
    // Reference to main buffer
    ElementListPtr<T> m_pElementList;
};

#endif /* BitonicBlock_h */
