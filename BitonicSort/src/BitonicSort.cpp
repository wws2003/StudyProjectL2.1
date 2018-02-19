//============================================================================
// Name        : BitonicSort.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>
#include "BitonicSorter.h"
#include "IntSortingNetworkImpl.h"

using namespace std;

#define TEST_DATA_SIZE (65536 * 2)

std::function<bool(int, int)> compareFunc = [](int lhs, int rhs) {
	return lhs < rhs;
};

/**
 * Initialize the test data
 * @param entries
 */
void initTestData(ElementList<int>& entries);

/**
 * Test bitonic sort
 * @param entries
 */
void testBitonicSort(const ElementList<int>& entries);

int main() {
	std::srand(std::time(0)); // use current time as seed for random generator
	ElementList<int> entries;
	initTestData(entries);
	testBitonicSort(entries);
	std::cout << "Finish testing bitonic sort\n";
	return 0;
}

void initTestData(ElementList<int>& entries) {
	entries.clear();
	for (int i = 0; i < TEST_DATA_SIZE; i++) {
		entries.push_back(i);
	}
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(entries.begin(), entries.end(), g);
}

void testBitonicSort(const ElementList<int>& entries) {
	SortingNetworkPtr<int> pSortingNetwork = new SortingNetworkImpl<int>(entries);
	BitonicSorter<int> bitonicSorter;
	bitonicSorter.sort(pSortingNetwork, compareFunc, SortOrder::ASC);
	ElementList<int> sortedEntries;
	pSortingNetwork->collect(sortedEntries);

	size_t elementCnt = sortedEntries.size();
	for(unsigned int i = 0; i < elementCnt - 1; i++) {
		assert(compareFunc(sortedEntries[i], sortedEntries[i + 1]));
	}

	freePtr(pSortingNetwork);
}
