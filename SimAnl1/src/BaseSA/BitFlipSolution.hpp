/*
 * BitFlipSolution.h
 *
 *  Created on: May 15, 2019
 *      Author: pham
 */

#ifndef BITFLIPSOLUTION_H_
#define BITFLIPSOLUTION_H_

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
class BitFlipSolution: public ISolution {
public:
	BitFlipSolution(const std::string& arrStr) : ISolution(),
	m_array(getArrayFromString(arrStr)),
	m_mt(987654321),
	m_dist(0, N - 1) {
	};

	BitFlipSolution(const std::array<char, N>& arr) : ISolution(),
			m_array(arr),
			m_mt(987654321),
			m_dist(0, N - 1) {
	};

	virtual ~BitFlipSolution(){};

	/**
	 * Get the energy specified by this solution
	 * @return
	 */
	virtual energy_t getEnergy() {
		return getEnergy(getIntVal());
	};

	/**
	 * Get the next solution
	 * @return
	 */
	virtual SolutionPtr getNextSolution() {
		int flipedIndex = m_dist(m_mt);
		std::array<char, N> newArr = m_array;
		newArr[flipedIndex] = newArr[flipedIndex] == '0' ? '1' : '0';
		return getNextSolution(newArr, m_mt, m_dist);
	}

	/**
	 * Display of the solution
	 * @return
	 */
	virtual std::string getDisp() {
		std::stringstream ss;
		std::for_each(m_array.begin(), m_array.end(), [&ss](char &c){ss << c;});
		ss << " " << getIntVal();
		return ss.str();
	}

	//----------------------Pure virtual methods-------------------------

	virtual energy_t getEnergy(int val) = 0;

	virtual SolutionPtr getNextSolution(const std::array<char, N>& arr,
			std::mt19937 mt,
			std::uniform_int_distribution<int> dists) = 0;

protected:
	int getIntVal() {
		int v = 0;
		for (unsigned int i = 0; i < N; i++) {
			v = (v << 1) + (m_array[i] == '0' ? 0 : 1);
		}
		return v;
	}

	BitFlipSolution(const std::array<char, N>& arr,
			std::mt19937 mt,
			std::uniform_int_distribution<int> dists) : ISolution(),
					m_array(arr),
					m_mt(mt),
					m_dist(dists){};

private:
	std::array<char, N> getArrayFromString(const std::string& str) {
		std::array<char, N> arr;
		for(unsigned int i = 0; i < N; i++) {
			arr[i] = str[i];
		}
		return arr;
	}

	std::array<char, N> m_array;
	std::mt19937 m_mt;
	std::uniform_int_distribution<int> m_dist;
};

#endif /* BITFLIPSOLUTION_H_ */
