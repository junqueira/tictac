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

	std::vector<std::vector<double>> getDistances();

private:
	unsigned numCities = 0;
	std::vector<std::vector<double>> allDistances = {};
	std::unordered_map<int, int> index2CityCode = {};
};

#endif
