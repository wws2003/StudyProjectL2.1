/*
 * GraphBiPartioningSolution.cpp
 *
 *  Created on: May 19, 2019
 *      Author: pham
 */

#include "GraphBiPartioningSolution.h"

template<std::size_t N>
std::string GraphBiPartioningSolution<N>::getDisp() {
	std::stringstream partition1Vertices;
	std::stringstream partition2Vertices;

	for(unsigned int i = 0; i < N; i++) {
		if (BitSwapSolution<N>::get(i) == '0') {
			partition1Vertices << i;
		} else {
			partition2Vertices << i;
		}
	}

	std::stringstream paritions;
	paritions << "Partition 1: "
			<< partition1Vertices.str()
			<< std::endl
			<< "Partition 2: "
			<< partition2Vertices.str()
			<< std::endl;

	return paritions.str();
}

template<std::size_t N>
energy_t GraphBiPartioningSolution<N>::getEnergy() {
	if (m_cachedEnergy >= 0) {
		m_cachedEnergy = doGetEnergy();
	}
	return m_cachedEnergy;
}

template<std::size_t N>
SolutionPtr GraphBiPartioningSolution<N>::getNextSolution(unsigned int swappedIndex1,
			unsigned int swappedIndex2,
			std::mt19937 mt,
			std::uniform_int_distribution<int> dists) {

}

template<std::size_t N>
energy_t GraphBiPartioningSolution<N>::doGetEnergy() {
	// TODO Implement
	return 1.0;
}
