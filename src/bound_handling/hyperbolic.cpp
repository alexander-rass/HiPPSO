/**
* @file   hyperbolic.cpp
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
		if(mpftoperations::Compare(vel[d], 0.0) > 0) {
			dif = mpftoperations::Subtract(high_position[d], oldPos[d]);
		} else {
			dif = mpftoperations::Subtract(low_position[d], oldPos[d]);
		}
		mpf_t* tmp1 = mpftoperations::Divide(vel[d], dif);
		mpf_t* tmp2 = mpftoperations::Abs(tmp1);
		mpf_t* n1 = mpftoperations::ToMpft(1.0);
		mpf_t* denominator = mpftoperations::Add(tmp2, n1);
		mpf_t* new_vel = mpftoperations::Divide(vel[d], denominator);
		mpftoperations::ReleaseValue(vel[d]);
		vel[d] = new_vel;
		mpftoperations::ReleaseValue(n1);
		mpftoperations::ReleaseValue(dif);
		mpftoperations::ReleaseValue(tmp1);
		mpftoperations::ReleaseValue(tmp2);
		mpftoperations::ReleaseValue(denominator);
	}
	p->SetVelocity(vel);
	std::vector<mpf_t*> newPos = vectoroperations::Add(oldPos, vel);
	p->SetPosition(newPos);
	// There is no velocity adjustment because this bound handling strategy already
	// adjusts the velocity.
	vectoroperations::ReleaseValues(low_position);
	vectoroperations::ReleaseValues(high_position);
	vectoroperations::ReleaseValues(oldPos);
	vectoroperations::ReleaseValues(vel);
	vectoroperations::ReleaseValues(newPos);
}

std::string BoundHandlingHyperbolic::GetName(){
	return "Hyp";
}

} // namespace highprecisionpso
