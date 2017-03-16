/**
* @file   default_updater.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains the standard class for processing the velocity and position update of each particle.
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

#include "position_and_velocity_updater/default_updater.h"

#include <gmp.h>
#include <vector>

#include "bound_handling/bound_handling.h"
#include "general/configuration.h"
#include "general/operations.h"
#include "general/particle.h"
#include "neighborhood/neighborhood.h"

namespace highprecisionpso {

void DefaultUpdater::Update(Particle* p) {
	std::vector<mpf_t*> localDir = configuration::g_bound_handling->GetDirectionVector(
			p->position, p->local_attractor_position);
	std::vector<mpf_t*> helperVector = vectoroperations::Randomize(localDir);
	vectoroperations::ReleaseValues(localDir);
	localDir = helperVector;
	std::vector<mpf_t*> glAtPos = configuration::g_neighborhood->GetGlobalAttractorPosition(p);
	std::vector<mpf_t*> globalDir = configuration::g_bound_handling->GetDirectionVector(p->position, glAtPos);
	vectoroperations::ReleaseValues(glAtPos);
	helperVector = vectoroperations::Randomize(globalDir);
	vectoroperations::ReleaseValues(globalDir);
	globalDir = helperVector;
	std::vector<mpf_t*> localPart = vectoroperations::Multiply(localDir,
			configuration::g_coefficient_local_attractor);
	vectoroperations::ReleaseValues(localDir);
	std::vector<mpf_t*> globalPart = vectoroperations::Multiply(globalDir,
			configuration::g_coefficient_global_attractor);
	vectoroperations::ReleaseValues(globalDir);
	std::vector<mpf_t*> oldVelocityPart = vectoroperations::Multiply(p->velocity,
			configuration::g_chi);
	helperVector = vectoroperations::Add(localPart, globalPart);
	vectoroperations::ReleaseValues(localPart);
	vectoroperations::ReleaseValues(globalPart);
	std::vector<mpf_t*> newVelocity = vectoroperations::Add(oldVelocityPart,
			helperVector);
	vectoroperations::ReleaseValues(oldVelocityPart);
	vectoroperations::ReleaseValues(helperVector);
	p->SetVelocity(newVelocity);
	vectoroperations::ReleaseValues(newVelocity);
	configuration::g_bound_handling->SetParticleUpdate(p);
}

std::string DefaultUpdater::GetName(){
	return "Default";
}

} // namespace highprecisionpso
