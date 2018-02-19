/*
 * SkipList.cpp
 *
 *  Created on: Nov 26, 2017
 *      Author: pham
 */

#include <cstdlib>
#include <functional>
#include <iostream>
#include "SkipListImpl.h"

template<typename K, typename V>
SkipListImpl<K,V>::SkipListImpl(K extremeMinKey,
		K extremeMaxKey,
		V emptyVal,
		int maxLayerCount,
		CompareFunc<K> compareFunc) : m_maxLayerCount(maxLayerCount) {
	// Compare function setting
	m_compareFunc = compareFunc;
	// Head stack, tail stack initialization
	m_pHeadStack = SkipListStackPtr<K,V>(new SkipListStack<K,V>(extremeMinKey, emptyVal));
	m_pTailStack = SkipListStackPtr<K,V>(new SkipListStack<K,V>(extremeMaxKey, emptyVal));
	m_pHeadStack->linkToRightStackAtLayer(StackLinkInfo<K,V>(m_pTailStack, 1), 0, true, true);
}

template<typename K, typename V>
SkipListImpl<K,V>::~SkipListImpl() {
	// Free all stacks
	SkipListStackPtr<K,V> pCurrentStack = this->m_pHeadStack;
	while(pCurrentStack != this->m_pTailStack) {
		SkipListStackPtr<K,V> pNextStack = pCurrentStack->getRightLinkInfoAtLayer(0).m_linkedTarget;
		freePtr(pCurrentStack);
		pCurrentStack = pNextStack;
	}
	freePtr(this->m_pTailStack);
}

template<typename K, typename V>
void SkipListImpl<K,V>::insert(K key, V value) {
	SkipListStackPtr<K,V> pLowerBoundStack = this->getLowerBoundStack(key);
	if (isEqual(pLowerBoundStack->getKey(), key)) {
		// An element with the same key already exists -> Overwrite
		pLowerBoundStack->setValue(value);
		return;
	}
	// New key
	// 1. Create new stack with no layer
	SkipListStackPtr<K,V> pNewStack = SkipListStackPtr<K,V>(new SkipListStack<K,V>(key, value));

	// 2. Build up stack and create links
	SkipListStackPtr<K,V> pUpperBoundStack = pLowerBoundStack->getRightLinkInfoAtLayer(0).m_linkedTarget;
	while((this->shouldBuildUpStack() || pNewStack->getLayerCount() == 0)
			&& pNewStack->getLayerCount() < m_maxLayerCount) {
		// 2.1 Build up one layer
		pNewStack->buildUpOneLayer();
		int newStackLayerCount = pNewStack->getLayerCount();

		// 2.2 Left links
		StackLinkInfo<K,V> pClosestLeftLinkInfoAtLayer = this->findClosestStackLinkInfoAtLayer(pLowerBoundStack,
				m_pHeadStack,
				&SkipListStack<K,V>::getLeftLinkInfoAtLayer,
				newStackLayerCount);
		SkipListStackPtr<K,V> pClosestLeftStackAtLayer = pClosestLeftLinkInfoAtLayer.m_linkedTarget;
		int distanceFromClosestLeftStackToNewStack = pClosestLeftLinkInfoAtLayer.m_distance + 1;
		pClosestLeftStackAtLayer->linkToRightStackAtLayer(StackLinkInfo<K,V>(pNewStack, distanceFromClosestLeftStackToNewStack),
				newStackLayerCount - 1,
				pClosestLeftStackAtLayer == m_pHeadStack,
				false);

		// 2.3 Right links
		StackLinkInfo<K,V> pClosestRightLinkInfoAtLayer = this->findClosestStackLinkInfoAtLayer(pUpperBoundStack,
				m_pTailStack,
				&SkipListStack<K,V>::getRightLinkInfoAtLayer,
				newStackLayerCount);
		SkipListStackPtr<K,V> pClosestRightStackAtLayer = pClosestRightLinkInfoAtLayer.m_linkedTarget;
		int distanceFromNewStackToClosestRightStack = pClosestRightLinkInfoAtLayer.m_distance + 1;
		pNewStack->linkToRightStackAtLayer(StackLinkInfo<K,V>(pClosestRightStackAtLayer, distanceFromNewStackToClosestRightStack),
				newStackLayerCount - 1,
				false,
				pClosestRightStackAtLayer == m_pTailStack);
	}

	// 3.Update distance
	updateLinkInfoDistance(pNewStack, 1);
}

