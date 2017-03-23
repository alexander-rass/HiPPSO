/**
* @file   position_and_velocity_updater/orientation_change_updater.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2014
* @brief  This file contains a position and velocity updater, which changes orientation if some directions have little movement.
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

#include "position_and_velocity_updater/orientation_change_updater.h"

#include <gmp.h>
#include <sstream>
#include <vector>
#include <iostream>

#include "bound_handling/bound_handling.h"
#include "general/configuration.h"
#include "arbitrary_precision_calculation/operations.h"
#include "general/particle.h"
#include "neighborhood/neighborhood.h"

namespace highprecisionpso {

OrientationChangeUpdater::OrientationChangeUpdater(double log2limit):log2_limit_(log2limit){
	orthogonal_transformation_matrix_.clear();
}

void OrientationChangeUpdater::Update(Particle* p) {
	if(orthogonal_transformation_matrix_.size() != (unsigned int) configuration::g_dimensions){
		for(unsigned int i = 0; i < orthogonal_transformation_matrix_.size(); i++){
			arbitraryprecisioncalculation::vectoroperations::ReleaseValues(orthogonal_transformation_matrix_[i]);
		}
		orthogonal_transformation_matrix_ = std::vector< std::vector<mpf_t*> >(configuration::g_dimensions, std::vector<mpf_t*>(configuration::g_dimensions, (mpf_t*)NULL));
		for(int i = 0; i < configuration::g_dimensions; ++i){
			for(int j = 0; j < configuration::g_dimensions; ++j){
				orthogonal_transformation_matrix_[i][j] = arbitraryprecisioncalculation::mpftoperations::ToMpft(((i==j)?1.0:0.0));
			}
		}
	}
	std::vector<mpf_t*> localDir = configuration::g_bound_handling->GetDirectionVector(
			p->position, p->local_attractor_position);
	std::vector<mpf_t*> glAtPos = configuration::g_neighborhood->GetGlobalAttractorPosition(p);
	std::vector<mpf_t*> globalDir = configuration::g_bound_handling->GetDirectionVector(p->position, glAtPos);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(glAtPos);
	std::pair<double, int> lowest = std::make_pair(1e301, -1), highest = std::make_pair(-1e301, -1);
	std::vector<mpf_t*> velRot = Rotation(p->velocity);
	std::vector<mpf_t*> globalRot = Rotation(globalDir);
	std::vector<mpf_t*> localRot = Rotation(localDir);
	for(int d = 0; d < configuration::g_dimensions; ++d){
		mpf_t* v1 = arbitraryprecisioncalculation::mpftoperations::Abs(velRot[d]);
		mpf_t* v2 = arbitraryprecisioncalculation::mpftoperations::Abs(globalRot[d]);
		mpf_t* v3 = arbitraryprecisioncalculation::mpftoperations::Abs(localRot[d]);
		mpf_t* sum1 = arbitraryprecisioncalculation::mpftoperations::Add(v1, v2);
		mpf_t* sum2 = arbitraryprecisioncalculation::mpftoperations::Add(sum1, v3);
		double logv = arbitraryprecisioncalculation::mpftoperations::Log2Double(sum2);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v1);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v2);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v3);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(sum1);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(sum2);
		std::pair<double,int> cur = std::make_pair(logv, d);
		lowest = std::min(lowest, cur);
		highest = std::max(highest, cur);
	}

	if(highest.first - lowest.first > log2_limit_){
		int d1 = lowest.second, d2 = highest.second;
		if(d1 != d2 && std::min(d1, d2) != -1){
			mpf_t* two = arbitraryprecisioncalculation::mpftoperations::ToMpft(0.5);
			mpf_t* sq2 = arbitraryprecisioncalculation::mpftoperations::Sqrt(two);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(two);
			std::vector<mpf_t*> vec1, vec2;
			for(int d = 0; d < configuration::g_dimensions; ++d){
				mpf_t* v = arbitraryprecisioncalculation::mpftoperations::Add(orthogonal_transformation_matrix_[d1][d], orthogonal_transformation_matrix_[d2][d]);
				vec1.push_back(arbitraryprecisioncalculation::mpftoperations::Multiply(v, sq2));
				arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v);
				v = arbitraryprecisioncalculation::mpftoperations::Subtract(orthogonal_transformation_matrix_[d1][d], orthogonal_transformation_matrix_[d2][d]);
				vec2.push_back(arbitraryprecisioncalculation::mpftoperations::Multiply(v, sq2));
				arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v);
			}
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(sq2);
			arbitraryprecisioncalculation::vectoroperations::ReleaseValues(orthogonal_transformation_matrix_[d1]);
			arbitraryprecisioncalculation::vectoroperations::ReleaseValues(orthogonal_transformation_matrix_[d2]);
			orthogonal_transformation_matrix_[d1] = vec1;
			orthogonal_transformation_matrix_[d2] = vec2;

			arbitraryprecisioncalculation::vectoroperations::ReleaseValues(velRot);
			arbitraryprecisioncalculation::vectoroperations::ReleaseValues(globalRot);
			arbitraryprecisioncalculation::vectoroperations::ReleaseValues(localRot);
			velRot = Rotation(p->velocity);
			globalRot = Rotation(globalDir);
			localRot = Rotation(localDir);
#if 0		// CHECK whether inverse of the orthogonal transformation matrix
			// is equal to the transposed orthogonal transformation matrix
			// Finally the maximal difference between the product of
			// the orthogonal transformation matrix and its transposed version
			// and identity matrix is printed to stdout.
			arbitraryprecisioncalculation::mpftoperations::StatisticalCalculationsStart();
			mpf_t* maxdif = arbitraryprecisioncalculation::mpftoperations::ToMpft(0.0);
			for(int i = 0; i < configuration::g_dimensions; i++) for(int j = 0;j < configuration::g_dimensions; j++){
				mpf_t* cur = arbitraryprecisioncalculation::mpftoperations::ToMpft((i==j)?1.0:0.0);
				for(int k = 0; k < configuration::g_dimensions; k++){
					mpf_t* tmp = arbitraryprecisioncalculation::mpftoperations::Multiply(orthogonal_transformation_matrix_[i][k], orthogonal_transformation_matrix_[j][k]);
					mpf_t* next = arbitraryprecisioncalculation::mpftoperations::Subtract(cur, tmp);
					arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp);
					arbitraryprecisioncalculation::mpftoperations::ReleaseValue(cur);
					cur = next;
				}
				mpf_t* tmp = arbitraryprecisioncalculation::mpftoperations::Abs(cur);
				arbitraryprecisioncalculation::mpftoperations::ReleaseValue(cur);
				mpf_t* nmax = arbitraryprecisioncalculation::mpftoperations::Max(tmp, maxdif);
				arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp);
				arbitraryprecisioncalculation::mpftoperations::ReleaseValue(maxdif);
				maxdif = nmax;
			}
			std::cout << arbitraryprecisioncalculation::mpftoperations::MpftToString(maxdif) << std::endl;
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(maxdif);
			arbitraryprecisioncalculation::mpftoperations::StatisticalCalculationsEnd();
#endif
		}
	}

	std::vector<mpf_t*> lRotRan = arbitraryprecisioncalculation::vectoroperations::Randomize(localRot);
	std::vector<mpf_t*> gRotRan = arbitraryprecisioncalculation::vectoroperations::Randomize(globalRot);

	std::vector<mpf_t*> scaledGRotRan = arbitraryprecisioncalculation::vectoroperations::Multiply(gRotRan, configuration::g_coefficient_global_attractor);
	std::vector<mpf_t*> scaledLRotRan = arbitraryprecisioncalculation::vectoroperations::Multiply(lRotRan, configuration::g_coefficient_local_attractor);

	std::vector<mpf_t*> rotRan = arbitraryprecisioncalculation::vectoroperations::Add(scaledGRotRan, scaledLRotRan);

	std::vector<mpf_t*> randomPart = InverseRotation(rotRan);
	std::vector<mpf_t*> oldPart = arbitraryprecisioncalculation::vectoroperations::Multiply(p->velocity, configuration::g_chi);

	std::vector<mpf_t*> newVelocity = arbitraryprecisioncalculation::vectoroperations::Add(randomPart, oldPart);

	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(localDir);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(globalDir);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(velRot);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(globalRot);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(localRot);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(gRotRan);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(lRotRan);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(scaledGRotRan);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(scaledLRotRan);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(rotRan);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(randomPart);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(oldPart);

	p->SetVelocity(newVelocity);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(newVelocity);
	configuration::g_bound_handling->SetParticleUpdate(p);
}

std::string OrientationChangeUpdater::GetName(){
	std::ostringstream os;
	os << "OC" << log2_limit_;
	std::string res = os.str();
	return res;
}

std::vector<mpf_t*> OrientationChangeUpdater::Rotation(std::vector<mpf_t*> v){
	std::vector<mpf_t*> res;
	for(int d = 0; d < configuration::g_dimensions; ++d){
		mpf_t* next = arbitraryprecisioncalculation::mpftoperations::ToMpft(0.0);
		for(int i = 0; i < configuration::g_dimensions; ++i){
			mpf_t* cur = arbitraryprecisioncalculation::mpftoperations::Multiply(v[i], orthogonal_transformation_matrix_[d][i]);
			mpf_t* prev = next;
			next = arbitraryprecisioncalculation::mpftoperations::Add(cur, prev);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(cur);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(prev);
		}
		res.push_back(next);
	}
	return res;
}

std::vector<mpf_t*> OrientationChangeUpdater::InverseRotation(std::vector<mpf_t*> v){
	std::vector<mpf_t*> res;
	for(int d = 0; d < configuration::g_dimensions; ++d){
		mpf_t* next = arbitraryprecisioncalculation::mpftoperations::ToMpft(0.0);
		for(int i = 0; i < configuration::g_dimensions; ++i){
			mpf_t* cur = arbitraryprecisioncalculation::mpftoperations::Multiply(v[i], orthogonal_transformation_matrix_[i][d]);
			mpf_t* prev = next;
			next = arbitraryprecisioncalculation::mpftoperations::Add(cur, prev);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(cur);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(prev);
		}
		res.push_back(next);
	}
	return res;
}

void OrientationChangeUpdater::LoadData(std::ifstream* inputstream, ProgramVersion* version_of_stored_data){
	int R;
	(*inputstream) >> R;
	orthogonal_transformation_matrix_ = std::vector<std::vector<mpf_t*> >(R);
	for(int r = 0; r < R; r++){
		int C;
		(*inputstream) >> C;
		orthogonal_transformation_matrix_[r] = std::vector<mpf_t*>(C, (mpf_t*) NULL);
		for(int c = 0; c < C; c++){
			orthogonal_transformation_matrix_[r][c] = arbitraryprecisioncalculation::mpftoperations::LoadMpft(inputstream);
		}
	}
}

void OrientationChangeUpdater::StoreData(std::ofstream* outputstream) {
	(*outputstream) << orthogonal_transformation_matrix_.size() << std::endl;
	for(unsigned int i = 0; i < orthogonal_transformation_matrix_.size(); i++){
		(*outputstream) << orthogonal_transformation_matrix_[i].size() << std::endl;
		for(unsigned int j = 0; j < orthogonal_transformation_matrix_[i].size(); j++) {
			arbitraryprecisioncalculation::mpftoperations::StoreMpft(orthogonal_transformation_matrix_[i][j], outputstream);
		}
	}
}

} // namespace highprecisionpso
