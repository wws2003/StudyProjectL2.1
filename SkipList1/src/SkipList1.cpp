//============================================================================
// Name        : SkipList1.cpp
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
#include "SkipListImpl.h"
#include "ISkipList.h"
#include "IntSkipListImpl.h"

using namespace std;

#define TEST_DATA_SIZE (240000)
#define MAX_LAYER_CNT (22)

typedef int Entry;
typedef std::vector<int> Entries;

/**
 * Initialize the test data
 * @param entries
 */
void initTestData(Entries& entries);

/**
 * Test insertion into skiplist against the given entries
 * @param entries
 * @param pSkipListRef
 */
void testInsert(const Entries& entries, SkipListPtr<int, int>& pSkipListRef);

/**
 * Test find operation of the skiplist against the given entries
 * @param entries
 * @param pSkipList
 */
void testFind(const Entries& entries, SkipListPtr<int, int> pSkipList);

/**
 * Test sublist retrieval operation of the skiplist
 * @param entries
 * @param pSkipList
 */
void testSubList(const Entries& entries, SkipListPtr<int, int> pSkipList);

/**
 * Test remove operation of the skiplist against the given entries
 * @param entries
 * @param pSkipList
 */
void testRemove(const Entries& entries, SkipListPtr<int, int> pSkipList);

std::function<bool(int, int)> compareFunc = [](int lhs, int rhs) {
	return lhs < rhs;
};

int main() {
	std::srand(std::time(0)); // use current time as seed for random generator
	Entries entries;
	SkipListPtr<int, int> pSkipList;
	initTestData(entries);
	std::cout << "Finish initialize data, starting test" << std::endl;
	testInsert(entries, pSkipList);
	testFind(entries, pSkipList);
	testSubList(entries, pSkipList);
	testRemove(entries, pSkipList);
	freePtr(pSkipList);
	return 0;
}

void initTestData(Entries& entries) {
	entries.clear();
	for (int i = 0; i < TEST_DATA_SIZE; i++) {
		entries.push_back(i);
	}
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(entries.begin(), entries.end(), g);
}

void testInsert(const Entries& entries, SkipListPtr<int, int>& pSkipListRef) {
	SkipListPtr<int, int> pSkipList = SkipListPtr<int, int>(new IntSkipListImpl(INT32_MIN,
			INT32_MAX,
			0,
			MAX_LAYER_CNT,
			compareFunc));
	for(Entry entry : entries) {
		pSkipList->insert(entry, entry);
	}
	int size = entries.size();
	int skipListSize = pSkipList->size();
	// DEBUG
	std::cout << "Skip list size " << skipListSize << std::endl;
	assert(skipListSize == size);

	pSkipListRef = pSkipList;
	std::cout << "Test insert finished" << std::endl;
}

void testFind(const Entries& entries, SkipListPtr<int, int> pSkipList) {
	std::cout << "Start test find" << std::endl;
	for(Entry entry : entries) {
		VList<Entry> vals;
		pSkipList->find(entry, vals);
		assert(vals.size() == 1);
		assert(vals[0] == entry);
	}

	int entriesCnt = entries.size();
	for (Entry entry = entriesCnt; entry < entriesCnt + 1000; entry++) {
		VList<Entry> vals;
		pSkipList->find(entry, vals);
		assert(vals.empty());
	}
	std::cout << "Test find finished" << std::endl;
}

void testSubList(const Entries& entries, SkipListPtr<int, int> pSkipList) {
	unsigned int entriesCnt = entries.size();

	Entries sortedEntries(entries.begin(), entries.end());
	std::sort(sortedEntries.begin(), sortedEntries.end());

	std::function<bool(const Entries&, const ElementList<Entry, Entry>&, int, int)> elementsContentCheck = [](const Entries& originalEntries,
			const ElementList<Entry, Entry>& elementList,
			int startIndex,
			int offset) {
		for (int index = startIndex; index <= startIndex + offset - 1; index++) {
			int i = index - startIndex;
			Entry entry = originalEntries[index];
			assert(elementList[i].first == entry);
			assert(elementList[i].second == entry);
		}
		return true;
	};

	// Sublist starts at the head
	unsigned int halfListSize = entriesCnt / 2;
	ElementList<Entry, Entry> headElements;
	pSkipList->getSubList(0, halfListSize, headElements);
	assert(headElements.size() == halfListSize);
	elementsContentCheck(sortedEntries, headElements, 0, halfListSize);

	// Sublist in the middle
	unsigned int quarterListSize = entriesCnt / 4;
	ElementList<Entry, Entry> middleElements;
	pSkipList->getSubList(halfListSize, quarterListSize, middleElements);
	assert(middleElements.size() == quarterListSize);
	elementsContentCheck(sortedEntries, middleElements, halfListSize, quarterListSize);

	// Sublist ends at the tail
	unsigned int halfListSizeToTail =  entriesCnt - halfListSize;
	ElementList<Entry, Entry> tailElements;
	pSkipList->getSubList(halfListSize, halfListSizeToTail, tailElements);
	assert(tailElements.size() == halfListSizeToTail);
	elementsContentCheck(sortedEntries, tailElements, halfListSize, halfListSizeToTail);

	std::cout << "Test sublist finished" << std::endl;
}

void testRemove(const Entries& entries, SkipListPtr<int, int> pSkipList) {
	std::cout << "Start test remove" << std::endl;

	// Remove first 30 elements
	int removedEntriesCnt = 30;
	for (int i = 0; i < removedEntriesCnt; i++) {
		assert(pSkipList->remove(entries[i]));
		assert(!pSkipList->remove(entries[i]));
	}

	// Assert new size
	int remainingEntriesCnt = entries.size() - removedEntriesCnt;
	assert(pSkipList->size() == remainingEntriesCnt);

	std::cout << "Test remove finished" << std::endl;
}
