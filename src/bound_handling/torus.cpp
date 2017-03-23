/**
* @file   bound_handling/torus.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file contains the torus bound handling strategy.
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

#include "bound_handling/torus.h"

#include "function/function.h"
#include "general/configuration.h"
#include "arbitrary_precision_calculation/operations.h"
#include "general/particle.h"

namespace highprecisionpso {

void BoundHandlingTorus::SetParticleUpdate(Particle * p){
	std::vector<mpf_t*> oldPos = p->GetPosition();
	std::vector<mpf_t*> vel = p->GetVelocity();
	std::vector<mpf_t*> newPos = arbitraryprecisioncalculation::vectoroperations::Add(oldPos, vel);
	{
		std::vector<mpf_t*> lower = configuration::g_function->GetLowerSearchSpaceBound();
		std::vector<mpf_t*> upper = configuration::g_function->GetUpperSearchSpaceBound();
		for(int i = 0; i < configuration::g_dimensions; i++){
			if(arbitraryprecisioncalculation::mpftoperations::Compare((lower[i]), (newPos[i])) > 0 ||
				arbitraryprecisioncalculation::mpftoperations::Compare((newPos[i]), (upper[i])) > 0) {

				mpf_t* mod_num = arbitraryprecisioncalculation::mpftoperations::Subtract(upper[i], lower[i]);
				while(arbitraryprecisioncalculation::mpftoperations::Compare(lower[i], newPos[i]) > 0) {
					mpf_t* tmp = arbitraryprecisioncalculation::mpftoperations::Add(newPos[i], mod_num);
					std::swap(tmp, newPos[i]);
					arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp);
				}
				while(arbitraryprecisioncalculation::mpftoperations::Compare(newPos[i], upper[i]) > 0) {
					mpf_t* tmp = arbitraryprecisioncalculation::mpftoperations::Subtract(newPos[i], mod_num);
					std::swap(tmp, newPos[i]);
					arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp);
				}
				arbitraryprecisioncalculation::mpftoperations::ReleaseValue(mod_num);
			}
		}
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(lower);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(upper);
	}
	p->SetPosition(newPos);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(oldPos);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(vel);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newPos);
}

std::string BoundHandlingTorus::GetName(){
	return "Torus";
}

std::vector<mpf_t*> BoundHandlingTorus::GetDirectionVector(const std::vector<mpf_t*> & position, const std::vector<mpf_t*> & aim){
	std::vector<mpf_t*> dir = arbitraryprecisioncalculation::vectoroperations::Subtract(aim, position);
	{
		std::vector<mpf_t*> lower = configuration::g_function->GetLowerSearchSpaceBound();
		std::vector<mpf_t*> upper = configuration::g_function->GetUpperSearchSpaceBound();
		for(int i = 0; i < configuration::g_dimensions; i++){
			mpf_t* mod_num = arbitraryprecisioncalculation::mpftoperations::Subtract(upper[i], lower[i]);
			std::vector<mpf_t*> options;
			options.push_back(arbitraryprecisioncalculation::mpftoperations::Add(dir[i], mod_num));
			options.push_back(arbitraryprecisioncalculation::mpftoperations::Subtract(dir[i], mod_num));
			mpf_t* abs_cur = arbitraryprecisioncalculation::mpftoperations::Abs(dir[i]);
			for(unsigned int k = 0; k < options.size(); k++){
				mpf_t* abs_next = arbitraryprecisioncalculation::mpftoperations::Abs(options[k]);
				if(arbitraryprecisioncalculation::mpftoperations::Compare(abs_next, abs_cur) < 0) {
					std::swap(abs_cur, abs_next);
					std::swap(dir[i], options[k]);
				}
				arbitraryprecisioncalculation::mpftoperations::ReleaseValue(abs_next);
			}
			arbitraryprecisioncalculation::vectoroperations::ReleaseValues(options);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(abs_cur);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(mod_num);
		}
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(lower);
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(upper);
	}
	return dir;
}

} // namespace highprecisionpso
