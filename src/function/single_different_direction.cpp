/**
* @file   function/single_different_direction.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   August, 2015
* @brief  This file contains the description of function which performs different in a single direction.
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

#include "function/single_different_direction.h"

#include <sstream>

#include "general/check_condition.h"
#include "arbitrary_precision_calculation/operations.h"
#include "arbitrary_precision_calculation/parse.h"

namespace highprecisionpso {

SingleDifferentDirection::SingleDifferentDirection(double single_dimension_power,
		double remaining_dimension_power,
		SingleDifferentDirectionMode mode,
		std::vector<std::string> rng_description) :
				single_dimension_exponent_(single_dimension_power),
				remaining_dimension_exponent_(remaining_dimension_power),
				direction_mode_(mode),
				rng_description_(rng_description) {
	special_direction_cached_.clear();
	special_direction_cached_precision_ = 1;
}

SingleDifferentDirection::~SingleDifferentDirection(){
	arbitraryprecisioncalculation::mpftoperations::ChangeNumberOfMpftValuesCached( -(int)(special_direction_cached_.size()) );
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(special_direction_cached_);
	special_direction_cached_.clear();
}

void SingleDifferentDirection::InitSpecialDirection(const std::vector<mpf_t*> & vec) {
	unsigned int D = vec.size();
	if(special_direction_cached_.size() != vec.size() || special_direction_cached_precision_ != mpf_get_default_prec()) {
		special_direction_cached_precision_ = mpf_get_default_prec();
		arbitraryprecisioncalculation::mpftoperations::ChangeNumberOfMpftValuesCached( -((int)(special_direction_cached_.size())) );
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(special_direction_cached_);
		special_direction_cached_.clear();
		arbitraryprecisioncalculation::RandomNumberGenerator* rng = NULL;
		switch (direction_mode_){
			case SINGLE_DIFFERENT_DIRECTION_MODE_FIRST:
				special_direction_cached_.push_back(arbitraryprecisioncalculation::mpftoperations::ToMpft(1.0));
				for(unsigned int d = 1; d < D; d++){
					special_direction_cached_.push_back(arbitraryprecisioncalculation::mpftoperations::ToMpft(0.0));
				}
				break;
			case SINGLE_DIFFERENT_DIRECTION_MODE_DIAGONAL:
				for(unsigned int d = 0; d < D; d++){
					special_direction_cached_.push_back(arbitraryprecisioncalculation::mpftoperations::ToMpft(1.0));
				}
				break;
			case SINGLE_DIFFERENT_DIRECTION_MODE_RANDOM:
				{
					unsigned int parsed = 0;
					rng = arbitraryprecisioncalculation::parse::ParseRandomNumberGenerator(rng_description_, parsed);
				}
				for(unsigned int d = 0; d < D; d++){
					special_direction_cached_.push_back(arbitraryprecisioncalculation::mpftoperations::GetGaussianRandomMpft(0.0, 1.0, rng));
				}
				break;
			default:
				AssertCondition(false, "Invalid / Unsupported option for function singleDifferentDirection.");
		}
		arbitraryprecisioncalculation::mpftoperations::ChangeNumberOfMpftValuesCached( special_direction_cached_.size() );
	}
}

mpf_t* SingleDifferentDirection::Eval(const std::vector<mpf_t*> & vec) {
	InitSpecialDirection(vec);
	if(special_direction_cached_.size() != vec.size()){
		return arbitraryprecisioncalculation::mpftoperations::GetUndefined();
	}
	std::vector<mpf_t*> proj = arbitraryprecisioncalculation::vectoroperations::OrthogonalProjection(vec, special_direction_cached_);
	std::vector<mpf_t*> orth = arbitraryprecisioncalculation::vectoroperations::Subtract(vec , proj);
	mpf_t* len_proj = arbitraryprecisioncalculation::vectoroperations::SquaredEuclideanLength(proj);
	mpf_t* len_orth = arbitraryprecisioncalculation::vectoroperations::SquaredEuclideanLength(orth);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(proj);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(orth);
	if(single_dimension_exponent_ != 1.0){
		mpf_t* tmp = arbitraryprecisioncalculation::mpftoperations::Pow(len_proj, single_dimension_exponent_);
		std::swap(tmp, len_proj);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp);
	}
	if(remaining_dimension_exponent_ != 1.0){
		mpf_t* tmp = arbitraryprecisioncalculation::mpftoperations::Pow(len_orth, remaining_dimension_exponent_);
		std::swap(tmp, len_orth);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp);
	}
	mpf_t* res = arbitraryprecisioncalculation::mpftoperations::Add(len_proj, len_orth);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(len_proj);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(len_orth);
	return res;
}

std::string SingleDifferentDirection::GetName(){
	std::ostringstream os;
	os << "SinDiffDir" << single_dimension_exponent_ << "_" << remaining_dimension_exponent_ << "_" << (int)direction_mode_;
	return os.str();
}

} // namespace highprecisionpso
