
//
//  BitonicVerticalArrayData.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 4/19/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef BitonicVerticalArrayData_h
#define BitonicVerticalArrayData_h

#include "Common.h"

/**
 * Simple struct to present internal data of a vertical array (now just a simple array)
 */
template<typename T>
struct BitonicVerticalArrayData {
public:
    BitonicVerticalArrayData(T* data, size_t size) {
        m_data = data;
        m_size = size;
    };
    
    virtual ~BitonicVerticalArrayData() {};
    
    T* m_data;
    size_t m_size;
};

#endif /* BitonicVerticalArrayData_h */
