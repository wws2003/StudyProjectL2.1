/*
 * BitonicSorter.h
 *
 *  Created on: Jan 1, 2018
 *      Author: pham
 */

#ifndef BITONICSORTER_H_
#define BITONICSORTER_H_

#include <map>
#include <functional>
#include <utility>
#include "Common.h"
#include "SortingNetwork.h"

template<typename T>
using LowerUpperGenerateFunc = std::function<std::pair<T,T>(T, T, CompareFunc<T>)>;

template<typename T>
using LowerUpperGenerateFuncMap = std::map<SortOrder, LowerUpperGenerateFunc<T> >;

/*
 * Class to conduct bitonic sort
 */
template<typename T>
class BitonicSorter {
public:
	BitonicSorter(){};
	virtual ~BitonicSorter(){};

	// Experiment

	/**
	 * Conduct sort on a sorting network (which must contain a power of two elements)
	 * @param pSortingNetwork
	 * @param compareFunc
	 * @param sortOrder
	 */
	void sort(SortingNetworkPtr<T> pSortingNetwork,
			CompareFunc<T> compareFunc,
			SortOrder sortOrder) const;

private:
	/**
	 * Validate size of elements (only accept power of 2)
	 * @param elementCnt
	 */
	void validateElementsCount(size_t elementCnt) const;
};

#endif /* BITONICSORTER_H_ */
