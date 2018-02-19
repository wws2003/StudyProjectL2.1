/*
 * SkipListStack.h
 *
 *  Created on: Nov 27, 2017
 *      Author: pham
 */

#ifndef SKIPLISTSTACK_H_
#define SKIPLISTSTACK_H_

#include <vector>
#include "Common.h"
#include "LinkInfo.h"

template<typename K, typename V>
using StackLinkInfo = LinkInfo<SkipListStackPtr<K,V> >;

template<typename K, typename V>
class SkipListStack {
public:
	/**
	 * Construct an empty stack with key and value
	 * @param key
	 * @param value
	 */
	SkipListStack(K key, V value);
	virtual ~SkipListStack();

	/**
	 * Get the value associated to the stack
	 * @return the key
	 */
	inline K getKey() const {return m_key;};

	/**
	 * Get the value associated to the stack
	 * @return the value
	 */
	inline V getValue() const {return m_value;};

	/**
	 * Set the value associated to the stack
	 * @param val the value to set
	 */
	inline void setValue(V val){m_value = val;};

	/**
	 * Get the layer count (i.e. the height) of the stack
	 * @return
	 */
	int getLayerCount() const;

	/**
	 * Push a new layer to the top of the stack
	 */
	void buildUpOneLayer();

	/**
	 * Create right link to the specified stack at the specified layer <BR>
	 * Throw logic error exception if new link created
	 * @param linkInfo Link info for the stack to create left link and distance to it
	 * @param layer The layer
	 * @param forceStackBuildUp True if the current stack can build up layers to meet the given layer
	 * @param forceRightStackBuildUp True if the right stack can build up layers to meet the given layer
	 */
	void linkToRightStackAtLayer(StackLinkInfo<K, V> linkInfo,
			int layer,
			bool forceStackBuildUp = false,
			bool forceRightStackBuildUp = false);

	/**
	 * Get the right link info at the given layer
	 * @param layer
	 * @return
	 */
	StackLinkInfo<K,V> getRightLinkInfoAtLayer(int layer) const;

	/**
	 * Get the left link info at the given layer
	 * @param layer
	 * @return
	 */
	StackLinkInfo<K,V> getLeftLinkInfoAtLayer(int layer) const;

private:
	std::vector<StackLinkInfo<K,V> > m_rightLinkInfoByLayer;
	std::vector<StackLinkInfo<K,V> > m_leftLinkInfoByLayer;
	K m_key;
	V m_value;
};


#endif /* SKIPLISTSTACK_H_ */
