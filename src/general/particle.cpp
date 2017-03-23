/**
* @file   general/particle.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains information about the particles of the swarm.
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

#include "general/particle.h"

#include "function/function.h"
#include "general/check_condition.h"
#include "general/configuration.h"
#include "arbitrary_precision_calculation/operations.h"
#include "neighborhood/neighborhood.h"
#include "position_and_velocity_updater/position_and_velocity_updater.h"
#include "statistics/statistics.h"

namespace highprecisionpso {

int Particle::active_particles_ = 0;

Particle::Particle() {
	id = active_particles_++;
	
	local_attractor_position.clear();
	local_attractor_value_cached_ = NULL;
	local_attractor_value_cached_precision_ = 1;
}

std::vector<mpf_t*> Particle::GetLocalAttractorPosition() {
	return arbitraryprecisioncalculation::vectoroperations::Clone(local_attractor_position);
}

mpf_t* Particle::GetLocalAttractorValue() {
	if(0 == local_attractor_position.size())return NULL;
	if(local_attractor_value_cached_ == NULL 
			|| mpf_get_default_prec() != local_attractor_value_cached_precision_){
		if(local_attractor_value_cached_ == NULL)arbitraryprecisioncalculation::mpftoperations::ChangeNumberOfMpftValuesCached(1);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(local_attractor_value_cached_);
		local_attractor_value_cached_precision_ = mpf_get_default_prec();
		local_attractor_value_cached_ = configuration::g_function->Evaluate(local_attractor_position);
	}
	return arbitraryprecisioncalculation::mpftoperations::Clone(local_attractor_value_cached_);
}
std::vector<mpf_t*> Particle::GetPosition() {
	return arbitraryprecisioncalculation::vectoroperations::Clone(position);
}
std::vector<mpf_t*> Particle::GetVelocity() {
	return arbitraryprecisioncalculation::vectoroperations::Clone(velocity);
}

void Particle::SetLocalAttractorPosition(std::vector<mpf_t*> newLocalAttractorPosition) {
	configuration::g_statistics->local_attractor_update_counter[id]++;
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(local_attractor_position);
	local_attractor_position = arbitraryprecisioncalculation::vectoroperations::Clone(newLocalAttractorPosition);
	if(local_attractor_value_cached_ != NULL) {
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(local_attractor_value_cached_);
		arbitraryprecisioncalculation::mpftoperations::ChangeNumberOfMpftValuesCached(-1);
	}
	local_attractor_value_cached_ = NULL;
}

void Particle::SetPosition(std::vector<mpf_t*> newPosition) {
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(position);
	position = arbitraryprecisioncalculation::vectoroperations::Clone(newPosition);
	mpf_t* newVal = configuration::g_function->Evaluate(position);
	mpf_t* curLocalAttractorValue = GetLocalAttractorValue();
	if (curLocalAttractorValue == NULL || (arbitraryprecisioncalculation::mpftoperations::Compare(newVal, curLocalAttractorValue) <= 0) ) {
		SetLocalAttractorPosition(position);
		UpdateGlobalAttractor(position, newVal);
	}
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(curLocalAttractorValue);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(newVal);
}
void Particle::SetVelocity(std::vector<mpf_t*> newVelocity) {
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(velocity);
	velocity = arbitraryprecisioncalculation::vectoroperations::Clone(newVelocity);
}

void Particle::UpdateGlobalAttractor(std::vector<mpf_t*> goodPosition,
		mpf_t* goodValue) {
	configuration::g_neighborhood->UpdateAttractor(goodPosition, goodValue, this);
}

void Particle::UpdatePosition() {
	configuration::g_position_and_velocity_updater->Update(this);
}

void Particle::LoadData(std::ifstream* inputstream, ProgramVersion* version_of_stored_data){
	AssertCondition( position.size() == 0, "Load of particle failed.");
	AssertCondition( velocity.size() == 0, "Load of particle failed.");
	AssertCondition( local_attractor_position.size() == 0, "Load of particle failed.");
	for (int d = 0; d < configuration::g_dimensions; d++) {
		position.push_back(arbitraryprecisioncalculation::mpftoperations::LoadMpft(inputstream));
	}
	for (int d = 0; d < configuration::g_dimensions; d++) {
		velocity.push_back(arbitraryprecisioncalculation::mpftoperations::LoadMpft(inputstream));
	}
	for (int d = 0; d < configuration::g_dimensions; d++) {
		local_attractor_position.push_back(arbitraryprecisioncalculation::mpftoperations::LoadMpft(inputstream));
	}
}

void Particle::StoreData(std::ofstream* outputstream){
	for (int d = 0; d < configuration::g_dimensions; d++) {
		arbitraryprecisioncalculation::mpftoperations::StoreMpft(position[d], outputstream);
	}
	(*outputstream) << std::endl;
	for (int d = 0; d < configuration::g_dimensions; d++) {
		arbitraryprecisioncalculation::mpftoperations::StoreMpft(velocity[d], outputstream);
	}
	(*outputstream) << std::endl;
	for (int d = 0; d < configuration::g_dimensions; d++) {
		arbitraryprecisioncalculation::mpftoperations::StoreMpft(local_attractor_position[d], outputstream);
	}
	(*outputstream) << std::endl;
}

} // namespace highprecisionpso
