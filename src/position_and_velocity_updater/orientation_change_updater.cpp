/**
* @file   orientation_change_updater.cpp
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
			vectoroperations::ReleaseValues(orthogonal_transformation_matrix_[i]);
		}
		orthogonal_transformation_matrix_ = std::vector< std::vector<mpf_t*> >(configuration::g_dimensions, std::vector<mpf_t*>(configuration::g_dimensions, (mpf_t*)NULL));
		for(int i = 0; i < configuration::g_dimensions; ++i){
			for(int j = 0; j < configuration::g_dimensions; ++j){
				orthogonal_transformation_matrix_[i][j] = mpftoperations::ToMpft(((i==j)?1.0:0.0));
			}
		}
	}
	std::vector<mpf_t*> localDir = configuration::g_bound_handling->GetDirectionVector(
			p->position, p->local_attractor_position);
	std::vector<mpf_t*> glAtPos = configuration::g_neighborhood->GetGlobalAttractorPosition(p);
	std::vector<mpf_t*> globalDir = configuration::g_bound_handling->GetDirectionVector(p->position, glAtPos);
	vectoroperations::ReleaseValues(glAtPos);
	std::pair<double, int> lowest = std::make_pair(1e301, -1), highest = std::make_pair(-1e301, -1);
	std::vector<mpf_t*> velRot = Rotation(p->velocity);
	std::vector<mpf_t*> globalRot = Rotation(globalDir);
	std::vector<mpf_t*> localRot = Rotation(localDir);
	for(int d = 0; d < configuration::g_dimensions; ++d){
		mpf_t* v1 = mpftoperations::Abs(velRot[d]);
		mpf_t* v2 = mpftoperations::Abs(globalRot[d]);
		mpf_t* v3 = mpftoperations::Abs(localRot[d]);
		mpf_t* sum1 = mpftoperations::Add(v1, v2);
		mpf_t* sum2 = mpftoperations::Add(sum1, v3);
		double logv = mpftoperations::Log2Double(sum2);
		mpftoperations::ReleaseValue(v1);
		mpftoperations::ReleaseValue(v2);
		mpftoperations::ReleaseValue(v3);
		mpftoperations::ReleaseValue(sum1);
		mpftoperations::ReleaseValue(sum2);
		std::pair<double,int> cur = std::make_pair(logv, d);
		lowest = std::min(lowest, cur);
		highest = std::max(highest, cur);
	}

	if(highest.first - lowest.first > log2_limit_){
		int d1 = lowest.second, d2 = highest.second;
		if(d1 != d2 && std::min(d1, d2) != -1){
			mpf_t* two = mpftoperations::ToMpft(0.5);
			mpf_t* sq2 = mpftoperations::Sqrt(two);
			mpftoperations::ReleaseValue(two);
			std::vector<mpf_t*> vec1, vec2;
			for(int d = 0; d < configuration::g_dimensions; ++d){
				mpf_t* v = mpftoperations::Add(orthogonal_transformation_matrix_[d1][d], orthogonal_transformation_matrix_[d2][d]);
				vec1.push_back(mpftoperations::Multiply(v, sq2));
				mpftoperations::ReleaseValue(v);
				v = mpftoperations::Subtract(orthogonal_transformation_matrix_[d1][d], orthogonal_transformation_matrix_[d2][d]);
				vec2.push_back(mpftoperations::Multiply(v, sq2));
				mpftoperations::ReleaseValue(v);
			}
			mpftoperations::ReleaseValue(sq2);
			vectoroperations::ReleaseValues(orthogonal_transformation_matrix_[d1]);
			vectoroperations::ReleaseValues(orthogonal_transformation_matrix_[d2]);
			orthogonal_transformation_matrix_[d1] = vec1;
			orthogonal_transformation_matrix_[d2] = vec2;

			vectoroperations::ReleaseValues(velRot);
			vectoroperations::ReleaseValues(globalRot);
			vectoroperations::ReleaseValues(localRot);
			velRot = Rotation(p->velocity);
			globalRot = Rotation(globalDir);
			localRot = Rotation(localDir);
#if 0		// CHECK whether inverse of the orthogonal transformation matrix
			// is equal to the transposed orthogonal transformation matrix
			// Finally the maximal difference between the product of
			// the orthogonal transformation matrix and its transposed version
			// and identity matrix is printed to stdout.
			mpftoperations::StatisticalCalculationsStart();
			mpf_t* maxdif = mpftoperations::ToMpft(0.0);
			for(int i = 0; i < configuration::g_dimensions; i++) for(int j = 0;j < configuration::g_dimensions; j++){
				mpf_t* cur = mpftoperations::ToMpft((i==j)?1.0:0.0);
				for(int k = 0; k < configuration::g_dimensions; k++){
					mpf_t* tmp = mpftoperations::Multiply(orthogonal_transformation_matrix_[i][k], orthogonal_transformation_matrix_[j][k]);
					mpf_t* next = mpftoperations::Subtract(cur, tmp);
					mpftoperations::ReleaseValue(tmp);
					mpftoperations::ReleaseValue(cur);
					cur = next;
				}
				mpf_t* tmp = mpftoperations::Abs(cur);
				mpftoperations::ReleaseValue(cur);
				mpf_t* nmax = mpftoperations::Max(tmp, maxdif);
				mpftoperations::ReleaseValue(tmp);
				mpftoperations::ReleaseValue(maxdif);
				maxdif = nmax;
			}
			std::cout << mpftoperations::MpftToString(maxdif) << std::endl;
			mpftoperations::ReleaseValue(maxdif);
			mpftoperations::StatisticalCalculationsEnd();
#endif
		}
	}

	std::vector<mpf_t*> lRotRan = vectoroperations::Randomize(localRot);
	std::vector<mpf_t*> gRotRan = vectoroperations::Randomize(globalRot);

	std::vector<mpf_t*> scaledGRotRan = vectoroperations::Multiply(gRotRan, configuration::g_coefficient_global_attractor);
	std::vector<mpf_t*> scaledLRotRan = vectoroperations::Multiply(lRotRan, configuration::g_coefficient_local_attractor);

	std::vector<mpf_t*> rotRan = vectoroperations::Add(scaledGRotRan, scaledLRotRan);

	std::vector<mpf_t*> randomPart = InverseRotation(rotRan);
	std::vector<mpf_t*> oldPart = vectoroperations::Multiply(p->velocity, configuration::g_chi);

	std::vector<mpf_t*> newVelocity = vectoroperations::Add(randomPart, oldPart);

	vectoroperations::ReleaseValues(localDir);
	vectoroperations::ReleaseValues(globalDir);
	vectoroperations::ReleaseValues(velRot);
	vectoroperations::ReleaseValues(globalRot);
	vectoroperations::ReleaseValues(localRot);
	vectoroperations::ReleaseValues(gRotRan);
	vectoroperations::ReleaseValues(lRotRan);
	vectoroperations::ReleaseValues(scaledGRotRan);
	vectoroperations::ReleaseValues(scaledLRotRan);
	vectoroperations::ReleaseValues(rotRan);
	vectoroperations::ReleaseValues(randomPart);
	vectoroperations::ReleaseValues(oldPart);

	p->SetVelocity(newVelocity);
	vectoroperations::ReleaseValues(newVelocity);
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
		mpf_t* next = mpftoperations::ToMpft(0.0);
		for(int i = 0; i < configuration::g_dimensions; ++i){
			mpf_t* cur = mpftoperations::Multiply(v[i], orthogonal_transformation_matrix_[d][i]);
			mpf_t* prev = next;
			next = mpftoperations::Add(cur, prev);
			mpftoperations::ReleaseValue(cur);
			mpftoperations::ReleaseValue(prev);
		}
		res.push_back(next);
	}
	return res;
}

std::vector<mpf_t*> OrientationChangeUpdater::InverseRotation(std::vector<mpf_t*> v){
	std::vector<mpf_t*> res;
	for(int d = 0; d < configuration::g_dimensions; ++d){
		mpf_t* next = mpftoperations::ToMpft(0.0);
		for(int i = 0; i < configuration::g_dimensions; ++i){
			mpf_t* cur = mpftoperations::Multiply(v[i], orthogonal_transformation_matrix_[i][d]);
			mpf_t* prev = next;
			next = mpftoperations::Add(cur, prev);
			mpftoperations::ReleaseValue(cur);
			mpftoperations::ReleaseValue(prev);
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
			orthogonal_transformation_matrix_[r][c] = mpftoperations::LoadMpft(inputstream, version_of_stored_data);
		}
	}
}

void OrientationChangeUpdater::StoreData(std::ofstream* outputstream) {
	(*outputstream) << orthogonal_transformation_matrix_.size() << std::endl;
	for(unsigned int i = 0; i < orthogonal_transformation_matrix_.size(); i++){
		(*outputstream) << orthogonal_transformation_matrix_[i].size() << std::endl;
		for(unsigned int j = 0; j < orthogonal_transformation_matrix_[i].size(); j++) {
			mpftoperations::StoreMpft(orthogonal_transformation_matrix_[i][j], outputstream);
		}
	}
}

} // namespace highprecisionpso
