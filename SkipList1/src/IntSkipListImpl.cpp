/*
 * IntSkipListImpl.cpp
 *
 *  Created on: Dec 3, 2017
 *      Author: pham
 */

#include "IntSkipListImpl.h"
#include "SkipListStack.cpp"
#include "SkipListImpl.cpp"

template class SkipListStack<int, int>;
template struct LinkInfo<SkipListStack<int, int> >;
template class SkipListImpl<int, int>;
