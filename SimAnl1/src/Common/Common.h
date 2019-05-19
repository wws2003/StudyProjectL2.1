/*
 * Common.h
 *
 *  Created on: May 12, 2019
 *      Author: pham
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <cstdlib>

typedef double energy_t;

#define NullPtr (NULL)

#define freePtr(ptr) {delete ptr;}

class ISolution;
typedef ISolution* SolutionPtr;

class Graph;
typedef Graph* GraphPtr;

#endif /* COMMON_H_ */
