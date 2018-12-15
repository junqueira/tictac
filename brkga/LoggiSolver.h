/*
 * LoggiSolver.h
 *
 *  Created on: Dec 15, 2018
 *      Author: Alan
 */

#ifndef LoggiSOLVER_H
#define LoggiSOLVER_H

#include <list>
#include <limits>
#include <vector>
#include <algorithm>
#include "LoggiInstance.h"

class LoggiSolver {
public:
	LoggiSolver(LoggiInstance& instance, const std::vector< double >& chromosome);
	virtual ~LoggiSolver();

	double getCost();

private:
	double cost = 0.0;
};

#endif
