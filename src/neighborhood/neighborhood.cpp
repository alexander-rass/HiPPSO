/**
* @file   neighborhood/neighborhood.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2015
* @brief  This file contains an abstract classes for neighborhood generation.
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

#include "neighborhood/neighborhood.h"

#include "arbitrary_precision_calculation/operations.h"
#include "general/particle.h"

namespace highprecisionpso {

Neighborhood::~Neighborhood(){
	while(!remaining_updates_.empty()){
		std::pair< std::pair< std::vector<mpf_t*>, mpf_t*>, int> n = remaining_updates_.front();
		remaining_updates_.pop();
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(n.first.first);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(n.first.second);
	}
}

std::vector<mpf_t*> Neighborhood::GetGlobalAttractorPosition(Particle* p){
	return GetGlobalAttractorPosition(p->id);
}

mpf_t* Neighborhood::GetGlobalAttractorValue(Particle* p){
	return GetGlobalAttractorValue(p->id);
}

void Neighborhood::UpdateAttractor(std::vector<mpf_t*> position, mpf_t* value, int particleId){
	remaining_updates_.push(std::make_pair(std::make_pair(arbitraryprecisioncalculation::vectoroperations::Clone(position), arbitraryprecisioncalculation::mpftoperations::Clone(value)), particleId));
}

void Neighborhood::ProceedAllUpdates(){
	while(!remaining_updates_.empty()){
		std::pair< std::pair< std::vector<mpf_t*>, mpf_t*>, int> currentUpdate = remaining_updates_.front();
		std::vector<mpf_t*> position = currentUpdate.first.first;
		mpf_t* value = currentUpdate.first.second;
		int particleId = currentUpdate.second;
		UpdateAttractorInstantly(position, value, particleId);
		remaining_updates_.pop();
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(position);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(value);
	}
}

void Neighborhood::UpdateAttractor(std::vector<mpf_t*> position, mpf_t* value, Particle* p){
	UpdateAttractor(position, value, p->id);
}

void Neighborhood::UpdateAttractorInstantly(std::vector<mpf_t*> position, mpf_t* value, Particle* p){
	UpdateAttractorInstantly(position, value, p->id);
}

} // namespace highprecisionpso
