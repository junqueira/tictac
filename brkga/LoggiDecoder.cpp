/*
 * TSPDecoder.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: Alan
 */

#include "LoggiDecoder.h"

LoggiDecoder::LoggiDecoder(LoggiInstance& _instance) : instance(_instance) {
}

LoggiDecoder::~LoggiDecoder() {
}

double LoggiDecoder::decode(const std::vector< double >& chromosome) const {
	LoggiSolver solver(instance, chromosome);

	const unsigned fitness = solver.getCost();

	return double(fitness);
}

