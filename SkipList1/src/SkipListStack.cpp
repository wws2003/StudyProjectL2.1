/*
 * SkipListStack.cpp
 *
 *  Created on: Nov 27, 2017
 *      Author: pham
 */

#include <stdexcept>
#include <functional>
#include "SkipListStack.h"

template<typename K, typename V>
SkipListStack<K, V>::SkipListStack(K key, V value) : m_key(key), m_value(value) {
}

template<typename K, typename V>
SkipListStack<K, V>::~SkipListStack() {
}

template<typename K, typename V>
int SkipListStack<K, V>::getLayerCount() const {
	return m_leftLinkInfoByLayer.size();
}

template<typename K, typename V>
void SkipListStack<K, V>::buildUpOneLayer() {
	m_leftLinkInfoByLayer.push_back(StackLinkInfo<K,V>(NullPtr, 0));
	m_rightLinkInfoByLayer.push_back(StackLinkInfo<K,V>(NullPtr, 0));
}

template<typename K, typename V>
void SkipListStack<K, V>::linkToRightStackAtLayer(StackLinkInfo<K,V> linkInfo,
		int layer,
		bool forceStackBuildUp,
		bool forceRightStackBuildUp) {

	// Function to build up stack if necessary
	std::function<void(SkipListStackPtr<K,V>, int, bool)> stackBuildFunc = [](SkipListStackPtr<K,V> pStack,
			int l,
			bool forceBuildUp) {
		int stackLayerCount = pStack->getLayerCount();
		int requiredLayerCount = l + 1;
		if (stackLayerCount < requiredLayerCount && !forceBuildUp) {
			throw std::logic_error("Stack does not have enough layer to create link");
		}
		while(pStack->getLayerCount() < requiredLayerCount) {
			pStack->buildUpOneLayer();
		}
	};

	// Build up stack if necessary and allowed
	stackBuildFunc(this, layer, forceStackBuildUp);
	stackBuildFunc(linkInfo.m_linkedTarget, layer, forceRightStackBuildUp);

	// Create left and right links
	m_rightLinkInfoByLayer[layer] = linkInfo;
	linkInfo.m_linkedTarget->m_leftLinkInfoByLayer[layer] = StackLinkInfo<K,V>(this, linkInfo.m_distance);
}

template<typename K, typename V>
StackLinkInfo<K,V> SkipListStack<K, V>::getRightLinkInfoAtLayer(int layer) const {
	if (layer >= getLayerCount()) {
		return StackLinkInfo<K,V>(NullPtr, 0);
	}
	return m_rightLinkInfoByLayer[layer];
}

template<typename K, typename V>
StackLinkInfo<K,V> SkipListStack<K, V>::getLeftLinkInfoAtLayer(int layer) const {
	if (layer >= getLayerCount()) {
		return StackLinkInfo<K,V>(NullPtr, 0);
	}
	return m_leftLinkInfoByLayer[layer];
}

/*-------------------------------------Private methods-------------------------------------*/

