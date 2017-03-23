/**
* @file   testing_updater.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   August, 2015
* @brief  This file contains an experimental position and velocity updater.
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

#include "position_and_velocity_updater/testing_updater.h"

#include <gmp.h>
#include <sstream>
#include <vector>

#include "bound_handling/bound_handling.h"
#include "general/check_condition.h"
#include "general/configuration.h"
#include "arbitrary_precision_calculation/operations.h"
#include "general/particle.h"
#include "neighborhood/neighborhood.h"
#include "statistics/statistics.h"

namespace highprecisionpso {

void TestingUpdater::Update(Particle* p) {
	static std::vector<mpf_t*> last_nozero_velocity;
	while(last_nozero_velocity.size() < (unsigned int)configuration::g_dimensions){
		mpf_t* one = mpftoperations::ToMpft(1.0);
		mpf_t* tmp1 = mpftoperations::GetRandomMpft();
		mpf_t* tmp2 = mpftoperations::Multiply2Exp(tmp1, 1);
		mpftoperations::ReleaseValue(tmp1);
		mpftoperations::ReleaseValue(tmp2);
		mpftoperations::ReleaseValue(one);
		last_nozero_velocity.push_back(mpftoperations::Subtract(tmp2, one));// x = rand(0.0, 1.0) * 2.0 - 1.0
	}
	int current_dir = (configuration::g_statistics->current_iteration / 10) % configuration::g_dimensions;

	std::vector<mpf_t*> localDir = configuration::g_bound_handling->GetDirectionVector(
			p->position, p->local_attractor_position);
	std::vector<mpf_t*> helperVector = vectoroperations::Randomize(localDir);
	vectoroperations::ReleaseValues(localDir);
	localDir = helperVector;
	std::vector<mpf_t*> glAtPos = configuration::g_neighborhood->GetGlobalAttractorPosition(p);
	std::vector<mpf_t*> unchangedGlobalDir = configuration::g_bound_handling->GetDirectionVector(p->position, glAtPos);
	std::vector<mpf_t*> globalDir = vectoroperations::Clone(unchangedGlobalDir);
	helperVector = vectoroperations::Randomize(globalDir);
	vectoroperations::ReleaseValues(globalDir);
	globalDir = helperVector;
	std::vector<mpf_t*> localPart = vectoroperations::Multiply(localDir,
			configuration::g_coefficient_local_attractor);
	vectoroperations::ReleaseValues(localDir);
	std::vector<mpf_t*> globalPart = vectoroperations::Multiply(globalDir,
			configuration::g_coefficient_global_attractor);
	std::vector<mpf_t*> oldVelocityPart = vectoroperations::Multiply(p->velocity,
			configuration::g_chi);
	helperVector = vectoroperations::Add(localPart, globalPart);
	vectoroperations::ReleaseValues(localPart);
	vectoroperations::ReleaseValues(globalPart);
	std::vector<mpf_t*> newVelocity = vectoroperations::Add(oldVelocityPart,
			helperVector);
	vectoroperations::ReleaseValues(oldVelocityPart);
	vectoroperations::ReleaseValues(helperVector);
	for(int i = 0; i < configuration::g_dimensions; i++){
		if(i == current_dir)continue;
		mpftoperations::ReleaseValue(newVelocity[i]);
		newVelocity[i] = mpftoperations::Clone(unchangedGlobalDir[i]);
	}
	vectoroperations::ReleaseValues(globalDir);
	vectoroperations::ReleaseValues(unchangedGlobalDir);
	if(mpftoperations::Compare((newVelocity[current_dir]), 0.0) == 0){
		mpftoperations::ReleaseValue(newVelocity[current_dir]);
		newVelocity[current_dir] = mpftoperations::Randomize(last_nozero_velocity[current_dir]);
		AssertCondition(mpftoperations::Compare((newVelocity[current_dir]), 0.0) != 0, "");
	} else {
		mpftoperations::ReleaseValue (last_nozero_velocity[current_dir]);
		last_nozero_velocity[current_dir] = mpftoperations::Clone(newVelocity[current_dir]);
	}
	p->SetVelocity(newVelocity);
	vectoroperations::ReleaseValues(newVelocity);
	configuration::g_bound_handling->SetParticleUpdate(p);
	/*
	for(int i = 0; i < configuration::DIMENSIONS; i++){
		if(i == current_dir)continue;
		if(Operations::compare((glAtPos[i]), (p->position[i])) != 0){
			PrintHelper::print_mpf_t_Short(glAtPos[i]);
			cout << std::endl;
			PrintHelper::print_mpf_t_Short(p->position[i]);
			cout << std::endl;
			PrintHelper::print_mpf_t_Short(Operations::subtract(glAtPos[i], p->position[i]));
			cout << std::endl;
		}
//		check_condition(Operations::compare((glAtPos[i]), (p->position[i])) == 0);
	}
	cout << "pos ";
	PrintHelper::printVectorShort(p->position);
	cout << "\nvel ";
	PrintHelper::printVectorShort(p->velocity);
	cout << std::endl;
	*/
	vectoroperations::ReleaseValues(glAtPos);
}

std::string TestingUpdater::GetName(){
	return "Testing";
}

} // namespace highprecisionpso
