/**
* @file   adjacency_list_neighborhood.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   March, 2015
* @brief  This file contains neighborhood topologies specified by adjacency lists.
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

#include "neighborhood/adjacency_list_neighborhood.h"

#include <sstream>

#include "function/function.h"
#include "general/check_condition.h"
#include "general/configuration.h"
#include "general/operations.h"
#include "statistics/statistics.h"

namespace highprecisionpso {

AdjacencyListNeighborhood::~AdjacencyListNeighborhood(){
	for(unsigned int i = 0; i < global_attractor_positions_.size(); i++){
		vectoroperations::ReleaseValues(global_attractor_positions_[i]);
		global_attractor_positions_[i].clear();
	}
	vectoroperations::ReleaseValues(global_attractor_values_cached_);
	global_attractor_values_cached_.clear();
}


AdjacencyListNeighborhood::AdjacencyListNeighborhood(){
	global_attractor_index_ = -1;
	global_attractor_positions_.clear();
	global_attractor_values_cached_.clear();
	global_attractor_values_cached_precision_.clear();
	adjacency_list.clear();
}

void AdjacencyListNeighborhood::SetGlobalAttractorPositions(std::vector<std::vector<mpf_t*> > globalAttractors){
	AssertCondition(configuration::g_particles > 0, "The number of particles needs to be positive.");
	AssertCondition(globalAttractors.size() == (unsigned int)configuration::g_particles, "The number of particles is inconsistent.");
	AssertCondition(configuration::g_dimensions > 0, "The number of dimensions needs to be positive.");
	AssertCondition(globalAttractors[0].size() == (unsigned int)configuration::g_dimensions, "The number of dimensions is inconsistent.");
	for(int p = 0; p < configuration::g_particles; p++){
		AssertCondition(globalAttractors[p].size() == (unsigned int)configuration::g_dimensions, "The number of dimensions is inconsistent.");
	}
	for(unsigned int i = 0; i < global_attractor_positions_.size(); i++){
		vectoroperations::ReleaseValues(global_attractor_positions_[i]);
		global_attractor_positions_[i].clear();
	}
	global_attractor_positions_ = std::vector<std::vector<mpf_t*> >(configuration::g_particles);
	for(int i = 0; i < configuration::g_particles; i++) {
		global_attractor_positions_[i] = vectoroperations::Clone(globalAttractors[i]);
	}
	for(unsigned int i = 0; i < global_attractor_values_cached_.size(); i++){
		if(global_attractor_values_cached_[i] != NULL) {
			mpftoperations::ChangeNumberOfMpftValuesCached(-1);
			mpftoperations::ReleaseValue(global_attractor_values_cached_[i]);
			global_attractor_values_cached_[i] = NULL;
		}
	}
	global_attractor_values_cached_ = std::vector<mpf_t*>(configuration::g_particles, (mpf_t*) NULL);
	global_attractor_values_cached_precision_ = std::vector<unsigned int>(configuration::g_particles, 0);
	global_attractor_index_ = -1;
}

std::vector<mpf_t*> AdjacencyListNeighborhood::GetGlobalAttractorPosition(){
	int id = GetGlobalAttractorIndex();
	return vectoroperations::Clone(global_attractor_positions_[id]);
}

std::vector<mpf_t*> AdjacencyListNeighborhood::GetGlobalAttractorPosition(int particleId){
	AssertCondition(particleId >= 0, "adjacency list neighborhood: Queried particle id is less than zero.");
	AssertCondition(global_attractor_positions_.size() > (unsigned int)particleId, "adjacency list neighborhood: Queried particle id is larger than the largest id.");
	return vectoroperations::Clone(global_attractor_positions_[particleId]);
}

mpf_t* AdjacencyListNeighborhood::GetGlobalAttractorValue(int particleId){
	AssertCondition(particleId >= 0, "adjacency list neighborhood: Queried particle id is less than zero.");
	AssertCondition(global_attractor_positions_.size() > (unsigned int)particleId, "adjacency list neighborhood: Queried particle id is larger than the largest id.");
	if(global_attractor_values_cached_precision_[particleId] != mpf_get_default_prec()
			|| global_attractor_values_cached_[particleId] == NULL){

		global_attractor_values_cached_precision_[particleId] = mpf_get_default_prec();
		global_attractor_index_ = -1;
		if(global_attractor_values_cached_[particleId] != NULL){
			mpftoperations::ReleaseValue(global_attractor_values_cached_[particleId]);
			mpftoperations::ChangeNumberOfMpftValuesCached(-1);
		}
		global_attractor_values_cached_[particleId] = configuration::g_function->Evaluate(global_attractor_positions_[particleId]);
		mpftoperations::ChangeNumberOfMpftValuesCached(1);
	}
	return mpftoperations::Clone(global_attractor_values_cached_[particleId]);
}

mpf_t* AdjacencyListNeighborhood::GetGlobalAttractorValue(){
	int id = GetGlobalAttractorIndex();
	return GetGlobalAttractorValue(id);
}

void AdjacencyListNeighborhood::UpdateAttractorInstantly(std::vector<mpf_t*> position, mpf_t* value, int particleId){
	if(adjacency_list.size() == 0){
		InitializeAdjacencyList();
	}
	for(unsigned int iInd = 0; iInd < adjacency_list[particleId].size(); iInd++){
		unsigned int id = adjacency_list[particleId][iInd];
		if(global_attractor_positions_.size() <= id) {
			while(global_attractor_positions_.size() <= id){
				global_attractor_positions_.push_back(std::vector<mpf_t*>());
				global_attractor_values_cached_.push_back(NULL);
				global_attractor_values_cached_precision_.push_back(0);
			}
		}
		if(global_attractor_positions_[id].size() == 0){
			global_attractor_index_ = -1;
			configuration::g_statistics->global_attractor_update_counter[particleId]++;
			global_attractor_positions_[id] = vectoroperations::Clone(position);
			if(global_attractor_values_cached_[id] == NULL) mpftoperations::ChangeNumberOfMpftValuesCached(1);
			mpftoperations::ReleaseValue(global_attractor_values_cached_[id]);
			global_attractor_values_cached_[id] = configuration::g_function->Evaluate(global_attractor_positions_[id]);
			global_attractor_values_cached_precision_[id] = mpf_get_default_prec();
		} else {
			AssertCondition(global_attractor_positions_[id].size() == position.size(), "adjacency list neighborhood: The number of dimensions is inconsistent.");
			mpf_t* curvalue = GetGlobalAttractorValue(id);
			if(mpftoperations::Compare(value, curvalue) <= 0) {
				global_attractor_index_ = -1;
				configuration::g_statistics->global_attractor_update_counter[particleId]++;
				vectoroperations::ReleaseValues(global_attractor_positions_[id]);
				global_attractor_positions_[id] = vectoroperations::Clone(position);
				mpftoperations::ReleaseValue(global_attractor_values_cached_[id]);
				global_attractor_values_cached_[id] = configuration::g_function->Evaluate(global_attractor_positions_[id]);
				global_attractor_values_cached_precision_[id] = mpf_get_default_prec();
			}
			mpftoperations::ReleaseValue(curvalue);
		}
	}
}

int AdjacencyListNeighborhood::GetGlobalAttractorIndex(){
	if(global_attractor_index_ == -1) {
		AssertCondition(global_attractor_positions_.size() == (unsigned int) configuration::g_particles, "adjacency list neighborhood: The number of particles is inconsistent.");
		int id = 0;
		mpf_t* best = GetGlobalAttractorValue(0);
		for(int i = 1; i < configuration::g_particles; i++) {
			mpf_t* cur = GetGlobalAttractorValue(i);
			if(mpftoperations::Compare(best, cur) >= 0) {
				std::swap(best, cur);
				id = i;
			}
			mpftoperations::ReleaseValue(cur);
		}
		mpftoperations::ReleaseValue(best);
		global_attractor_index_ = id;
	}
	return global_attractor_index_;
}

void AdjacencyListNeighborhood::LoadData(std::ifstream* inputstream, ProgramVersion* version_of_stored_data){
	global_attractor_positions_ = std::vector<std::vector<mpf_t*> >(configuration::g_particles);
	for(int p = 0; p < configuration::g_particles; p++){
		global_attractor_positions_.push_back(std::vector<mpf_t*>());
		for(int d = 0; d < configuration::g_dimensions; d++){
			global_attractor_positions_[p].push_back(mpftoperations::LoadMpft(inputstream, version_of_stored_data));
		}
	}
}
void AdjacencyListNeighborhood::StoreData(std::ofstream* outputstream){
	for(int p = 0; p < configuration::g_particles; p++){
		for(int d = 0; d < configuration::g_dimensions; d++){
			mpftoperations::StoreMpft(global_attractor_positions_[p][d], outputstream);
		}
		(*outputstream) << std::endl;
	}
}

LBestNeighborhood::LBestNeighborhood(int k):AdjacencyListNeighborhood(),num_neighbors_(k){}

void LBestNeighborhood::InitializeAdjacencyList(){
	AssertCondition(num_neighbors_ % 2 == 0, "The number of neighbors for lbest topology must be an even number!");
	int K = num_neighbors_ / 2;
	if(K > configuration::g_particles) K = configuration::g_particles;
	adjacency_list = std::vector<std::vector<unsigned int> >(configuration::g_particles);
	for(int i = 0; i < configuration::g_particles; i++) {
		adjacency_list[i].push_back(i);
		for(int k = 1; k <= K; k++){
			adjacency_list[i].push_back((i + k) % configuration::g_particles);
			adjacency_list[i].push_back((i + configuration::g_particles - k) % configuration::g_particles);
		}
	}
}

std::string LBestNeighborhood::GetName(){
	std::ostringstream os;
	os << "Lbest" << num_neighbors_;
	return os.str();
}

RingNeighborhood::RingNeighborhood():LBestNeighborhood(2){}

std::string RingNeighborhood::GetName(){
	std::ostringstream os;
	os << "Ring";
	return os.str();
}

GridNeighborhood::GridNeighborhood(int r, int c) : AdjacencyListNeighborhood(), grid_rows_ (r), grid_columns_ (c) {
}

void GridNeighborhood::InitializeAdjacencyList(){
	AssertCondition(grid_rows_ * grid_columns_ == configuration::g_particles, "For the grid topology the number of rows multiplied by the number of columns must be equal to the number of particles.");
	adjacency_list = std::vector<std::vector<unsigned int> >(configuration::g_particles);
	for(int i = 0; i < configuration::g_particles; i++) {
		adjacency_list[i].push_back(i);
		int r = i / grid_columns_;
		int c = i - grid_columns_ * r;
		adjacency_list[i].push_back(r * grid_columns_ + ((c + 1) % grid_columns_));
		adjacency_list[i].push_back(r * grid_columns_ + ((c + grid_columns_ - 1) % grid_columns_));
		adjacency_list[i].push_back(((r + 1) % grid_rows_) * grid_columns_ + c);
		adjacency_list[i].push_back(((r + grid_rows_ - 1) % grid_rows_) * grid_columns_ + c);
	}
}

std::string GridNeighborhood::GetName(){
	std::ostringstream os;
	os << "Gr" << grid_rows_ << "." << grid_columns_;
	return os.str();
}

WheelNeighborhood::WheelNeighborhood(){
}

void WheelNeighborhood::InitializeAdjacencyList(){
	adjacency_list = std::vector<std::vector<unsigned int> >(configuration::g_particles);
	for(int i = 0; i < configuration::g_particles; i++) {
		adjacency_list[i].push_back(i);
		if(i != 0){
			adjacency_list[i].push_back(0);
			adjacency_list[0].push_back(i);
		}
	}
}

std::string WheelNeighborhood::GetName(){
	std::ostringstream os;
	os << "Wheel";
	return os.str();
}

} // namespace highprecisionpso
