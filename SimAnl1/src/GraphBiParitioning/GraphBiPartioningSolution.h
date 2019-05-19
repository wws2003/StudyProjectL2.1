/*
 * GraphBiPartioningSolution.h
 *
 *  Created on: May 19, 2019
 *      Author: pham
 */

#ifndef GRAPHBIPARTIONINGSOLUTION_H_
#define GRAPHBIPARTIONINGSOLUTION_H_

#include "BaseSA/ISolution.h"
#include "BaseSA/BitSwapSolution.hpp"
#include "Graph.h"

/*
 * Solution for graph bipartitioning problem
 */
template<std::size_t N>
class GraphBiPartioningSolution: public BitSwapSolution<N> {
public:
	GraphBiPartioningSolution(const std::array<char, N>& arr, GraphPtr pGraph) :
		BitSwapSolution<N>(arr),
		m_pGraph(pGraph),
		m_cachedEnergy(-1.0) {};

	virtual ~GraphBiPartioningSolution(){};

	/**
	 * Get the energy specified by this solution
	 * @return
	 */
	virtual energy_t getEnergy();

	/**
	 * Display of the solution
	 * @return
	 */
	virtual std::string getDisp();

	/**
	 * Get the next solution based on one swapping operation
	 * @param swappedIndex1
	 * @param swappedIndex2
	 * @param mt
	 * @param dists
	 * @return
	 */
	virtual SolutionPtr getNextSolution(unsigned int swappedIndex1,
			unsigned int swappedIndex2,
			std::mt19937 mt,
			std::uniform_int_distribution<int> dists);

private:
	energy_t doGetEnergy();

	GraphPtr m_pGraph;
	energy_t m_cachedEnergy;
};

#endif /* GRAPHBIPARTIONINGSOLUTION_H_ */
