//
//  BitonicVerticalArrayInfo.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/16/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef BitonicVerticalArrayInfo_h
#define BitonicVerticalArrayInfo_h

#include "Common.h"

/**
 * Struct to present vertical bin of elements in bitonic sort network
 */
template<typename T>
struct BitonicVerticalArrayInfo {
public:
    BitonicVerticalArrayInfo(SortOrder sortOrder,
                         size_t orderKeptBlockSize,
                         size_t swapBlockSize,
                         int sortingDepth) : m_sortOrder(sortOrder),
    m_orderKeptBlockSize(orderKeptBlockSize),
    m_swapBlockSize(swapBlockSize),
    m_sortingDepth(sortingDepth) {
    };
    
    // Order
    SortOrder m_sortOrder;
    
    // Order kept comparison count (i.e. number of sequential elements keeping the same sort order)
    size_t m_orderKeptBlockSize;
    
    // Swap block size (range against which comparison and swap can be carried out)
    size_t m_swapBlockSize;
    
    // Sorting depth (actually can be deduced from swap block size and element count)
    int m_sortingDepth;
};


#endif /* BitonicVerticalArrayInfo_h */
