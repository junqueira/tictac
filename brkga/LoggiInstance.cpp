/*
 * LoggiInstance.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: Rodrigo
 */

#include "LoggiInstance.h"
#include <iostream>

LoggiInstance::LoggiInstance(const std::string& instanceFile){
	std::ifstream fin(instanceFile.c_str());
	if(! fin) { throw Error("LoggiInstance: Cannot open input file."); }

	fin >> this->numCities;

	for (int i = 0; i < (int) numCities; ++i) {
		int cityCode;

		fin >> cityCode;

		if(cityCode == 3509205){
			cajamar = i;
		}

		this->index2CityCode[i] = cityCode;

		fin >> populations[i];

		allPopulation += populations[i];
	}

	for (int i = 0; i < (int) numCities; ++i) {
		allDistances.push_back(std::vector<double>());

		for (int j = 0; j < (int) numCities; j++) {
			double distanceFromIToJ;
			fin >> distanceFromIToJ;

			allDistances[i].push_back(distanceFromIToJ);
		}
	}
}

LoggiInstance::~LoggiInstance() { }

int LoggiInstance::getCityId(int index){
	return index2CityCode[index];
}

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

double LoggiInstance::getWillPercentage(){
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

std::unordered_map<int, int> LoggiInstance::getWills(){
	return this->wills;
}

std::unordered_map<int, int> LoggiInstance::getCriminalities(){
	return this->criminalities;
}

std::unordered_map<int, int> LoggiInstance::getPopulations(){
	return this->populations;
}

unsigned LoggiInstance::getMinServedPopulation(){
	return 0.6 * allPopulation;
}

int LoggiInstance::getStorageCost(){
	return 90000;
}

unsigned LoggiInstance::getCajamar(){
	return this->cajamar;
}
