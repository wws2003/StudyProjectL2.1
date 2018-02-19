/*
 * BitonicSorter.cpp
 *
 *  Created on: Jan 1, 2018
 *      Author: pham
 */

//#define _DEBUG_MODE 1

#ifdef _DEBUG_MODE
#include <iostream>
#endif
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include "BitonicSorter.h"

template<typename T>
void BitonicSorter<T>::sort(SortingNetworkPtr<T> pSortingNetwork,
		CompareFunc<T> compareFunc,
		SortOrder sortOrder) const {

	size_t elementCnt = pSortingNetwork->size();

	// Validate parameter first
	this->validateElementsCount(elementCnt);

	// Non-recursive approach: BM2 -> BM4 -> BM8 -> ..-> BM(n)
	int stageElementCnt = 2;
	while(stageElementCnt <= elementCnt) {
		int currentStateElementCnt = stageElementCnt;
		// BM2 = Swap
		// BM4 = Swap and then BM2
		// BM8 = Swap and then BM4 and then BM2
		// ...
		while(currentStateElementCnt >= 2) {
			pSortingNetwork->swap(currentStateElementCnt / 2, // Swap distance (half of element count)
					stageElementCnt / 2, // Swap order kept distance (swap block size -invariant in current while loop)
					compareFunc,
					sortOrder);
			currentStateElementCnt /= 2;
		}
		stageElementCnt *= 2;
	}
}

template<typename T>
void BitonicSorter<T>::validateElementsCount(size_t elementCnt) const {
	while(elementCnt > 1) {
		if (elementCnt % 2 != 0) {
			throw std::invalid_argument("Currently only accept list having number of elements is power of 2");
		}
		elementCnt /= 2;
	}
}
