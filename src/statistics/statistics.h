/**
* @file   statistics/statistics.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains an class, which stores current information about the particle swarm.
*
* @copyright
* This project is released under the MIT License (MIT).
*
* @copyright
* The MIT License (MIT)
*
* @copyright
* Copyright (c) 2016 by Friedrich-Alexander-Universität Erlangen-Nürnberg and
* Alexander Raß
*
* @copyright
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* @copyright
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* @copyright
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#ifndef HIGH_PRECISION_PSO_STATISTICS_STATISTICS_H_
#define HIGH_PRECISION_PSO_STATISTICS_STATISTICS_H_

#include <fstream>
#include <string>
#include <vector>

#include "general/general_objects.h"
#include "general/particle.h"

namespace highprecisionpso {

/**
* @brief This class stores current information about the particle swarm.
*/
class Statistics {
public:
	/**
	* @brief The constructor.
	*
	* @param initial_swarm The list of particles, which are contained in the current swarm.
	*/
	Statistics(std::vector<Particle*>* initial_swarm);

	/**
	* @brief If the current iteration counter has a value, where statistical calculations are expected,
	* then the values of all statistics are evaluated and stored in respective vectors.
	*/
	void EvaluateStatistics();
	
	/**
	* @brief Loads data to reset the state of the statistics object to the state which was stored.
	*
	* @param input_stream The stream which contains the data about the statistics (iteration counter, attractor positions and update counter).
	* @param version_of_stored_data The version of the program when the backup was created.
	*/
	void LoadData(std::ifstream* input_stream, ProgramVersion* version_of_stored_data);
	/**
	* @brief Stores the current state of the statistics object (iteration counter, attractor positions and update counter) such that it can completely restore the current state with this data.
	*
	* @param output_stream The output stream where the state should be stored.
	*/
	void StoreData(std::ofstream* output_stream);

	/**
	* @brief The list of particles, which are contained in the current swarm.
	*/
	std::vector<Particle*>* swarm;
	/**
	* @brief The iteration numbers of stored (but not yet saved) statistical data.
	*/
	std::vector<long long> stored_statistical_iterations;
	/**
	* @brief Various lists of stored (but not yet saved) statistical data.
	*/
	std::vector<std::vector<std::string> > stored_statistical_data;
	/**
	* @brief For each particle the number of local attractor updates enforced by found positions of this particle.
	*/
	std::vector<long long> local_attractor_update_counter;
	/**
	* @brief For each particle the number of global attractor updates enforced by found positions of this particle.
	*/
	std::vector<long long> global_attractor_update_counter;
	/**
	* @brief The current iteration counter.
	*/
	long long current_iteration;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_STATISTICS_STATISTICS_H_ */
