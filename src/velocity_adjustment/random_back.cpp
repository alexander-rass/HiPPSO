/**
* @file   velocity_adjustment/random_back.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file contains the random back velocity adjustment strategy.
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

#include "velocity_adjustment/random_back.h"

#include "arbitrary_precision_calculation/operations.h"
#include "general/particle.h"

namespace highprecisionpso {

void VelocityAdjustmentRandomBack::AdjustVelocity(Particle * p, std::vector<bool> adjustDimensions, std::vector<mpf_t*> oldPosition){
	for(unsigned int i = 0; i < adjustDimensions.size(); i++) {
		if(adjustDimensions[i]) {
			mpf_t* tmp = arbitraryprecisioncalculation::mpftoperations::Negate(p->velocity[i]);
			mpf_t* tmp2 = arbitraryprecisioncalculation::mpftoperations::Randomize(tmp);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(p->velocity[i]);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp);
			p->velocity[i] = tmp2;
		}
	}
}

std::string VelocityAdjustmentRandomBack::GetName(){
	return "RanBack";
}

} // namespace highprecisionpso
