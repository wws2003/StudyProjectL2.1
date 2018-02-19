/*
 * SortingNetworkImpl.h
 *
 *  Created on: Jan 7, 2018
 *      Author: pham
 */

#ifndef SORTINGNETWORKIMPL_H_
#define SORTINGNETWORKIMPL_H_

#include <map>
#include <functional>
#include "SortingNetwork.h"

template<typename T>
using LowerUpperSwapFunc = std::function<void(ElementList<T>&,
		unsigned int,
		unsigned int,
		CompareFunc<T>)>;

template<typename T>
using LowerUpperSwapFuncMap = std::map<SortOrder, LowerUpperSwapFunc<T> >;

typedef std::function<SortOrder (int, int, SortOrder, SortOrder )> SortOrderDetectFunc;

/*
 * Simple implementation of sorting network
 */
template<typename T>
class SortingNetworkImpl : public SortingNetwork<T> {
public:
	SortingNetworkImpl(const ElementList<T>& elements);
	virtual ~SortingNetworkImpl(){};

	/**
	 * Get size of sorting network
	 */
	virtual size_t size();

	/**
	 * Conduct swap though all the network, apply changes into the internal collection
	 * @param compareDistance Element[i] will be compared with Element[i + compareDistance]
	 * @param sortOrderKeptSwapCnt Number of comparison kept the same order
	 * @param compareFunc
	 * @param firstSortOrder First comparison order
	 */
	virtual void swap(int swapDistance,
			int sortOrderKeptSwapCnt,
			CompareFunc<T> compareFunc,
			SortOrder firstSortOrder);

	/**
	 * Retrieve back elements into output parameter
	 * @param elements
	 */
	virtual void collect(ElementList<T>& elements) const;

private:
	/**
	 * Validate size of elements (only accept power of 2)
	 * @param elementCnt
	 */
	void validateElementsCount(size_t elementCnt) const;

	ElementList<T> m_elements;

	SortOrderDetectFunc m_sortOrderDetectFunc;
	LowerUpperSwapFuncMap<T> m_lowerUpperSwapFuncMap;
};

#endif /* SORTINGNETWORKIMPL_H_ */
