/**
* @file   position_and_velocity_updater/delta_updater.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2014
* @brief  This file contains a class for processing the velocity and position update of each particle with random reinitialization of velocity in case of low energy.
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

#include "position_and_velocity_updater/delta_updater.h"

#include <gmp.h>
#include <sstream>

#include "bound_handling/bound_handling.h"
#include "general/check_condition.h"
#include "general/configuration.h"
#include "arbitrary_precision_calculation/operations.h"
#include "general/particle.h"
#include "neighborhood/neighborhood.h"

namespace highprecisionpso {

DeltaUpdater::DeltaUpdater(double delta):initial_delta_(delta), gamma_(1.0){
	delta_ = arbitraryprecisioncalculation::mpftoperations::ToMpft(initial_delta_);
}

DeltaUpdater::DeltaUpdater(double initial_delta, double gamma):initial_delta_(initial_delta), gamma_(gamma){
	delta_ = arbitraryprecisioncalculation::mpftoperations::ToMpft(initial_delta_);
}

void DeltaUpdater::Update(Particle* p) {
	bool deltaUpdate = true;
	std::vector<mpf_t*> glAtPos = configuration::g_neighborhood->GetGlobalAttractorPosition(p);
	std::vector<mpf_t*> globalDirTmp = configuration::g_bound_handling->GetDirectionVector(p->position, glAtPos);
	for(int d = 0; d < configuration::g_dimensions; ++d){
		mpf_t* v1 = arbitraryprecisioncalculation::mpftoperations::Abs(globalDirTmp[d]);
		mpf_t* v2 = arbitraryprecisioncalculation::mpftoperations::Abs(p->velocity[d]);
		mpf_t* sum = arbitraryprecisioncalculation::mpftoperations::Add(v1, v2);
		deltaUpdate = (arbitraryprecisioncalculation::mpftoperations::Compare(sum, delta_) < 0);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v1);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v2);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(sum);
		if(!deltaUpdate)break;
	}
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(globalDirTmp);
	if(deltaUpdate){
		std::vector<mpf_t*> part = arbitraryprecisioncalculation::vectoroperations::GetConstantVector(configuration::g_dimensions, delta_);
		std::vector<mpf_t*> part1M = arbitraryprecisioncalculation::vectoroperations::Multiply(part, 2.0);
		std::vector<mpf_t*> part1Rand = arbitraryprecisioncalculation::vectoroperations::Randomize(part1M);
		std::vector<mpf_t*> newVelocity = arbitraryprecisioncalculation::vectoroperations::Subtract(part1Rand, part);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(part);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(part1M);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(part1Rand);
		mpf_t* newDelta = arbitraryprecisioncalculation::mpftoperations::Multiply(delta_, gamma_);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(delta_);
		delta_ = newDelta;
		p->SetVelocity(newVelocity);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newVelocity);
	} else {
		std::vector<mpf_t*> localDir = configuration::g_bound_handling->GetDirectionVector(
				p->position, p->local_attractor_position);
		std::vector<mpf_t*> helperVector = arbitraryprecisioncalculation::vectoroperations::Randomize(localDir);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(localDir);
		localDir = helperVector;
		std::vector<mpf_t*> globalDir = configuration::g_bound_handling->GetDirectionVector(p->position, glAtPos);
		helperVector = arbitraryprecisioncalculation::vectoroperations::Randomize(globalDir);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(globalDir);
		globalDir = helperVector;
		std::vector<mpf_t*> localPart = arbitraryprecisioncalculation::vectoroperations::Multiply(localDir,
				configuration::g_coefficient_local_attractor);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(localDir);
		std::vector<mpf_t*> globalPart = arbitraryprecisioncalculation::vectoroperations::Multiply(globalDir,
				configuration::g_coefficient_global_attractor);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(globalDir);
		std::vector<mpf_t*> oldVelocityPart = arbitraryprecisioncalculation::vectoroperations::Multiply(p->velocity,
				configuration::g_chi);
		helperVector = arbitraryprecisioncalculation::vectoroperations::Add(localPart, globalPart);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(localPart);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(globalPart);
		std::vector<mpf_t*> newVelocity = arbitraryprecisioncalculation::vectoroperations::Add(oldVelocityPart,
				helperVector);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(oldVelocityPart);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(helperVector);
		p->SetVelocity(newVelocity);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newVelocity);
	}
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(glAtPos);
	configuration::g_bound_handling->SetParticleUpdate(p);
}

std::string DeltaUpdater::GetName(){
	std::ostringstream os;
	os << "Delta" << initial_delta_;
	if(gamma_ != 1.0) os << "g" << gamma_;
	std::string res = os.str();
	return res;
}

void DeltaUpdater::LoadData(std::ifstream* inputstream, ProgramVersion* version_of_stored_data){
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(delta_);
	delta_ = arbitraryprecisioncalculation::mpftoperations::LoadMpft(inputstream);
}

void DeltaUpdater::StoreData(std::ofstream* outputstream) {
	arbitraryprecisioncalculation::mpftoperations::StoreMpft(delta_, outputstream);
}

} // namespace highprecisionpso
