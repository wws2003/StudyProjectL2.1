/*
 * SASolver.h
 *
 *  Created on: May 12, 2019
 *      Author: pham
 */

#ifndef SASOLVER_H_
#define SASOLVER_H_

#include "ISolution.h"
#include <random>
#include <chrono>

/*
 * Class to solve a certain SA problem
 */
class SASolver {
public:
	SASolver(double initialTemp, double coolingRate, double minTemp) :
		m_mt(12345678),
		m_dist(0.0, 1.0),
		m_initialTemp(initialTemp),
		m_coolingRate(coolingRate),
		m_minTemp(minTemp) {
	};

	virtual ~SASolver(){};

	SolutionPtr solve(SolutionPtr pInitialSolution);

private:
	bool canAccept(energy_t newEnergy, energy_t oldEnergy, double temp);

	std::mt19937 m_mt;
	std::uniform_real_distribution<double> m_dist;

	double m_initialTemp;
	double m_coolingRate;
	double m_minTemp;
};

#endif /* SASOLVER_H_ */
