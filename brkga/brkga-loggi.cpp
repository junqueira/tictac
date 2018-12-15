/**
 * brkga-Loggi.cpp
 *
 * Driver class with a simple example of how to instantiate and use the BRKGA API to find solutions
 * to the symmetric traveling salesman problem (Loggi) on LoggiLIB instances.
 *
 * See LoggiDecoder.h for details on the decoder's implementation.
 *
 * Created on : Nov 17, 2011 by rtoso
 * Authors    : Rodrigo Franco Toso <rtoso@cs.rutgers.edu>
 *              Mauricio G.C. Resende <mgcr@research.att.com>
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018
 * Rodrigo Franco Toso (rfrancotoso@gmail.com) and
 * Mauricio G.C. Resende
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <iostream>
#include <algorithm>
#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"

#include "LoggiSolver.h"
#include "LoggiDecoder.h"
#include "LoggiInstance.h"

int main(int argc, char* argv[]) {
	if(argc < 2) { std::cerr << "usage: <LoggiLIB-file>" << std::endl; return -1; }

	const clock_t begin = clock();

	const std::string instanceFile = std::string(argv[1]);
	std::cout << "Instance file: " << instanceFile << std::endl;

	// Read the instance:
	LoggiInstance instance(instanceFile); 	// initialize the instance
	LoggiDecoder decoder(instance);		// initialize the decoder

	const long unsigned rngSeed = time(0);	// seed to the random number generator
	MTRand rng(rngSeed);					// initialize the random number generator

	const unsigned n = instance.getNumCities() * 2;		// size of chromosomes
	const unsigned p = 256;		// size of population
	const double pe = 0.10;		// fraction of population to be the elite-set
	const double pm = 0.10;		// fraction of population to be replaced by mutants
	const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
	const unsigned K = 3;		// number of independent populations
	const unsigned MAXT = 2;	// number of threads for parallel decoding

	// initialize the BRKGA-based heuristic
	BRKGA< LoggiDecoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);

	// BRKGA inner loop (evolution) configuration: Exchange top individuals
	const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
	const unsigned X_NUMBER = 2;	// exchange top 2 best
	const unsigned MAX_GENS = 50;	// run for 1000 gens

	// BRKGA evolution configuration: restart strategy
	unsigned relevantGeneration = 1;	// last relevant generation: best updated or reset called
	const unsigned RESET_AFTER = 200;
	std::vector< double > bestChromosome;
	double bestFitness = std::numeric_limits< double >::max();

	// Print info about multi-threading:
	#ifdef _OPENMP
		std::cout << "Running for " << MAX_GENS << " generations using " << MAXT
				<< " out of " << omp_get_max_threads()
				<< " available thread units..." << std::endl;
	#endif
	#ifndef _OPENMP
		std::cout << "Running for " << MAX_GENS
				<< " generations without multi-threading..." << std::endl;
	#endif
	
	// Run the evolution loop:
	unsigned generation = 1;		// current generation
	do {
		std::cout << "GEN: " << generation << std::endl;
		algorithm.evolve();	// evolve the population for one generation

		// Bookeeping: has the best solution thus far improved?
		if(algorithm.getBestFitness() < bestFitness) {
			// Save the best solution to be used after the evolution chain:
			relevantGeneration = generation;
			bestFitness = algorithm.getBestFitness();
			bestChromosome = algorithm.getBestChromosome();
			
			std::cout << "\t" << generation
					<< ") Improved best solution thus far: "
					<< bestFitness << std::endl;
		}

		//  Evolution strategy: restart
		if(generation - relevantGeneration > RESET_AFTER) {
			algorithm.reset();	// restart the algorithm with random keys
			relevantGeneration = generation;
			
			std::cout << "\t" << generation << ") Reset at generation "
					<< generation << std::endl;
		}

		// Evolution strategy: exchange top individuals among the populations
		if(generation % X_INTVL == 0 && relevantGeneration != generation) {
			algorithm.exchangeElite(X_NUMBER);
			
			std::cout << "\t" << generation
					<< ") Exchanged top individuals." << std::endl;
		}

		// Next generation?
		++generation;
	} while (generation < MAX_GENS);

	// print the fitness of the top 10 individuals of each population:
	std::cout << "Fitness of the top 10 individuals of each population:" << std::endl;
	const unsigned bound = std::min(p, unsigned(10));	// makes sure we have 10 individuals
	for(unsigned i = 0; i < K; ++i) {
		std::cout << "Population #" << i << ":" << std::endl;
		for(unsigned j = 0; j < bound; ++j) {
			std::cout << "\t" << j << ") "
					<< algorithm.getPopulation(i).getFitness(j) << std::endl;
		}
	}

	// rebuild the best solution:
	LoggiSolver bestSolution(instance, bestChromosome);

	// print its distance:
	std::cout << "Best solution found has objective value = "
	 		<< bestSolution.getCost() << std::endl;

	const clock_t end = clock();
	std::cout << "BRKGA run finished in " << (end - begin) / double(CLOCKS_PER_SEC) << " s." << std::endl;

	return 0;
}
