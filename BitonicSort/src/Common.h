/*
 * Common.h
 *
 *  Created on: Dec 31, 2017
 *      Author: pham
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <cstdlib>
#include <functional>
#include <vector>

#define NullPtr (NULL)

#define freePtr(ptr) {delete ptr;}

template<typename T>
using ElementList = std::vector<T>;

template<typename T>
using ConstElementList = std::vector<const T>;

template<typename K>
using CompareFunc = std::function<bool(K, K)>;

enum SortOrder {
	ASC = 0,
	DESC = 1
};

template<typename T>
class SortingNetwork;

template<typename T>
using SortingNetworkPtr = SortingNetwork<T>*;

#endif /* COMMON_H_ */
