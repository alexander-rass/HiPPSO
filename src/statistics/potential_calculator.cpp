/**
* @file   potential_calculator.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2014
* @brief  This file contains classes for statistical calculations.
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

#include "statistics/potential_calculator.h"

#include <sstream>

#include "function/function.h"
#include "general/configuration.h"
#include "general/operations.h"
#include "general/particle.h"
#include "neighborhood/neighborhood.h"
#include "statistics/statistics.h"

namespace highprecisionpso {

std::vector<std::vector<mpf_t*> > PotentialCalculator::Evaluate(){
	std::vector<std::vector<mpf_t*> > data;
	for(int i = 0; i < configuration::g_particles; i++) {
		data.push_back(std::vector<mpf_t*>(0));
		Particle* p = (*(configuration::g_statistics->swarm))[i];
		for(int j = 0; j < configuration::g_dimensions; j++){
			data[i].push_back(Evaluate(p, j));
		}
	}
	return data;
}

// PotentialFunctionDifference
mpf_t* PotentialFunctionDifference::Evaluate(Particle* p, int dim){
	mpf_t* f1 = (*(configuration::g_function)).Evaluate(p->position);
	std::vector<mpf_t*> position2 = p->position;
	mpf_t* x = mpftoperations::Add(p->position[dim], p->velocity[dim]);
	position2[dim] = x;
	mpf_t* f2 = (*(configuration::g_function)).Evaluate(position2);
	mpftoperations::ReleaseValue(x);
	mpf_t* dif = mpftoperations::Subtract(f1,f2);
	mpf_t* res = mpftoperations::Abs(dif);
	mpftoperations::ReleaseValue(f1);
	mpftoperations::ReleaseValue(f2);
	mpftoperations::ReleaseValue(dif);
	return res;
}

std::string PotentialFunctionDifference::GetName(){
	return "FD";
}

// PotentialAbsVelocityPlusDistToGlobalAttractor
PotentialAbsVelocityPlusDistToGlobalAttractor::PotentialAbsVelocityPlusDistToGlobalAttractor(double scale) : scale_(scale){
}

mpf_t* PotentialAbsVelocityPlusDistToGlobalAttractor::Evaluate(Particle* p, int dim){
	mpf_t* v1 = mpftoperations::Abs(p->velocity[dim]);
	mpf_t* v2 = mpftoperations::Multiply(v1, scale_);
	mpftoperations::ReleaseValue(v1);
	mpf_t* res = v2;
	std::vector<mpf_t*> glAtPos = configuration::g_neighborhood->GetGlobalAttractorPosition(p);
	v1 = mpftoperations::Subtract(p->position[dim], glAtPos[dim]);
	vectoroperations::ReleaseValues(glAtPos);
	v2 = mpftoperations::Abs(v1);
	mpftoperations::ReleaseValue(v1);
	v1 = res;
	res = mpftoperations::Add(v1, v2);
	mpftoperations::ReleaseValue(v1);
	mpftoperations::ReleaseValue(v2);
	return res;
}

std::string PotentialAbsVelocityPlusDistToGlobalAttractor::GetName(){
	std::ostringstream os;
	os << "VpG" << scale_;
	std::string res = os.str();
	return res;
}

PotentialSqrtAbsVelocityPlusSqrtDistToGlobalAttractor::PotentialSqrtAbsVelocityPlusSqrtDistToGlobalAttractor(double scale) : scale_(scale){}

mpf_t* PotentialSqrtAbsVelocityPlusSqrtDistToGlobalAttractor::Evaluate(Particle* p, int dim){
	mpf_t* v1 = mpftoperations::Abs(p->velocity[dim]);
	mpf_t* v2 = mpftoperations::Sqrt(v1);
	mpftoperations::ReleaseValue(v1);
	v1 = mpftoperations::Multiply(v2, scale_);
	mpftoperations::ReleaseValue(v2);
	mpf_t* sqrtv = v1;
	std::vector<mpf_t*> glAtPos = configuration::g_neighborhood->GetGlobalAttractorPosition(p);
	v1 = mpftoperations::Subtract(p->position[dim], glAtPos[dim]);
	vectoroperations::ReleaseValues(glAtPos);
	v2 = mpftoperations::Abs(v1);
	mpftoperations::ReleaseValue(v1);
	mpf_t* sqrtdiff = mpftoperations::Sqrt(v2);
	mpftoperations::ReleaseValue(v2);
	mpf_t* res = mpftoperations::Add(sqrtv, sqrtdiff);
	mpftoperations::ReleaseValue(sqrtv);
	mpftoperations::ReleaseValue(sqrtdiff);
	return res;
}

std::string PotentialSqrtAbsVelocityPlusSqrtDistToGlobalAttractor::GetName(){
	return "SqVpSqP";
}

} // namespace highprecisionpso
