/**
* @file   statistics.cpp
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

#include "statistics/statistics.h"

#include <gmp.h>

#include "general/check_condition.h"
#include "general/configuration.h"
#include "arbitrary_precision_calculation/operations.h"
#include "general/particle.h"
#include "statistics/direct_statistics.h"

namespace highprecisionpso {

Statistics::Statistics(std::vector<Particle*>* initialSwarm) {
	swarm = initialSwarm;
	stored_statistical_iterations.clear();
	local_attractor_update_counter = std::vector<long long>(configuration::g_particles, 0);
	global_attractor_update_counter = std::vector<long long>(configuration::g_particles, 0);
	current_iteration = 0;
}

void Statistics::EvaluateStatistics() {
	bool show = false;
	for(unsigned int i = 0; i < configuration::g_iterations_with_statistic_evaluations.size(); i++){
		if(configuration::g_iterations_with_statistic_evaluations[i][0] <= current_iteration
				&& configuration::g_iterations_with_statistic_evaluations[i][1] >= current_iteration
				&& ((current_iteration - configuration::g_iterations_with_statistic_evaluations[i][0]) % configuration::g_iterations_with_statistic_evaluations[i][2] == 0)){

			show = true;
			break;
		}
	}
	if (!show)return;
	mpftoperations::StatisticalCalculationsStart();
	stored_statistical_iterations.push_back(current_iteration);
	for(unsigned int i = 0; i < configuration::g_statistics_list.size(); i++){
		if(stored_statistical_data.size() <= i){
			stored_statistical_data.push_back(std::vector<std::string>());
		}
		stored_statistical_data[i].push_back(configuration::g_statistics_list[i]->EvaluationToString());
	}
	mpftoperations::StatisticalCalculationsEnd();
}

void Statistics::LoadData(std::ifstream* inputstream, ProgramVersion* version_of_stored_data){
	(*inputstream) >> current_iteration;
	AssertCondition((int)local_attractor_update_counter.size() == configuration::g_particles, "Number of particles inconsistent with local attractor updates.");
	AssertCondition((int)global_attractor_update_counter.size() == configuration::g_particles, "Number of particles inconsistent with global attractor updates.");
	
	for(int i = 0; i < configuration::g_particles; i++){
		(*inputstream) >> local_attractor_update_counter[i];
	}
	for(int i = 0; i < configuration::g_particles; i++){
		(*inputstream) >> global_attractor_update_counter[i];
	}
	for (int i = 0; i < configuration::g_particles; i++) {
		(*(swarm))[i]->LoadData(inputstream, version_of_stored_data);
	}
}
void Statistics::StoreData(std::ofstream* outputstream){
	(*outputstream) << current_iteration << std::endl;
	for(int i = 0; i < configuration::g_particles; i++){
		(*outputstream) << local_attractor_update_counter[i] << ((i + 1 == configuration::g_particles)?"\n":" ");
	}
	for(int i = 0; i < configuration::g_particles; i++){
		(*outputstream) << global_attractor_update_counter[i] << ((i + 1 == configuration::g_particles)?"\n":" ");
	}
	for (int i = 0; i < configuration::g_particles; i++) {
		(*(swarm))[i]->StoreData(outputstream);
	}
}

} // namespace highprecisionpso
