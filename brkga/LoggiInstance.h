#ifndef LoggiINSTANCE_H
#define LoggiINSTANCE_H

#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

class LoggiInstance {
public:
	typedef std::runtime_error Error;

	LoggiInstance(const std::string& instanceFile);
	virtual ~LoggiInstance();

	int getNumCities();
	double getDistancePercentage();
	double getCriminalityPercentage();
	double getRoadPercentage();
	double getDensityPercentage();
	double getCostByAir();
	double getCostByRoad();

	std::vector<std::vector<double>> getDistances();
	std::unordered_set<int> getCitiesWithAirport();
	std::unordered_map<int, int> getPopulations();
	unsigned getMinServedPopulation();
	int getCityId(int index);
	int getStorageCost();

private:
	unsigned numCities = 0;
	unsigned servedPopulation = 0;
	unsigned allPopulation = 0;
	std::vector<std::vector<double>> allDistances = {};
	std::unordered_map<int, int> index2CityCode = {};
	std::unordered_map<int, int> populations = {};
	std::unordered_set<int> citiesWithAirport;

};

#endif
