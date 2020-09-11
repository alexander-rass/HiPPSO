/**
* @file   position_and_velocity_updater/search_radius_updater.cpp
* @author Alexander Dietsch (alexander.dietsch@fau.de)
* @date   August, 2020
* @brief This file contains a class for processing the velocity and position update of each particle with random offset in position that scales with particle velocity. 
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

#include "position_and_velocity_updater/search_radius_updater.h"

#include <gmp.h>
#include <vector>
#include <iostream>
#include "bound_handling/bound_handling.h"
#include "general/configuration.h"
#include "arbitrary_precision_calculation/configuration.h"
#include "arbitrary_precision_calculation/operations.h"
#include "general/particle.h"
#include "neighborhood/neighborhood.h"


namespace highprecisionpso {

SearchRadiusUpdaterCube::SearchRadiusUpdaterCube(double coefficient_radius, long long sample_points) : 
		coefficient_radius_(coefficient_radius) {
	
	if(sample_points < 0)
		sample_points_ = 0;
	else
		sample_points_ = sample_points;
}


void SearchRadiusUpdaterCube::Update(Particle* p) {
	std::vector<mpf_t*> localDir = configuration::g_bound_handling->GetDirectionVector(
			p->position, p->local_attractor_position);
	std::vector<mpf_t*> helperVector = arbitraryprecisioncalculation::vectoroperations::Randomize(localDir);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(localDir);
	localDir = helperVector;
	std::vector<mpf_t*> glAtPos = configuration::g_neighborhood->GetGlobalAttractorPosition(p);
	std::vector<mpf_t*> globalDir = configuration::g_bound_handling->GetDirectionVector(p->position, glAtPos);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(glAtPos);
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

	mpf_t *bestVal = NULL;
	if(sample_points_ > 0) {
		LazyParticle lzp(p);
		lzp.SetVelocity(newVelocity);
		configuration::g_bound_handling->SetParticleUpdate(&lzp);
		std::vector<mpf_t*> newPos = lzp.GetPosition();
		bestVal = configuration::g_function->Evaluate(newPos);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newPos);
	}

	mpf_t *sqVelocityLength = arbitraryprecisioncalculation::vectoroperations::SquaredEuclideanLength(newVelocity);
	mpf_t *velocityLength = arbitraryprecisioncalculation::mpftoperations::Sqrt(sqVelocityLength);
	mpf_t *searchRadius = arbitraryprecisioncalculation::mpftoperations::Multiply(velocityLength, coefficient_radius_);
	mpf_t *minusSearchRadius = arbitraryprecisioncalculation::mpftoperations::Multiply(searchRadius, -1);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(sqVelocityLength);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(velocityLength);

	arbitraryprecisioncalculation::RandomNumberGenerator *rng = arbitraryprecisioncalculation::Configuration::getStandardRandomNumberGenerator();

	std::vector<mpf_t *> bestVelocity = arbitraryprecisioncalculation::vectoroperations::Clone(newVelocity);
	long long samplesToGo = sample_points_;
	do {
		LazyParticle lzp(p);
		std::vector<mpf_t*> tempVelocity(newVelocity.size(), NULL);
		for(unsigned int i = 0; i < newVelocity.size(); ++i) {
			mpf_t *rndVal;
			if(rng->RandomLongLong() % 2 == 0) {
				rndVal = arbitraryprecisioncalculation::mpftoperations::Randomize(searchRadius);
			} else {
				rndVal = arbitraryprecisioncalculation::mpftoperations::Randomize(minusSearchRadius);
			}
			tempVelocity[i] = arbitraryprecisioncalculation::mpftoperations::Add(newVelocity[i], rndVal);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(rndVal);
		}
		lzp.SetVelocity(tempVelocity);
		configuration::g_bound_handling->SetParticleUpdate(&lzp);
		std::vector<mpf_t*> thisPos = lzp.GetPosition();
		mpf_t *thisVal = configuration::g_function->Evaluate(thisPos);
		if(thisVal != NULL) {
			if(bestVal == NULL || (arbitraryprecisioncalculation::mpftoperations::Compare(thisVal, bestVal) <= 0)) {
				std::swap(bestVal, thisVal);
				std::swap(tempVelocity, bestVelocity);
			}
		}
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(thisPos);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(thisVal);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(tempVelocity);
		--samplesToGo;
	} while(samplesToGo > 0);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(searchRadius);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(minusSearchRadius);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(bestVal);
	p->SetVelocity(bestVelocity);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(bestVelocity);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newVelocity);
	configuration::g_bound_handling->SetParticleUpdate(p);
}

std::string SearchRadiusUpdaterCube::GetName() {
	return "SR_C";
}

SearchRadiusUpdaterNormalDistr::SearchRadiusUpdaterNormalDistr(double coefficient_radius, long long sample_points) : 
		coefficient_radius_(coefficient_radius) {
	
	if(sample_points < 0)
		sample_points_ = 0;
	else
		sample_points_ = sample_points;
}


void SearchRadiusUpdaterNormalDistr::Update(Particle* p) {
	std::vector<mpf_t*> localDir = configuration::g_bound_handling->GetDirectionVector(
			p->position, p->local_attractor_position);
	std::vector<mpf_t*> helperVector = arbitraryprecisioncalculation::vectoroperations::Randomize(localDir);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(localDir);
	localDir = helperVector;
	std::vector<mpf_t*> glAtPos = configuration::g_neighborhood->GetGlobalAttractorPosition(p);
	std::vector<mpf_t*> globalDir = configuration::g_bound_handling->GetDirectionVector(p->position, glAtPos);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(glAtPos);
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

	mpf_t *bestVal = NULL;
	if(sample_points_ > 0) {
		LazyParticle lzp(p);
		configuration::g_bound_handling->SetParticleUpdate(&lzp);
		std::vector<mpf_t*> newPos = lzp.GetPosition();
		bestVal = configuration::g_function->Evaluate(newPos);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newPos);
	}

	mpf_t *sqVelocityLength = arbitraryprecisioncalculation::vectoroperations::SquaredEuclideanLength(newVelocity);
	mpf_t *velocityLength = arbitraryprecisioncalculation::mpftoperations::Sqrt(sqVelocityLength);
	mpf_t *searchRadius = arbitraryprecisioncalculation::mpftoperations::Multiply(velocityLength, coefficient_radius_);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(sqVelocityLength);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(velocityLength);

	std::vector<mpf_t *> bestVelocity = arbitraryprecisioncalculation::vectoroperations::Clone(newVelocity);
	long long samplesToGo = sample_points_;
	do {
		LazyParticle lzp(p);
		std::vector<mpf_t*> tempVelocity(newVelocity.size(), NULL);
		for(unsigned int i = 0; i < newVelocity.size(); ++i) {
			tempVelocity[i] = arbitraryprecisioncalculation::mpftoperations::GetGaussianRandomMpft(newVelocity[i], searchRadius);
		}
		lzp.SetVelocity(tempVelocity);
		configuration::g_bound_handling->SetParticleUpdate(&lzp);
		std::vector<mpf_t*> thisPos = lzp.GetPosition();
		mpf_t *thisVal = configuration::g_function->Evaluate(thisPos);
		if(thisVal != NULL) {
			if(bestVal == NULL || (arbitraryprecisioncalculation::mpftoperations::Compare(thisVal, bestVal) <= 0)) {
				std::swap(bestVal, thisVal);
				std::swap(tempVelocity, bestVelocity);
			}
		}
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(thisPos);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(thisVal);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(tempVelocity);
		--samplesToGo;
	} while(samplesToGo > 0);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(searchRadius);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(bestVal);
	p->SetVelocity(bestVelocity);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(bestVelocity);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newVelocity);
	configuration::g_bound_handling->SetParticleUpdate(p);
}

std::string SearchRadiusUpdaterNormalDistr::GetName() {
	return "SR_ND";
}


} // namespace highprecisionpso
