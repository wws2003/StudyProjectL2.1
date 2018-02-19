/*
 * SortingNetworkImpl.cpp
 *
 *  Created on: Jan 7, 2018
 *      Author: pham
 */

#include "SortingNetworkImpl.h"

template<typename T>
SortingNetworkImpl<T>::SortingNetworkImpl(const ElementList<T>& elements) : SortingNetwork<T>(elements) {

	this->validateElementsCount(elements.size());

	m_elements = elements;

	m_sortOrderDetectFunc = [](int sortOrderKeptSwapCnt,
			int currentStartIndex,
			SortOrder firstSortOrder,
			SortOrder reversedSortOrder) {
		return ((currentStartIndex / (2 * sortOrderKeptSwapCnt)) % 2 == 0) ? firstSortOrder : reversedSortOrder;
	};

	m_lowerUpperSwapFuncMap[SortOrder::ASC] = [](ElementList<T>& elements,
			unsigned int index1,
			unsigned int index2,
			CompareFunc<T> compareFunc) {
		T e1 = elements[index1];
		T e2 = elements[index2];
		elements[index1] = std::min(e1, e2, compareFunc);
		elements[index2] = std::max(e1, e2, compareFunc);
	};
	m_lowerUpperSwapFuncMap[SortOrder::DESC] = [](ElementList<T>& elements,
			unsigned int index1,
			unsigned int index2,
			CompareFunc<T> compareFunc) {
		T e1 = elements[index1];
		T e2 = elements[index2];
		elements[index1] = std::max(e1, e2, compareFunc);
		elements[index2] = std::min(e1, e2, compareFunc);
	};
}

template<typename T>
size_t SortingNetworkImpl<T>::size() {
	return m_elements.size();
}

template<typename T>
void SortingNetworkImpl<T>::swap(int swapDistance,
		int sortOrderKeptSwapCnt,
		CompareFunc<T> compareFunc,
		SortOrder firstSortOrder) {

	size_t elementCnt = size();
	if (elementCnt < 2) {
		return;
	}
	int numberOfSwapsInOneBlock = swapDistance;
	int numberOfSwapsBlocks = elementCnt / 2 / numberOfSwapsInOneBlock;
	SortOrder reversedSortOrder = (firstSortOrder == SortOrder::ASC) ? SortOrder::DESC : SortOrder::ASC;

	// Conduct swaps in blocks
	for(unsigned int i = 0; i < numberOfSwapsBlocks; i++) {
		// One swap block
		unsigned int startSwapIndex = i * (2 * numberOfSwapsInOneBlock);
		for(unsigned int j = 0; j < numberOfSwapsInOneBlock; j++) {
			unsigned int swapIndex1 = startSwapIndex + j;
			unsigned int swapIndex2 = swapIndex1 + swapDistance;
			// Conduct one swap
			SortOrder sortOrder = m_sortOrderDetectFunc(sortOrderKeptSwapCnt, swapIndex1, firstSortOrder, reversedSortOrder);
			m_lowerUpperSwapFuncMap[sortOrder](m_elements, swapIndex1, swapIndex2, compareFunc);
		}
	}
}

template<typename T>
void SortingNetworkImpl<T>::collect(ElementList<T>& elements) const {
	elements.clear();
	elements.insert(elements.end(), m_elements.begin(), m_elements.end());
}

template<typename T>
void SortingNetworkImpl<T>::validateElementsCount(size_t elementCnt) const {
	while(elementCnt > 1) {
		if (elementCnt % 2 != 0) {
			throw std::invalid_argument("Currently only accept list having number of elements is power of 2");
		}
		elementCnt /= 2;
	}
}
