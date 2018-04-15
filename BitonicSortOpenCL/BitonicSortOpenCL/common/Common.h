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
using ElementListPtr = ElementList<T>*;

template<typename T>
using ConstElementList = std::vector<const T>;

#define HOST_SWAP(a,b) {int aux = a; a = b; b = aux;}

enum SortOrder {
    ASC = 0,
    DESC = 1
};

#define HOST_COMPARE_AND_SWAP(a,b,order) {if ((a > b && order == SortOrder::ASC) || (a < b && order == SortOrder::DESC)) {int aux = a; a = b; b = aux;} }

#define HOST_REVERSED_ORDER(order) (order == SortOrder::ASC ? SortOrder::DESC : SortOrder::ASC)

enum MACHINE {
    HOST = 0,
    DEVICE = 1
};

template<typename T>
class SortingNetwork;

template<typename T>
using SortingNetworkPtr = SortingNetwork<T>*;

template<typename T>
class IBitonicBlockSolver;

template<typename T>
using BitonicBlockSolverPtr = IBitonicBlockSolver<T>*;

template<typename T>
class IBitonicVerticalArraySolver;

template<typename T>
using BitonicVerticalArraySolverPtr = IBitonicVerticalArraySolver<T>*;

#endif /* Common_h */
