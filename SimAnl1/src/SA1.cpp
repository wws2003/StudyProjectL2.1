//============================================================================
// Name        : SA1.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "BaseSA/SASolver.h"
#include "BaseSA/BitFlipSolution.hpp"

using namespace std;

#define BIT_CNT 6

class XBitFlipSolution: public BitFlipSolution<BIT_CNT> {
public:
	XBitFlipSolution(const std::string& arrStr) : BitFlipSolution<BIT_CNT>(arrStr) {
	};
	XBitFlipSolution(const std::array<char, BIT_CNT>& arr) : BitFlipSolution<BIT_CNT>(arr) {
	};
	XBitFlipSolution(const std::array<char, BIT_CNT>& arr,
				std::mt19937 mt,
				std::uniform_int_distribution<int> dists) : BitFlipSolution<6>(arr, mt, dists) {
	};

	virtual ~XBitFlipSolution(){};

	SolutionPtr getNextSolution(const std::array<char, BIT_CNT>& arr,
			std::mt19937 mt,
			std::uniform_int_distribution<int> dists) {
		return SolutionPtr(new XBitFlipSolution(arr, mt, dists));
	}

	energy_t getEnergy(int val) {
		// x^3 - 60*x^2 + 900*x + 100
		energy_t val2 = val * val;
		energy_t val3 = val2 * val;
		return val3 - 60 * val2 + 900 * val + 100;
	}
};

int main() {
	XBitFlipSolution solution0("010010");
	SASolver solver(800.0, 0.01, 1.0);

	SolutionPtr pBestSolution = solver.solve(&solution0);

	if (pBestSolution != &solution0) {
		freePtr(pBestSolution);
	}
	return 0;
}
