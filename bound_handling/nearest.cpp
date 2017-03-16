/**
* @file   nearest.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file contains the nearest bound handling strategy.
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

#include "bound_handling/nearest.h"

#include "function/function.h"
#include "general/configuration.h"
#include "general/operations.h"
#include "general/particle.h"
#include "velocity_adjustment/velocity_adjustment.h"

namespace highprecisionpso {

void BoundHandlingNearest::SetParticleUpdate(Particle * p){
	std::vector<mpf_t*> oldPos = p->GetPosition();
	std::vector<mpf_t*> vel = p->GetVelocity();
	std::vector<mpf_t*> newPos = vectoroperations::Add(oldPos, vel);
	std::vector<mpf_t*> low_position = configuration::g_function->GetLowerSearchSpaceBound();
	std::vector<mpf_t*> high_position = configuration::g_function->GetUpperSearchSpaceBound();
	std::vector<bool> modifiedDimensions(newPos.size(), false);
	for(unsigned int d = 0; d < newPos.size(); d++){
		if(mpftoperations::Compare(newPos[d], low_position[d]) < 0) {
			modifiedDimensions[d] = true;
			mpftoperations::ReleaseValue(newPos[d]);
			newPos[d] = mpftoperations::Clone(low_position[d]);
		}
		if(mpftoperations::Compare(newPos[d], high_position[d]) > 0) {
			modifiedDimensions[d] = true;
			mpftoperations::ReleaseValue(newPos[d]);
			newPos[d] = mpftoperations::Clone(high_position[d]);
		}
	}
	p->SetPosition(newPos);
	configuration::g_velocity_adjustment->AdjustVelocity(p, modifiedDimensions, oldPos);
	vectoroperations::ReleaseValues(low_position);
	vectoroperations::ReleaseValues(high_position);
	vectoroperations::ReleaseValues(oldPos);
	vectoroperations::ReleaseValues(vel);
	vectoroperations::ReleaseValues(newPos);
}

std::string BoundHandlingNearest::GetName(){
	return "Near";
}

} // namespace highprecisionpso
