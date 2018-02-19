/*
 * StackLinkInfo.h
 *
 *  Created on: Dec 10, 2017
 *      Author: pham
 */

#ifndef LINKINFO_H_
#define LINKINFO_H_

/**
 * Structure to store info of a link: Target object and distance to it
 */
template<typename T>
struct LinkInfo {
	LinkInfo(T t, int distance) : m_linkedTarget(t), m_distance(distance){};

	T m_linkedTarget;
	int m_distance;
};



#endif /* STACKLINKINFO_H_ */
