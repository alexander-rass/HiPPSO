/**
* @file   bound_handling/random_forth.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   March, 2015
* @brief  This file contains the random forth bound handling strategy.
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

#include "bound_handling/random_forth.h"

#include "function/function.h"
#include "general/check_condition.h"
#include "general/configuration.h"
#include "arbitrary_precision_calculation/operations.h"
#include "general/particle.h"
#include "velocity_adjustment/velocity_adjustment.h"

namespace highprecisionpso {

BoundHandlingRandomForth::BoundHandlingRandomForth(RandomForthMarkDimensions random_forth_mark_dimensions):random_forth_mark_dimensions_(random_forth_mark_dimensions){}

void BoundHandlingRandomForth::SetParticleUpdate(Particle * p){
	std::vector<mpf_t*> oldPos = p->GetPosition();
	std::vector<mpf_t*> vel = p->GetVelocity();
	std::vector<mpf_t*> newPos = arbitraryprecisioncalculation::vectoroperations::Add(oldPos, vel);
	std::vector<bool> modifiedDimensions(newPos.size(), false);
	mpf_t* scale = arbitraryprecisioncalculation::mpftoperations::ToMpft(1.0);
	bool outside = false;
	std::vector<mpf_t*> lo = configuration::g_function->GetLowerSearchSpaceBound();
	std::vector<mpf_t*> hi = configuration::g_function->GetUpperSearchSpaceBound();
	int minScaleId = -1;
	for(unsigned int i = 0; i < newPos.size(); i++) {
		if(arbitraryprecisioncalculation::mpftoperations::Compare(lo[i], newPos[i]) > 0) {
			if(random_forth_mark_dimensions_ == RANDOM_FORTH_MARK_DIMENSIONS_OUTSIDE_BOUNDS){
				modifiedDimensions[i] = true;
			}
			outside = true;
			mpf_t* maxLen = arbitraryprecisioncalculation::mpftoperations::Subtract(lo[i], oldPos[i]);
			mpf_t* newScale = arbitraryprecisioncalculation::mpftoperations::Divide(maxLen, vel[i]);
			if(arbitraryprecisioncalculation::mpftoperations::Compare(newScale, scale) < 0) {
				minScaleId = i;
				std::swap(newScale, scale);
			}
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(newScale);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(maxLen);
		} else if(arbitraryprecisioncalculation::mpftoperations::Compare(hi[i], newPos[i]) < 0) {
			if(random_forth_mark_dimensions_ == RANDOM_FORTH_MARK_DIMENSIONS_OUTSIDE_BOUNDS){
				modifiedDimensions[i] = true;
			}
			outside = true;
			mpf_t* maxLen = arbitraryprecisioncalculation::mpftoperations::Subtract(hi[i], oldPos[i]);
			mpf_t* newScale = arbitraryprecisioncalculation::mpftoperations::Divide(maxLen, vel[i]);
			if(arbitraryprecisioncalculation::mpftoperations::Compare(newScale, scale) < 0) {
				minScaleId = i;
				std::swap(newScale, scale);
			}
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(newScale);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(maxLen);
		}
	}
	if(outside){
		if(random_forth_mark_dimensions_ == RANDOM_FORTH_MARK_DIMENSIONS_ALL_ON_CHANGE){
			modifiedDimensions = std::vector<bool>(newPos.size(), true);
		}
		AssertCondition(arbitraryprecisioncalculation::mpftoperations::Compare(scale, 0.0) >= 0, "");
		AssertCondition(arbitraryprecisioncalculation::mpftoperations::Compare(scale, 1.0) <= 0, "");
		{// difference to absorb: random scale
			mpf_t* random_scale = arbitraryprecisioncalculation::mpftoperations::Randomize(scale);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(scale);
			scale = random_scale;
		}
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newPos);
		std::vector<mpf_t*> cVel = arbitraryprecisioncalculation::vectoroperations::Multiply(vel, scale);
		newPos = arbitraryprecisioncalculation::vectoroperations::Add(oldPos, cVel);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(cVel);
		modifiedDimensions[minScaleId] = true;
		for(unsigned int i = 0; i < newPos.size(); i++) {
			if(arbitraryprecisioncalculation::mpftoperations::Compare(lo[i], newPos[i]) > 0) {
				modifiedDimensions[i] = true;
				arbitraryprecisioncalculation::mpftoperations::ReleaseValue(newPos[i]);
				newPos[i] = arbitraryprecisioncalculation::mpftoperations::Clone(lo[i]);
			} else if(arbitraryprecisioncalculation::mpftoperations::Compare(hi[i], newPos[i]) < 0) {
				modifiedDimensions[i] = true;
				arbitraryprecisioncalculation::mpftoperations::ReleaseValue(newPos[i]);
				newPos[i] = arbitraryprecisioncalculation::mpftoperations::Clone(hi[i]);
			}
		}
	}
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(scale);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(lo);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(hi);
	p->SetPosition(newPos);
	configuration::g_velocity_adjustment->AdjustVelocity(p, modifiedDimensions, oldPos);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(oldPos);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(vel);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newPos);
}

std::string BoundHandlingRandomForth::GetName(){
	return "RanForth";
}

} // namespace highprecisionpso
