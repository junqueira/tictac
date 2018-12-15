/*
 * LoggiInstance.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: Rodrigo
 */

#include "LoggiInstance.h"

LoggiInstance::LoggiInstance(const std::string& instanceFile){
	std::ifstream fin(instanceFile.c_str());
	if(! fin) { throw Error("LoggiInstance: Cannot open input file."); }

	fin >> this->numCities;

	for (int i = 0; i < (int) numCities; ++i) {
		int cityCode;

		fin >> cityCode;

		this->index2CityCode[i] = cityCode;

		allDistances.push_back(std::vector<double>());

		fin >> populations[i];

		allPopulation += populations[i];
	}

	for (int i = 0; i < (int) numCities; ++i) {
		for (int j = 0; j < (int) numCities; j++) {
			int distanceFromIToJ;
			fin >> distanceFromIToJ;

			allDistances[i].push_back(distanceFromIToJ);
		}
	}
}

LoggiInstance::~LoggiInstance() { }

int LoggiInstance::getNumCities(){
	return this->numCities;
}

std::vector<std::vector<double>> LoggiInstance::getDistances(){
	return this->allDistances;
}

double LoggiInstance::getDistancePercentage(){
	return 1.0;
}

double LoggiInstance::getCriminalityPercentage(){
	return 1.0;
}

double LoggiInstance::getRoadPercentage(){
	return 1.0;
}

double LoggiInstance::getDensityPercentage(){
	return 1.0;
}

double LoggiInstance::getCostByAir(){
	return 7.0;
}

double LoggiInstance::getCostByRoad(){
	return 9.0;
}

std::unordered_set<int> LoggiInstance::getCitiesWithAirport(){
	return this->citiesWithAirport;
}

std::unordered_map<int, int> LoggiInstance::getPopulations(){
	return this->populations;
}

unsigned LoggiInstance::getMinServedPopulation(){
	//TODO: get real min
	return 0.7 * allPopulation;
}
