/*
 * SASolver.cpp
 *
 *  Created on: May 12, 2019
 *      Author: pham
 */

#include "SASolver.h"
#include <cmath>
#include <iostream>
#include "SASolverDebugger.h"

SolutionPtr SASolver::solve(SolutionPtr pInitialSolution) {
	double temp = m_initialTemp;

	SASolverDebugger debugger;

	SolutionPtr pCurrentSolution = pInitialSolution;

	// Set current as best solution
	SolutionPtr pBestSolution = pCurrentSolution;
	energy_t bestEnergy = pBestSolution->getEnergy();

	unsigned int step = 1;
	while (temp > m_minTemp && !pCurrentSolution->isTerminateSolution()) {
		energy_t currentEnergy = pCurrentSolution->getEnergy();

		// Try new solution (a small move)
		SolutionPtr pNewSolution = pCurrentSolution->getNextSolution();
		energy_t newEnergy = pNewSolution->getEnergy();

		// DEBUG
		debugger.debug0_step(step++);
		debugger.debug1_newSolution(pNewSolution, newEnergy);

		// Decide if should accept the new move
		if (canAccept(newEnergy, currentEnergy, temp)) {
			debugger.debug2_accept(pNewSolution);
			pCurrentSolution = pNewSolution;
		} else {
			// Forget the new solution forever
			debugger.debug2_noaccept(pNewSolution);
			freePtr(pNewSolution);
		}

		// DEBUG
		debugger.debug3_currentSolution(pCurrentSolution);

		// Renew the best if needed
		currentEnergy = pCurrentSolution->getEnergy();
		if (currentEnergy < bestEnergy) {
			bestEnergy = currentEnergy;
			pBestSolution = pCurrentSolution;
		}

		// DEBUG
		debugger.debug4_bestSolution(pBestSolution, temp, bestEnergy);

		// Cool the system
		temp *= 1 - m_coolingRate;
	}
	return pBestSolution;
}

bool SASolver::canAccept(energy_t newEnergy, energy_t oldEnergy, double temp) {
	// If better solution, immediately accept
	if (newEnergy < oldEnergy) {
		return true;
	}
	// If the new solution is worse, check the probability
	return m_dist(m_mt) < std::exp((oldEnergy - newEnergy)  / temp);
}

