/*
 * Common.h
 *
 *  Created on: Nov 28, 2017
 *      Author: pham
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <cstdlib>

#define NullPtr (NULL)

#define freePtr(ptr) {delete ptr;}

template<typename K, typename V>
class SkipListStack;

template<typename K, typename V>
using SkipListStackPtr = SkipListStack<K,V>*;

template<typename K, typename V>
class ISkipList;

template<typename K, typename V>
using SkipListPtr = ISkipList<K, V>*;

#endif /* COMMON_H_ */
