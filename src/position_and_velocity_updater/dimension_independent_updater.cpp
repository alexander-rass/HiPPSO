/**
* @file   dimension_independent_updater.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   August, 2015
* @brief  This file contains a position and velocity updater, which acts independent to the coordinate axes.
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

#include "position_and_velocity_updater/dimension_independent_updater.h"

#include <gmp.h>
#include <sstream>
#include <vector>

#include "bound_handling/bound_handling.h"
#include "general/check_condition.h"
#include "general/configuration.h"
#include "general/operations.h"
#include "general/particle.h"
#include "neighborhood/neighborhood.h"

namespace highprecisionpso {

DimensionIndependentUpdater::DimensionIndependentUpdater(double reduction, int max_directions):reduction_(reduction), max_directions_(max_directions){}

void DimensionIndependentUpdater::Update(Particle* p) {
	AssertCondition(max_directions_ <= configuration::g_dimensions, "The number of directions for the dimension independent updater is to large. The maximal allowed value is the number of dimensions.");
	AssertCondition(max_directions_ > 0, "The number of directions for the dimension independent updater needs to be a positive value");

	std::vector<mpf_t*> nextVelocity = vectoroperations::Multiply(p->velocity,
			configuration::g_chi);

	std::vector<mpf_t*> localAttractorPosition = p->GetLocalAttractorPosition();
	std::vector<mpf_t*> globalAttractorPosition = configuration::g_neighborhood->GetGlobalAttractorPosition(p);

	std::vector<mpf_t*> main_direction;
	if(!vectoroperations::Equals(localAttractorPosition, globalAttractorPosition)){
		main_direction = configuration::g_bound_handling->GetDirectionVector(globalAttractorPosition, localAttractorPosition);
	} else if(!vectoroperations::Equals(p->position, globalAttractorPosition)){
		main_direction = configuration::g_bound_handling->GetDirectionVector(globalAttractorPosition, p->position);
	}
	
	if(!main_direction.empty()) {
		std::vector<mpf_t*> localDir = configuration::g_bound_handling->GetDirectionVector(
				p->position, localAttractorPosition);
		std::vector<mpf_t*> globalDir = configuration::g_bound_handling->GetDirectionVector(
				p->position, globalAttractorPosition);
		std::vector<std::vector<mpf_t*> > used_directions;
		std::vector<mpf_t*> remaining_main_direction = main_direction;

		for(int dirs = 0; dirs < max_directions_ - 1; dirs++){
			std::vector<mpf_t*> random_direction;
			for(int d = 0; d < configuration::g_dimensions; d++){
				random_direction.push_back(mpftoperations::GetGaussianRandomMpft(0.0, 1.0));
			}
			// remove used directions
			for(unsigned int i = 0; i < used_directions.size(); i++){
				std::vector<mpf_t*> projection = vectoroperations::OrthogonalProjection(random_direction, used_directions[i]);
				std::vector<mpf_t*> remaining = vectoroperations::Subtract(random_direction, projection);
				vectoroperations::ReleaseValues(random_direction);
				vectoroperations::ReleaseValues(projection);
				random_direction = remaining;
			}
			// remove remaining main direction
			{
				std::vector<mpf_t*> projection = vectoroperations::OrthogonalProjection(random_direction, remaining_main_direction);
				std::vector<mpf_t*> remaining = vectoroperations::Subtract(random_direction, projection);
				vectoroperations::ReleaseValues(random_direction);
				vectoroperations::ReleaseValues(projection);
				random_direction = remaining;
			}
			// scale random direction to correct relative length of remaining main direction
			mpf_t* random_direction_length;
			{
				std::vector<mpf_t*> tmp = vectoroperations::Multiply(random_direction, random_direction);
				random_direction_length = vectoroperations::Add(tmp);
				vectoroperations::ReleaseValues(tmp);
			}
			mpf_t* remaining_main_direction_length;
			{
				std::vector<mpf_t*> tmp = vectoroperations::Multiply(remaining_main_direction, remaining_main_direction);
				remaining_main_direction_length = vectoroperations::Add(tmp);
				vectoroperations::ReleaseValues(tmp);
			}
			AssertCondition(mpftoperations::Compare( random_direction_length, 0.0 ) > 0, "This is an internal error in the dimension independent updater. Please contact the program author.");
			AssertCondition(mpftoperations::Compare( remaining_main_direction_length, 0.0 ) > 0, "This is an internal error in the dimension independent updater. Please contact the program author.");
			mpf_t* squared_scale = mpftoperations::Divide(remaining_main_direction_length, random_direction_length);
			mpf_t* scale = mpftoperations::Sqrt(squared_scale);
			std::vector<mpf_t*> scaled_random_direction = vectoroperations::Multiply(random_direction, scale);
			std::vector<mpf_t*> reduced_random_direction = vectoroperations::Multiply(scaled_random_direction, reduction_);
			// the next direction consists of the remaining main direction and the reduced random direction
			std::vector<mpf_t*> next_direction = vectoroperations::Add(reduced_random_direction, remaining_main_direction);
			mpftoperations::ReleaseValue(squared_scale);
			mpftoperations::ReleaseValue(scale);
			mpftoperations::ReleaseValue(random_direction_length);
			mpftoperations::ReleaseValue(remaining_main_direction_length);
			vectoroperations::ReleaseValues(random_direction);
			vectoroperations::ReleaseValues(scaled_random_direction);
			vectoroperations::ReleaseValues(reduced_random_direction);
			// update velocity in the current direction and update remaining vectors
			std::vector<mpf_t*> curLocalDir = vectoroperations::OrthogonalProjection(localDir, next_direction);
			{
				std::vector<mpf_t*> tmp = vectoroperations::Subtract(localDir, curLocalDir);
				vectoroperations::ReleaseValues(localDir);
				localDir = tmp;
			}
			// - update velocity with local attractor
			mpf_t* local_part_scale = mpftoperations::ToMpft(configuration::g_coefficient_local_attractor);
			mpf_t* local_part_randomized = mpftoperations::Randomize(local_part_scale);
			std::vector<mpf_t*> curLocalPart = vectoroperations::Multiply(
					curLocalDir,
					local_part_randomized);
			mpftoperations::ReleaseValue(local_part_scale);
			local_part_scale = NULL;
			mpftoperations::ReleaseValue(local_part_randomized);
			local_part_randomized = NULL;

			{
				std::vector<mpf_t*> tmp = vectoroperations::Add(nextVelocity, curLocalPart);
				std::swap(tmp, nextVelocity);
				vectoroperations::ReleaseValues(tmp);
			}
			vectoroperations::ReleaseValues(curLocalDir);
			vectoroperations::ReleaseValues(curLocalPart);

			std::vector<mpf_t*> curGlobalDir = vectoroperations::OrthogonalProjection(globalDir, next_direction);
			{
				std::vector<mpf_t*> tmp = vectoroperations::Subtract(globalDir, curGlobalDir);
				vectoroperations::ReleaseValues(globalDir);
				globalDir = tmp;
			}
			// - update velocity with global attractor
			mpf_t* global_part_scale = mpftoperations::ToMpft(configuration::g_coefficient_global_attractor);
			mpf_t* global_part_randomized = mpftoperations::Randomize(global_part_scale);
			std::vector<mpf_t*> curGlobalPart = vectoroperations::Multiply(curGlobalDir, global_part_randomized);
			mpftoperations::ReleaseValue(global_part_scale);
			global_part_scale = NULL;
			mpftoperations::ReleaseValue(global_part_randomized);
			global_part_randomized = NULL;
			{
				std::vector<mpf_t*> tmp = vectoroperations::Add(nextVelocity, curGlobalPart);
				std::swap(tmp, nextVelocity);
				vectoroperations::ReleaseValues(tmp);
			}
			vectoroperations::ReleaseValues(curGlobalDir);
			vectoroperations::ReleaseValues(curGlobalPart);

			used_directions.push_back(next_direction);
			{
				std::vector<mpf_t*> tmp1 = vectoroperations::OrthogonalProjection(remaining_main_direction, next_direction);
				std::vector<mpf_t*> tmp2 = vectoroperations::Subtract(remaining_main_direction, tmp1);
				std::swap(tmp2, remaining_main_direction);
				vectoroperations::ReleaseValues(tmp1);
				vectoroperations::ReleaseValues(tmp2);
			}
		}
		// - update velocity with remaining local attractor
		mpf_t* local_part_scale = mpftoperations::ToMpft(configuration::g_coefficient_local_attractor);
		mpf_t* local_part_randomized = mpftoperations::Randomize(local_part_scale);
		std::vector<mpf_t*> localPart = vectoroperations::Multiply(
				localDir,
				local_part_randomized);
		mpftoperations::ReleaseValue(local_part_scale);
		local_part_scale = NULL;
		mpftoperations::ReleaseValue(local_part_randomized);
		local_part_randomized = NULL;
		{
			std::vector<mpf_t*> tmp = vectoroperations::Add(nextVelocity, localPart);
			std::swap(tmp, nextVelocity);
			vectoroperations::ReleaseValues(tmp);
		}
		vectoroperations::ReleaseValues(localPart);
		// - update velocity with remaining global attractor
		mpf_t* global_part_scale = mpftoperations::ToMpft(configuration::g_coefficient_global_attractor);
		mpf_t* global_part_randomized = mpftoperations::Randomize(global_part_scale);
		std::vector<mpf_t*> globalPart = vectoroperations::Multiply(globalDir, global_part_randomized);
		mpftoperations::ReleaseValue(global_part_scale);
		global_part_scale = NULL;
		mpftoperations::ReleaseValue(global_part_randomized);
		global_part_randomized = NULL;
		{
			std::vector<mpf_t*> tmp = vectoroperations::Add(nextVelocity, globalPart);
			std::swap(tmp, nextVelocity);
			vectoroperations::ReleaseValues(tmp);
		}
		vectoroperations::ReleaseValues(globalPart);

		for(unsigned int i = 0; i < used_directions.size(); i++){
			vectoroperations::ReleaseValues(used_directions[i]);
		}
		vectoroperations::ReleaseValues(remaining_main_direction);
		vectoroperations::ReleaseValues(localDir);
		vectoroperations::ReleaseValues(globalDir);
	}
	p->SetVelocity(nextVelocity);
	vectoroperations::ReleaseValues(nextVelocity);
	vectoroperations::ReleaseValues(localAttractorPosition);
	vectoroperations::ReleaseValues(globalAttractorPosition);
	configuration::g_bound_handling->SetParticleUpdate(p);
}

std::string DimensionIndependentUpdater::GetName(){
	std::ostringstream os;
	os << "DimIndep" << reduction_ << "_" << max_directions_;
	return os.str();
}

} // namespace highprecisionpso
