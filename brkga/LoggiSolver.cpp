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

	std::vector<int> storages = {0};
	
	//check viability
	int servedClients = 0.0;

	for (int i = 1; i < instance.getNumCities(); ++i) {
		int j = i + instance.getNumCities();

		if(chromosome[j] > 0.500){
			storages.push_back(j);
		}
	}

	auto distances = instance.getDistances();
	auto citiesWithAirport = instance.getCitiesWithAirport();
	auto populations = instance.getPopulations();
	auto criminalities = instance.getCriminalities();
	auto wills = instance.getWills();

	//For each city (in the chromosome)
	for (int i = 0; i < instance.getNumCities(); ++i) {
		double cityDesirability = chromosome[i];

		servedClients += (int) populations[i] * cityDesirability;

		double distanceKM = INF_DOUBLE;

		for(auto& storage : storages){
			double newDistance = distances[i][storage];
			distanceKM = newDistance < distanceKM ? newDistance : distanceKM;
		}

		double distanceCost = distanceKM 
			* (citiesWithAirport.count(i) ? instance.getCostByAir() : instance.getCostByRoad());

		double criminalityCost = criminalities[i];

		double willCost = wills[i];

		double cityCost = 0.0;

		cityCost += distanceCost * instance.getDistancePercentage();
		cityCost += criminalityCost * instance.getCriminalityPercentage();
		cityCost += willCost * instance.getWillPercentage();

		this->cost += cityCost * cityDesirability;
	}

	this->cost += (storages.size()-1) * instance.getStorageCost();

	if(servedClients < instance.getMinServedPopulation() || storages.size() < 1){
		this->cost = INF_INT;
	}
}

LoggiSolver::~LoggiSolver() {
}

double LoggiSolver::getCost(){
	return this->cost;
}

