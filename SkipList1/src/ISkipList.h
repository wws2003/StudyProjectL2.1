/*
 * ISkipList.h
 *
 *  Created on: Nov 27, 2017
 *      Author: pham
 */

#ifndef ISKIPLIST_H_
#define ISKIPLIST_H_

#include <vector>
#include <algorithm>

template<typename V>
using VList = std::vector<V>;

template<typename K, typename V>
using ElementList = std::vector<std::pair<K, V>>;

template<typename K, typename V>
class ISkipList {
public:
	virtual ~ISkipList(){};

	/**
	 * Insert new element (key-value pair) into the list. <BR>
	 * If the entry with the same key already exists, overwrite it
	 * @param key
	 * @param value
	 * @throws Exception if the insertion failed
	 */
	virtual void insert(K key, V value) = 0;

	/**
	 * Check if the list contains the given value
	 * @param key
	 * @return true/false
	 */
	virtual bool contains(K key) = 0;

	/**
	 * Delete elements in the list those match with given key
	 * @param key
	 * @return True if there is at least one element deleted. False otherwise
	 */
	virtual bool remove(K key) = 0;

	/**
	 * Find values of elements matching the given key
	 * @param key
	 * @param vals Output for the search. Empty if no matched element found
	 */
	virtual void find(K key, VList<V>& vals) = 0;

	/**
	 * Get size of the list
	 * @return
	 */
	virtual int size() = 0;

	/**
	 * Retrieve elements in the list given indexes
	 * @param startIndex
	 * @param offset
	 * @param elements Output of the retrieval
	 */
	virtual void getSubList(int startIndex, int offset, ElementList<K,V>& elements) = 0;
};

#endif /* ISKIPLIST_H_ */
