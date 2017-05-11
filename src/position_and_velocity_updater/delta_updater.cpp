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
    std::vector<bool> deltaUpdate = DimensionsOfDeltaUpdate(p);
	std::vector<mpf_t*> glAtPos = configuration::g_neighborhood->GetGlobalAttractorPosition(p);

    // calculate new velocity in case of delta update
    std::vector<mpf_t*> part = arbitraryprecisioncalculation::vectoroperations::GetConstantVector(configuration::g_dimensions, delta_);
    std::vector<mpf_t*> part1M = arbitraryprecisioncalculation::vectoroperations::Multiply(part, 2.0);
    std::vector<mpf_t*> part1Rand = arbitraryprecisioncalculation::vectoroperations::Randomize(part1M);
    std::vector<mpf_t*> newVelocity1 = arbitraryprecisioncalculation::vectoroperations::Subtract(part1Rand, part); // new velocity for delta update dimensions
    arbitraryprecisioncalculation::vectoroperations::ReleaseValues(part);
    arbitraryprecisioncalculation::vectoroperations::ReleaseValues(part1M);
    arbitraryprecisioncalculation::vectoroperations::ReleaseValues(part1Rand);

    // calculate new velocity in case of standard update
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
            helperVector); // new velocity for NON delta update dimensions
    arbitraryprecisioncalculation::vectoroperations::ReleaseValues(oldVelocityPart);
    arbitraryprecisioncalculation::vectoroperations::ReleaseValues(helperVector);

    // choose correct velocity
    bool first_delta_update = true;
    for (int d = 0; d < configuration::g_dimensions; d++) {
        if(deltaUpdate[d]){
            if(first_delta_update){
                CheckInitDeltaUpdateCounters_();
                mpf_t* newDelta = arbitraryprecisioncalculation::mpftoperations::Multiply(delta_, gamma_);
                arbitraryprecisioncalculation::mpftoperations::ReleaseValue(delta_);
                delta_ = newDelta;
                first_delta_update = false;
            }
            delta_update_counters_[p->id][d]++;
            std::swap(newVelocity[d], newVelocity1[d]);
        }
    }
    p->SetVelocity(newVelocity);
    arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newVelocity1);
    arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newVelocity);
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

std::vector<bool> DeltaUpdater::DimensionsOfDeltaUpdate(Particle* p){
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
    arbitraryprecisioncalculation::vectoroperations::ReleaseValues(glAtPos);
    return std::vector<bool>(configuration::g_dimensions, deltaUpdate);
}

void DeltaUpdater::CheckInitDeltaUpdateCounters_(){
    if(((int)delta_update_counters_.size()) != configuration::g_particles
         || ((int)delta_update_counters_[0].size()) != configuration::g_dimensions){
        delta_update_counters_ = std::vector<std::vector<unsigned long long> >(configuration::g_particles, std::vector<unsigned long long>(configuration::g_dimensions));
    }
}

std::vector<std::vector<unsigned long long> > DeltaUpdater::NumberOfDeltaUpdates(){
    CheckInitDeltaUpdateCounters_();
    return delta_update_counters_;
}

void DeltaUpdater::LoadData(std::ifstream* inputstream, ProgramVersion* version_of_stored_data){
    ProgramVersion v1_0_1("1.0.1");
    if((*version_of_stored_data) >= v1_0_1){
        int s1, s2;
        (*inputstream) >> s1 >> s2;
        delta_update_counters_ = std::vector<std::vector<unsigned long long> >(s1, std::vector<unsigned long long>(s2));
        for(int i1 = 0; i1 < s1; i1++){
            for(int i2 = 0; i2 < s2; i2++){
                (*inputstream) >> delta_update_counters_[i1][i2];
            }
        }
    }
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(delta_);
	delta_ = arbitraryprecisioncalculation::mpftoperations::LoadMpft(inputstream);
}

void DeltaUpdater::StoreData(std::ofstream* outputstream) {
    if(delta_update_counters_.size() == 0){
        (*outputstream) << " 0 0\n";
    } else {
        (*outputstream) << " " << delta_update_counters_.size() << " " << delta_update_counters_[0].size() << std::endl;
        for(auto it1: delta_update_counters_){
            for(auto it2: it1){
                (*outputstream) << it2 << " ";
            }
            (*outputstream) << std::endl;
        }
    }
	arbitraryprecisioncalculation::mpftoperations::StoreMpft(delta_, outputstream);
}






DeltaUpdaterTCS::DeltaUpdaterTCS(double delta):DeltaUpdater(delta){}

DeltaUpdaterTCS::DeltaUpdaterTCS(double initial_delta, double gamma):DeltaUpdater(initial_delta, gamma){}

std::vector<bool> DeltaUpdaterTCS::DimensionsOfDeltaUpdate(Particle* p){
	std::vector<bool> deltaUpdate(configuration::g_dimensions, true);
   	std::vector<mpf_t*> glAtPos = configuration::g_neighborhood->GetGlobalAttractorPosition(p);
    for(auto pp: *(configuration::g_statistics->swarm)){
        std::vector<mpf_t*> globalDirTmp = configuration::g_bound_handling->GetDirectionVector(pp->position, glAtPos);
        for(int d = 0; d < configuration::g_dimensions; ++d){
            mpf_t* v1 = arbitraryprecisioncalculation::mpftoperations::Abs(globalDirTmp[d]);
            mpf_t* v2 = arbitraryprecisioncalculation::mpftoperations::Abs(pp->velocity[d]);
            mpf_t* sum = arbitraryprecisioncalculation::mpftoperations::Add(v1, v2);
            if(! (arbitraryprecisioncalculation::mpftoperations::Compare(sum, delta_) < 0) ){
                deltaUpdate[d] = false;
            }
            arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v1);
            arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v2);
            arbitraryprecisioncalculation::mpftoperations::ReleaseValue(sum);
        }
       	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(globalDirTmp);
    }
    arbitraryprecisioncalculation::vectoroperations::ReleaseValues(glAtPos);
    return deltaUpdate;
}

std::string DeltaUpdaterTCS::GetName(){
	std::ostringstream os;
	os << "DeltaTCS" << initial_delta_;
	if(gamma_ != 1.0) os << "g" << gamma_;
	std::string res = os.str();
	return res;
}



} // namespace highprecisionpso
