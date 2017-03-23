/**
* @file   bound_handling/hyperbolic.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file contains the hyperbolic bound handling strategy.
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

#include "bound_handling/hyperbolic.h"

#include "function/function.h"
#include "general/configuration.h"
#include "arbitrary_precision_calculation/operations.h"
#include "general/particle.h"

namespace highprecisionpso {

void BoundHandlingHyperbolic::SetParticleUpdate(Particle * p){
	std::vector<mpf_t*> oldPos = p->GetPosition();
	std::vector<mpf_t*> vel = p->GetVelocity();
	std::vector<mpf_t*> low_position = configuration::g_function->GetLowerSearchSpaceBound();
	std::vector<mpf_t*> high_position = configuration::g_function->GetUpperSearchSpaceBound();
	for(unsigned int d = 0; d < vel.size(); d++){
		mpf_t* dif = (mpf_t*) NULL;
		if(arbitraryprecisioncalculation::mpftoperations::Compare(vel[d], 0.0) > 0) {
			dif = arbitraryprecisioncalculation::mpftoperations::Subtract(high_position[d], oldPos[d]);
		} else {
			dif = arbitraryprecisioncalculation::mpftoperations::Subtract(low_position[d], oldPos[d]);
		}
		mpf_t* tmp1 = arbitraryprecisioncalculation::mpftoperations::Divide(vel[d], dif);
		mpf_t* tmp2 = arbitraryprecisioncalculation::mpftoperations::Abs(tmp1);
		mpf_t* n1 = arbitraryprecisioncalculation::mpftoperations::ToMpft(1.0);
		mpf_t* denominator = arbitraryprecisioncalculation::mpftoperations::Add(tmp2, n1);
		mpf_t* new_vel = arbitraryprecisioncalculation::mpftoperations::Divide(vel[d], denominator);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(vel[d]);
		vel[d] = new_vel;
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(n1);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(dif);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp1);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp2);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(denominator);
	}
	p->SetVelocity(vel);
	std::vector<mpf_t*> newPos = arbitraryprecisioncalculation::vectoroperations::Add(oldPos, vel);
	p->SetPosition(newPos);
	// There is no velocity adjustment because this bound handling strategy already
	// adjusts the velocity.
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(low_position);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(high_position);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(oldPos);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(vel);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newPos);
}

std::string BoundHandlingHyperbolic::GetName(){
	return "Hyp";
}

} // namespace highprecisionpso
