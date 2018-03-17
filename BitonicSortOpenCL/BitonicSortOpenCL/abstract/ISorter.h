//
//  ISorter.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 3/17/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef ISorter_h
#define ISorter_h

#include "Common.h"

/**
 Interface for generic sorter
 */

template<typename T>
class ISorter {
public:
    ISorter(){};
    virtual ~ISorter(){};
    
    virtual void sort(const ElementList<T>& inElements,
                      ElementList<T>& outElements,
                      SortOrder sortOrder) const = 0;
};


#endif /* ISorter_h */
