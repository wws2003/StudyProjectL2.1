/*
 * ISolution.h
 *
 *  Created on: May 12, 2019
 *      Author: pham
 */

#ifndef ISOLUTION_H_
#define ISOLUTION_H_

#include "Common/Common.h"
#include <string>

class ISolution {
public:
	ISolution(){};
	virtual ~ISolution(){};

	/**
	 * Get the energy specified by this solution
	 * @return
	 */
	virtual energy_t getEnergy() = 0;

	/**
	 * Get the next solution
	 * @return
	 */
	virtual SolutionPtr getNextSolution() = 0;

	/**
	 * Display of the solution
	 * @return
	 */
	virtual std::string getDisp() = 0;

	/**
	 * Check if the solution is the final one
	 * @return
	 */
	virtual bool isTerminateSolution() {return false;};
};


#endif /* ISOLUTION_H_ */
