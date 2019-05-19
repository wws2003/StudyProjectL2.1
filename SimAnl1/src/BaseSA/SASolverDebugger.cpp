/*
 * SASolverDebugger.cpp
 *
 *  Created on: May 19, 2019
 *      Author: pham
 */

#include "SASolverDebugger.h"
#include <iostream>

void SASolverDebugger::debug0_step(unsigned int step) {
	executeDebug(0, [this, &step]() {
		this->doDebug0_step(step);
	});
}

void SASolverDebugger::debug1_newSolution(SolutionPtr pNewSolution, energy_t newEnegy) {
	executeDebug(1, [this, &pNewSolution, &newEnegy]() {
		this->doDebug1_newSolution(pNewSolution, newEnegy);
	});
}

void SASolverDebugger::debug2_accept(SolutionPtr pNewSolution) {
	executeDebug(2, [this, &pNewSolution]() {
		this->doDebug2_accept(pNewSolution);
	});
}

void SASolverDebugger::debug2_noaccept(SolutionPtr pNewSolution) {
	executeDebug(2, [this, &pNewSolution]() {
		this->doDebug2_noaccept(pNewSolution);
	});
}

void SASolverDebugger::debug3_currentSolution(SolutionPtr pCurrentSolution) {
	executeDebug(3, [this, &pCurrentSolution]() {
		this->doDebug3_currentSolution(pCurrentSolution);
	});
}

void SASolverDebugger::debug4_bestSolution(SolutionPtr pBestSolution, double temp, energy_t bestEnegy) {
	executeDebug(4, [this, &pBestSolution, &temp, &bestEnegy]() {
		this->doDebug4_bestSolution(pBestSolution, temp, bestEnegy);
	});
}

/*--------------------------Private methods------------------------*/

void SASolverDebugger::executeDebug(int type, std::function<void(void)> debugFunc) {
	if (m_debugTypeEnableMap[type]) {
		debugFunc();
	}
}

void SASolverDebugger::doDebug0_step(unsigned int step) {
	std::cout << "[0000]"
			<< "-------------------------"
			<< " Step: "
			<< step
			<< "-------------------------"
			<< std::endl;
}

void SASolverDebugger::doDebug1_newSolution(SolutionPtr pNewSolution, energy_t newEnergy) {
	std::cout << "[1111]"
			<< "Try "
			<< pNewSolution->getDisp()
			<< " Energy: "
			<< newEnergy
			<< std::endl;
}

void SASolverDebugger::doDebug2_accept(SolutionPtr pNewSolution) {
	std::cout << "[2222][0]"
			<< "Accept "
			<< pNewSolution->getDisp()
			<< std::endl;
}

void SASolverDebugger::doDebug2_noaccept(SolutionPtr pNewSolution) {
	std::cout << "[2222][1]"
			<< "Do not accept "
			<< pNewSolution->getDisp()
			<< std::endl;
}

void SASolverDebugger::doDebug3_currentSolution(SolutionPtr pCurrentSolution) {
	std::cout << "[3333]"
			<< "Current solution: "
			<< pCurrentSolution->getDisp()
			<< std::endl;
}

void SASolverDebugger::doDebug4_bestSolution(SolutionPtr pBestSolution, double temp, energy_t bestEnergy) {
	std::cout << "[4444]"
			<< "At temperature: "
			<< temp
			<< " -> Best solution: "
			<< pBestSolution->getDisp()
			<< " Energy: "
			<< bestEnergy
			<< std::endl;
}
