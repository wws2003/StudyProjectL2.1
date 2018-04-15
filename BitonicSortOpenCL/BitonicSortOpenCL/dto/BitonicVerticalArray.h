//
//  BitonicVerticalArray.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/14/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef BitonicVerticalArray_h
#define BitonicVerticalArray_h

#include "Common.h"

/**
* Struct to present vertical bin of elements in bitonic sort network
 */
template<typename T>
struct BitonicVerticalArray {
public:
    BitonicVerticalArray(SortOrder sortOrder,
                 size_t swapBlockSize,
                 int sortingDepth,
                 ElementListPtr<T> pElementList) : m_sortOrder(sortOrder),
    m_swapBlockSize(swapBlockSize),
    m_sortingDepth(sortingDepth),
    m_pElementList(pElementList){
    };
    
    // Order
    SortOrder m_sortOrder;
    // Swap block size (range against which comparison and swap can be carried out)
    size_t m_swapBlockSize;
    // Sorting depth (actually can be deduced from swap block size and element count)
    int m_sortingDepth;
    // Reference to main buffer
    ElementListPtr<T> m_pElementList;
};

#endif /* BitonicVerticalArray_h */
