/*
 * LoggiSolver.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: Alan
 */

#include "LoggiSolver.h"

LoggiSolver::LoggiSolver(LoggiInstance& instance, const std::vector< double >& chromosome) {
	//Start the solution cost with 0.0
	this->cost = 0.0;

	//For each city (in the chromosome)
	for (int i = 0; i < instance.getNumCities(); ++i) {
		double city = chromosome[i];

		double distanceCost = 0.0;
		//TODO: Measure distance cost

		double criminalityCost = 0.0;
		//TODO: Measure criminality cost

		double roadCost = 0.0;
		//TODO: Measure road cost

		double densityCost = 0.0;
		//TODO: Measure "density" cost

		double cityCost = 0.0;

		cityCost += distanceCost * instance.getDistancePercentage();
		cityCost += criminalityCost * instance.getCriminalityPercentage();
		cityCost += roadCost * instance.getRoadPercentage();
		cityCost += densityCost * instance.getDensityPercentage();

		this->cost += cityCost * city;
	}
}

LoggiSolver::~LoggiSolver() {
}

double LoggiSolver::getCost(){
	return this->cost;
}

