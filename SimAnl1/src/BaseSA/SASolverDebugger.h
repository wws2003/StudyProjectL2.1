/*
 * SASolverDebugger.h
 *
 *  Created on: May 19, 2019
 *      Author: pham
 */

#ifndef SASOLVERDEBUGGER_H_
#define SASOLVERDEBUGGER_H_

#include "Common/Common.h"
#include "ISolution.h"
#include <functional>
#include <array>

#define TYPE_CNT 5

/*
 * Debugger for SA solver
 */
class SASolverDebugger {
public:
	SASolverDebugger() : m_debugTypeEnableMap({true, true, true, true, true}) {};
	SASolverDebugger(std::array<bool, TYPE_CNT> debugTypeEnableMap): m_debugTypeEnableMap(debugTypeEnableMap) {};
	virtual ~SASolverDebugger(){};

	void debug0_step(unsigned int step);

	void debug1_newSolution(SolutionPtr pNewSolution, energy_t newEnegy);

	void debug2_accept(SolutionPtr pNewSolution);

	void debug2_noaccept(SolutionPtr pNewSolution);

	void debug3_currentSolution(SolutionPtr pCurrentSolution);

	void debug4_bestSolution(SolutionPtr pCurrentSolution, double temp, energy_t bestEnegy);

private:
	void executeDebug(int type, std::function<void(void)> debugFunc);

	void doDebug0_step(unsigned int step);

	void doDebug1_newSolution(SolutionPtr pNewSolution, energy_t newEnegy);

	void doDebug2_accept(SolutionPtr pNewSolution);

	void doDebug2_noaccept(SolutionPtr pNewSolution);

	void doDebug3_currentSolution(SolutionPtr pCurrentSolution);

	void doDebug4_bestSolution(SolutionPtr pBestSolution, double temp, energy_t bestEnergy);

	std::array<bool, TYPE_CNT> m_debugTypeEnableMap;
};

#endif /* SASOLVERDEBUGGER_H_ */
