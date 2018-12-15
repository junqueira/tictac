/*
 * LoggiSolver.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: Alan
 */

#include "LoggiSolver.h"
#define INF_DOUBLE 99999999
#define INF_INT 99999999

#include <iostream>

LoggiSolver::LoggiSolver(LoggiInstance& instance, const std::vector< double >& chromosome) {
	//Start the solution cost with 0.0
	this->cost = 0.0;

	std::vector<int> storages;
	
	//check viability
	double servedClients = 0.0;

	for (int i = 0; i < instance.getNumCities(); ++i) {
		int j = i + instance.getNumCities();

		if(chromosome[j] > 0.999){
			storages.push_back(j);
		}
	}

	auto distances = instance.getDistances();
	auto citiesWithAirport = instance.getCitiesWithAirport();
	auto populations = instance.getPopulations();

	//For each city (in the chromosome)
	for (int i = 0; i < instance.getNumCities(); ++i) {
		double cityDesirability = chromosome[i];

		servedClients += populations[i] * cityDesirability;

		double distanceKM = INF_DOUBLE;

		for(auto& storage : storages){
			double newDistance = distances[i][storage];
			distanceKM = newDistance < distanceKM ? newDistance : distanceKM;
		}

		double distanceCost = distanceKM 
			* (citiesWithAirport.count(i) ? instance.getCostByAir() : instance.getCostByRoad());

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

		this->cost += cityCost;
	}

	if(servedClients < instance.getMinServedPopulation()){
		this->cost = INF_INT;
	}
}

LoggiSolver::~LoggiSolver() {
}

double LoggiSolver::getCost(){
	return this->cost;
}