template<typename K, typename V>
bool SkipListImpl<K,V>::contains(K key) {
	SkipListStackPtr<K,V> pLowerBoundStack = this->getLowerBoundStack(key);
	return isEqual(pLowerBoundStack->getKey(), key);
}

template<typename K, typename V>
void SkipListImpl<K,V>::find(K key, VList<V>& vals) {
	vals.clear();
	SkipListStackPtr<K,V> pLowerBoundStack = this->getLowerBoundStack(key);
	if (isEqual(pLowerBoundStack->getKey(), key)) {
		vals.push_back(pLowerBoundStack->getValue());
	}
}


template<typename K, typename V>
bool SkipListImpl<K,V>::remove(K key) {
	SkipListStackPtr<K,V> pLowerBoundStack = this->getLowerBoundStack(key);
	if (!isEqual(pLowerBoundStack->getKey(), key)) {
		return false;
	}
	// Update distance above removed stack height
	updateLinkInfoDistance(pLowerBoundStack, -1);

	// Update link with in removed stack height
	int layerCount = pLowerBoundStack->getLayerCount();
	for (int l = 0; l < layerCount; l++) {
		StackLinkInfo<K,V> leftLinkInfo = pLowerBoundStack->getLeftLinkInfoAtLayer(l);
		StackLinkInfo<K,V> rightLinkInfo = pLowerBoundStack->getRightLinkInfoAtLayer(l);
		SkipListStackPtr<K,V> pLeftStack = leftLinkInfo.m_linkedTarget;
		int newDistance = leftLinkInfo.m_distance + rightLinkInfo.m_distance- 1;
		rightLinkInfo.m_distance = newDistance;
		pLeftStack->linkToRightStackAtLayer(rightLinkInfo, l);
	}
	freePtr(pLowerBoundStack);
	return true;
}

template<typename K, typename V>
int SkipListImpl<K,V>::size() {
	int highestLayer = m_pHeadStack->getLayerCount() - 1;

	SkipListStackPtr<K,V>  pCurrentStack = m_pHeadStack;
	int count = 0;
	while (pCurrentStack != m_pTailStack && pCurrentStack != NullPtr) {
		StackLinkInfo<K,V> rightLinkInfo = pCurrentStack->getRightLinkInfoAtLayer(highestLayer);
		count += rightLinkInfo.m_distance;
		pCurrentStack = rightLinkInfo.m_linkedTarget;
	}

	return count - 1;
}

template<typename K, typename V>
void SkipListImpl<K,V>::getSubList(int startIndex, int offset, ElementList<K,V>& elements) {
	// Clear destination list
	elements.clear();
	// Find pStartStack
	SkipListStackPtr<K,V> pStartStack = m_pHeadStack->getRightLinkInfoAtLayer(0).m_linkedTarget;
	int probingStartIndex = 0;
	int probingLayer = pStartStack->getLayerCount() - 1;

	while (pStartStack != m_pTailStack && pStartStack != NullPtr && probingStartIndex != startIndex) {
		// Try to move to right stacks at the current probing layer
		StackLinkInfo<K,V> rightLinkInfo = pStartStack->getRightLinkInfoAtLayer(probingLayer);
		int distanceAtProbingLayer = rightLinkInfo.m_distance;
		SkipListStackPtr<K,V> pRightLinkedStack = rightLinkInfo.m_linkedTarget;
		int nextProbingStartIndex = probingStartIndex + distanceAtProbingLayer;
		// Evaluate current status
		if (nextProbingStartIndex <= startIndex) {
			// Still can not meet start index or just meet start index -> go to right stack
			pStartStack = pRightLinkedStack;
			probingLayer = pRightLinkedStack->getLayerCount() - 1;
			probingStartIndex = nextProbingStartIndex;
		} else {
			// Moved over start index -> go down one layer
			probingLayer--;
		}
	}

	// Traverse along the list from start stack
	int insertedCnt = 0;
	while (pStartStack != m_pTailStack && pStartStack != NullPtr && insertedCnt++ < offset) {
		elements.push_back(std::pair<K,V>(pStartStack->getKey(), pStartStack->getValue()));
		pStartStack = pStartStack->getRightLinkInfoAtLayer(0).m_linkedTarget;
	}
}


/*---------------------------Private methods implementation--------------------*/

template<typename K, typename V>
SkipListStackPtr<K,V> SkipListImpl<K,V>::getLowerBoundStack(K key) {
	return this->getLowerBoundStack(key,
			m_pHeadStack,
			m_pTailStack,
			m_pHeadStack->getLayerCount() - 1);
}

