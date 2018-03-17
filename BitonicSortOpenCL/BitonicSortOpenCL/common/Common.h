//
//  Common.h
//  BitonicSortOpenCL
//
//  Created by wws2003 on 2/11/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#ifndef Common_h
#define Common_h

#include <cstdlib>
#include <functional>
#include <vector>
#include <OpenCL/OpenCL.h>

#define NullPtr (NULL)

#define freePtr(ptr) {delete ptr;}

template<typename T>
using ElementList = std::vector<T>;

template<typename T>
using ConstElementList = std::vector<const T>;

enum SortOrder {
    ASC = 0,
    DESC = 1
};

template<typename T>
class SortingNetwork;

template<typename T>
using SortingNetworkPtr = SortingNetwork<T>*;

#endif /* Common_h */
