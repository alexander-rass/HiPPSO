/**
* @file   neighborhood/global_best.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2015
* @brief  This file contains the global best neighborhood topology.
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

#include "neighborhood/global_best.h"

#include "function/function.h"
#include "general/check_condition.h"
#include "general/configuration.h"
#include "arbitrary_precision_calculation/operations.h"
#include "statistics/statistics.h"

namespace highprecisionpso {

GlobalBest::~GlobalBest(){
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(global_attractor_position_);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(global_attractor_value_cached_);
}

GlobalBest::GlobalBest(){
	global_attractor_position_ = std::vector<mpf_t*>(0);
	global_attractor_value_cached_ = NULL;
	global_attractor_value_cached_precision_ = 1;
}

std::vector<mpf_t*> GlobalBest::GetGlobalAttractorPosition(){
	return arbitraryprecisioncalculation::vectoroperations::Clone(global_attractor_position_);
}

std::vector<mpf_t*> GlobalBest::GetGlobalAttractorPosition(int particleId){
	AssertCondition(particleId >= 0, "adjacency list neighborhood: Queried particle id is less than zero.");
	AssertCondition(configuration::g_particles > particleId, "adjacency list neighborhood: Queried particle id is larger than the largest id.");
	return arbitraryprecisioncalculation::vectoroperations::Clone(global_attractor_position_);
}

mpf_t* GlobalBest::GetGlobalAttractorValue(int particleId){
	return GetGlobalAttractorValue();
}

mpf_t* GlobalBest::GetGlobalAttractorValue(){
	if(global_attractor_value_cached_ == NULL
			|| mpf_get_default_prec() != global_attractor_value_cached_precision_){
		if(global_attractor_value_cached_ == NULL) arbitraryprecisioncalculation::mpftoperations::ChangeNumberOfMpftValuesCached(1);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(global_attractor_value_cached_);
		global_attractor_value_cached_ = configuration::g_function->Evaluate(global_attractor_position_);
		global_attractor_value_cached_precision_ = mpf_get_default_prec();
	}
	return arbitraryprecisioncalculation::mpftoperations::Clone(global_attractor_value_cached_);
}

void GlobalBest::UpdateAttractorInstantly(std::vector<mpf_t*> position, mpf_t* value, int particleId){
	if(global_attractor_position_.size() == 0){
		configuration::g_statistics->global_attractor_update_counter[particleId]++;
		global_attractor_position_ = arbitraryprecisioncalculation::vectoroperations::Clone(position);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(global_attractor_value_cached_);
		if(global_attractor_value_cached_ == NULL) arbitraryprecisioncalculation::mpftoperations::ChangeNumberOfMpftValuesCached(1);
		global_attractor_value_cached_ = configuration::g_function->Evaluate(global_attractor_position_);
		global_attractor_value_cached_precision_ = mpf_get_default_prec();
	} else {
		AssertCondition(global_attractor_position_.size() == position.size(), "The number of dimensions is inconsistent.");
		mpf_t* curvalue = GetGlobalAttractorValue(particleId);
		if(arbitraryprecisioncalculation::mpftoperations::Compare(value, curvalue) <= 0) {
			configuration::g_statistics->global_attractor_update_counter[particleId]++;
			arbitraryprecisioncalculation::vectoroperations::ReleaseValues(global_attractor_position_);
			global_attractor_position_ = arbitraryprecisioncalculation::vectoroperations::Clone(position);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(global_attractor_value_cached_);
			global_attractor_value_cached_ = configuration::g_function->Evaluate(global_attractor_position_);
			global_attractor_value_cached_precision_ = mpf_get_default_prec();
		}
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(curvalue);
	}
}

std::string GlobalBest::GetName(){
	return "gBest";
}
void GlobalBest::LoadData(std::ifstream* inputstream, ProgramVersion* version_of_stored_data){
	for(int d = 0; d < configuration::g_dimensions; d++){
		global_attractor_position_.push_back(arbitraryprecisioncalculation::mpftoperations::LoadMpft(inputstream));
	}
	if((*version_of_stored_data)>=ProgramVersion("1.0.2")){
		global_attractor_value_cached_ = arbitraryprecisioncalculation::mpftoperations::LoadMpft(inputstream);
		(*inputstream) >> global_attractor_value_cached_precision_;
	}
}
void GlobalBest::StoreData(std::ofstream* outputstream){
	for(int d = 0; d < configuration::g_dimensions; d++){
		arbitraryprecisioncalculation::mpftoperations::StoreMpft(global_attractor_position_[d], outputstream);
	}
	arbitraryprecisioncalculation::mpftoperations::StoreMpft(global_attractor_value_cached_, outputstream);
	(*outputstream) << "\n" << global_attractor_value_cached_precision_ << "\n";
}

} // namespace highprecisionpso