template<typename K, typename V>
SkipListStackPtr<K,V> SkipListImpl<K,V>::getLowerBoundStack(K key,
		SkipListStackPtr<K,V> pHeadStack,
		SkipListStackPtr<K,V> pTailStack,
		int layer) {

	// Boundary check
	if (layer < 0) {
		return pHeadStack;
	}
	if (pHeadStack->getRightLinkInfoAtLayer(0).m_linkedTarget == pTailStack) {
		return pHeadStack;
	}
	if (isEqual(pHeadStack->getKey(), key)) {
		return pHeadStack;
	}

	// 1. Find mid stack
	SkipListStackPtr<K,V> pMidStack = pHeadStack->getRightLinkInfoAtLayer(layer).m_linkedTarget;

	// 2. Find search range
	SkipListStackPtr<K,V> pPrevStack = pHeadStack;
	while(pMidStack != pTailStack && !isGreaterThan(pMidStack->getKey(), key)) {
		pPrevStack = pMidStack;
		pMidStack = pMidStack->getRightLinkInfoAtLayer(layer).m_linkedTarget;
	}

	// 3. Recursive call
	return getLowerBoundStack(key, pPrevStack, pMidStack, layer - 1);
}

template<typename K, typename V>
StackLinkInfo<K,V> SkipListImpl<K,V>::findClosestStackLinkInfoAtLayer(SkipListStackPtr<K,V> pStack,
		SkipListStackPtr<K,V> pBoundStack,
		GetLinkedStackFunc getLinkedStack,
		int layerCount) const {

	SkipListStackPtr<K,V> pCurrentStack = pStack;
	int distance = 0;
	while(pCurrentStack != pBoundStack && pCurrentStack->getLayerCount() < layerCount) {
		int highestLayer = pCurrentStack->getLayerCount() - 1;
		StackLinkInfo<K,V> nextLinkInfo = getLinkedStack(*pCurrentStack, highestLayer);
		pCurrentStack = nextLinkInfo.m_linkedTarget;
		distance += nextLinkInfo.m_distance;
	}
	return StackLinkInfo<K,V>(pCurrentStack, distance);
}

template<typename K, typename V>
bool SkipListImpl<K,V>::shouldBuildUpStack() {
	int rand = std::rand();
	return rand > RAND_MAX / 2;
}

template<typename K, typename V>
void SkipListImpl<K,V>::updateLinkInfoDistance(SkipListStackPtr<K,V> pCenterTask, int deltaDistance) const {
	// Function to update distance to right stacks
	std::function<void(SkipListStackPtr<K,V>, int, int)> updateLinkInfoFromLayer = [](SkipListStackPtr<K,V> pStack,
			int _fromLayer,
			int _deltaDistance) {
		int maxLayer = pStack->getLayerCount() - 1;
		int layer = _fromLayer;
		while(layer <= maxLayer) {
			StackLinkInfo<K,V> rightLinkInfoAtLayer = pStack->getRightLinkInfoAtLayer(layer);
			SkipListStackPtr<K,V> pRightStack = rightLinkInfoAtLayer.m_linkedTarget;
			int distanceToRightStack = rightLinkInfoAtLayer.m_distance + _deltaDistance;
			pStack->linkToRightStackAtLayer(StackLinkInfo<K,V>(pRightStack, distanceToRightStack),
					layer);
			layer++;
		}
	};

	// Function to find next left stack (higher than the given stack)
	std::function<SkipListStackPtr<K,V>(SkipListStackPtr<K,V>)> findNewLeftStacFunc = [](SkipListStackPtr<K,V> pStack) {
		SkipListStackPtr<K,V> _pLeftStack = pStack;
		int currentLayerCount = pStack->getLayerCount();
		while(_pLeftStack != NullPtr) {
			int leftStackLayerCount = _pLeftStack->getLayerCount();
			if (leftStackLayerCount > currentLayerCount) {
				break;
			}
			_pLeftStack = _pLeftStack->getLeftLinkInfoAtLayer(leftStackLayerCount - 1).m_linkedTarget;
		}
		return _pLeftStack;
	};

	SkipListStackPtr<K,V> pLeftPivotStack = pCenterTask;
	while(pLeftPivotStack != m_pHeadStack && pLeftPivotStack != NullPtr) {
		int maxLayer = pLeftPivotStack->getLayerCount() - 1;
		// Update pLeftStack
		pLeftPivotStack = findNewLeftStacFunc(pLeftPivotStack);
		// Update link info
		if (pLeftPivotStack != NullPtr) {
			updateLinkInfoFromLayer(pLeftPivotStack, maxLayer + 1, deltaDistance);
		}
	}
}
