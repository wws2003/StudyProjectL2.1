/*
 * BitSwapSolution.hpp
 *
 *  Created on: May 19, 2019
 *      Author: pham
 */

#ifndef BITSWAPSOLUTION_HPP_
#define BITSWAPSOLUTION_HPP_

#include "ISolution.h"
#include <array>
#include <random>
#include <chrono>
#include <sstream>
#include <algorithm>

/*
 * Multiple bit flip solution
 */
template<std::size_t N>
class BitSwapSolution: public ISolution {
public:
	BitSwapSolution(const std::array<char, N>& arr) : ISolution(),
	m_array(arr),
	m_mt(987654321),
	m_dist(0, N - 1) {
	};

	virtual ~BitSwapSolution(){};

	/**
	 * Get the next solution
	 * @return
	 */
	virtual SolutionPtr getNextSolution();

	virtual SolutionPtr getNextSolution(unsigned int swappedIndex1,
			unsigned int swappedIndex2,
			std::mt19937 mt,
			std::uniform_int_distribution<int> dists) = 0;

protected:
	inline char get(int index){return m_array[index];}

private:
	std::array<char, N> m_array;
	std::mt19937 m_mt;
	std::uniform_int_distribution<int> m_dist;
};

template<std::size_t N>
SolutionPtr BitSwapSolution<N>::getNextSolution() {
	int swappedIndex1 = m_dist(m_mt);
	int swappedIndex2 = swappedIndex1;
	int maxTryCnt = 5;
	int tryCnt = 0;
	while(swappedIndex2 == swappedIndex1 && ++maxTryCnt <= maxTryCnt) {
		swappedIndex2 = m_dist(m_mt);
	}
	if (swappedIndex2 == swappedIndex1) {
		swappedIndex2 = (swappedIndex1 + 1) % N;
	}
	return getNextSolution(swappedIndex1, swappedIndex2, m_mt, m_dist);
}

#endif /* BITSWAPSOLUTION_HPP_ */
