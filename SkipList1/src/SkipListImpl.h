/*
 * SkipList.h
 *
 *  Created on: Nov 26, 2017
 *      Author: pham
 */

#include <vector>
#include <functional>
#include <algorithm>
#include "ISkipList.h"
#include "SkipListStack.h"

#ifndef SKIPLIST_H_
#define SKIPLIST_H_

template<typename K>
using CompareFunc = std::function<bool(K, K)>;

template<typename K, typename V>
class SkipListImpl : public ISkipList<K,V> {
public:
	// TODO Add comparator parameter into constructor
	/**
	 * Constructor
	 * @param extremeMinKey
	 * @param extremeMaxKey
	 * @param emptyVal
	 * @param maxLayerCount
	 */
	SkipListImpl(K extremeMinKey, K extremeMaxKey, V emptyVal, int maxLayerCount, CompareFunc<K> compareFunc);
	virtual ~SkipListImpl();

	/**
	 * Insert new element (key-value pair) into the list. <BR>
	 * If the entry with the same key already exists, overwrite it
	 * @param key
	 * @param value
	 * @throws Exception if the insertion failed
	 */
	virtual void insert(K key, V value);

	/**
	 * Check if the list contains the given value
	 * @param key
	 * @return true/false
	 */
	virtual bool contains(K key);

	/**
	 * Delete elements in the list those match with given key
	 * @param key
	 * @return True if there is at least one element deleted. False otherwise
	 */
	virtual bool remove(K key);

	/**
	 * Find values of elements matching the given key
	 * @param key
	 * @param vals Output for the search. Empty if no matched element found
	 */
	virtual void find(K key, VList<V>& vals);

	/**
	 * Get size of the list
	 * @return
	 */
	virtual int size();

	/**
	 * Retrieve elements in the list given indexes
	 * @param startIndex
	 * @param offset
	 * @param elements Output of the retrieval
	 */
	virtual void getSubList(int startIndex, int offset, ElementList<K,V>& elements);

private:

	/**
	 * Get the pointer to the lower bound stack to the given key. <BR>
	 * The result must not be null or tail stack
	 * @param key
	 * @return
	 */
	SkipListStackPtr<K,V> getLowerBoundStack(K key);

	/**
	 * Get the pointer to the lower bound stack to the given key between 2 stacks.
	 * The result must not be null
	 * @param key
	 * @param pHeadStack
	 * @param pTailStack
	 * @param layer
	 * @return
	 */
	SkipListStackPtr<K,V> getLowerBoundStack(K key,
			SkipListStackPtr<K,V> pHeadStack,
			SkipListStackPtr<K,V> pTailStack,
			int layer);

	typedef std::function<StackLinkInfo<K,V>(const SkipListStack<K,V>&, int)> GetLinkedStackFunc;

	/**
	 * Find the closest stack can be linked with current stack at the given layer
	 * @param pStack
	 * @param pBoundStack
	 * @param getLinkedStack
	 * @param layerCount
	 * @return
	 */
	StackLinkInfo<K,V> findClosestStackLinkInfoAtLayer(SkipListStackPtr<K,V> pStack,
			SkipListStackPtr<K,V> pBoundStack,
			GetLinkedStackFunc getLinkedStack,
			int layerCount) const;

	/**
	 * Decide if a stack should build up new layer
	 * @return
	 */
	bool shouldBuildUpStack();

	/**
	 * Update (increment or decrement) distance
	 * of stacks in the left of given stack (exclusive)
	 * to the stacks in the right of given stack (exclusive),
	 * those have the height greater than given stack height
	 * @param pLeftStack
	 * @param distanceDelta
	 */
	void updateLinkInfoDistance(SkipListStackPtr<K,V> pCenterTask, int distanceDelta) const;

	inline bool isLessThan(K key1, K key2) {return m_compareFunc(key1, key2);}

	inline bool isGreaterThan(K key1, K key2) {return m_compareFunc(key2, key1);}

	inline bool isEqual(K key1, K key2) {return !isLessThan(key1, key2) && !isGreaterThan(key1, key2);};

	int m_maxLayerCount;
	CompareFunc<K> m_compareFunc;

	SkipListStackPtr<K,V> m_pHeadStack;
	SkipListStackPtr<K,V> m_pTailStack;
};

#endif /* SKIPLIST_H_ */
