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

	std::vector<int> storages = {instance.getCajamar()};
	
	//check viability
	int servedClients = 0.0;

	for (int i = 0; i < instance.getNumCities(); ++i) {
		if(i == instance.getCajamar())
			continue;

		int j = i + instance.getNumCities();

		if(chromosome[j] > 0.500){
			storages.push_back(j);
		}
	}

	auto distances = instance.getDistances();
	auto citiesWithAirport = instance.getCitiesWithAirport();
	auto populations = instance.getPopulations();
	auto criminalities = instance.getCriminalities();
	auto idhs = instance.getIDHs();
	auto wills = instance.getWills();
	auto individualWage = instance.getIndividualWage();

	//For each city (in the chromosome)
	for (int i = 0; i < instance.getNumCities(); ++i) {
		double cityDesirability = chromosome[i];

		servedClients += (int) populations[i] * cityDesirability;

		double distanceKM = INF_DOUBLE;

		for(auto& storage : storages){
			double newDistance = distances[i][storage];

			if(storage == i){
				if(i == instance.getCajamar()){
					newDistance = 0;
				}else{
					newDistance = distances[i][instance.getCajamar()];
				}
			}
			distanceKM = newDistance < distanceKM ? newDistance : distanceKM;
		}

		double distanceCost = distanceKM 
			* (citiesWithAirport.count(i) ? instance.getCostByAir() : instance.getCostByRoad());

		double criminalityCost = criminalities[i];

		double willCost = wills[i];

		double cityCost = 0.0;

		double idhAvg = 0.0;
		double idhMax = 0.0;

		for(auto item : idhs){
			double idh = item.second;
			idhAvg += idh;
			idhMax = idh > idhMax ? idh : idhMax;
		}

		idhAvg /= idhs.size();

		cityCost += distanceCost * instance.getDistancePercentage();
		cityCost -= populations[i] * instance.getPopulationPercentage();
		cityCost += criminalityCost * instance.getCriminalityPercentage();
		cityCost += willCost * instance.getWillPercentage();
		cityCost -= individualWage[i] * instance.getIndividualWagePercentage();
		cityCost += (1 - (0.1 * ((idhs[i] - idhAvg) / (idhMax)))) * distanceKM * instance.getIDHPercentage();

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

