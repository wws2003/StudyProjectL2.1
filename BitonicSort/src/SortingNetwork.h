/*
 * SortingNetwork.h
 *
 *  Created on: Jan 7, 2018
 *      Author: pham
 */

#ifndef SORTINGNETWORK_H_
#define SORTINGNETWORK_H_

#include "Common.h"

template<typename T>
class SortingNetwork {
public:
	/**
	 * Constructor for sorting network
	 * @param elements
	 */
	SortingNetwork(const ElementList<T>& elements){};

	virtual ~SortingNetwork(){};

	/**
	 * Get size of sorting network
	 */
	virtual size_t size() = 0;

	/**
	 * Conduct swap though all the network, apply changes into the internal collection
	 * @param compareDistance Element[i] will be compared with Element[i + compareDistance]
	 * @param sortOrderKeptDistance Number of comparison kept the same order
	 * @param compareFunc
	 * @param firstSortOrder First comparison order
	 */
	virtual void swap(int compareDistance,
			int sortOrderKeptSwapCnt,
			CompareFunc<T> compareFunc,
			SortOrder firstSortOrder) = 0;

	/**
	 * Retrieve back elements into output parameter
	 * @param elements
	 */
	virtual void collect(ElementList<T>& elements) const = 0;
};


#endif /* SORTINGNETWORK_H_ */
