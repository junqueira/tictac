/*
 * LoggiDecoder.h
 *
 *  Created on: Dec 15, 2018
 *      Author: Alan
 */

#ifndef LOGGIDECODER_H
#define LOGGIDECODER_H

#include "LoggiSolver.h"
#include "LoggiInstance.h"

class LoggiDecoder {
public:
	LoggiDecoder(LoggiInstance& instance);
	virtual ~LoggiDecoder();

	double decode(const std::vector< double >& chromosome) const;

private:
	LoggiInstance& instance;
};

#endif
