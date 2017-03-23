/**
* @file   function/random_positive_definite.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains the description of a function with random positive definite second derivative matrix.
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

#include <arbitrary_precision_calculation/random_number_generator.h>
#include "function/random_positive_definite.h"

#include "arbitrary_precision_calculation/operations.h"
#include "arbitrary_precision_calculation/parse.h"
#include "general/check_condition.h"

namespace highprecisionpso {

RandomPositiveDefiniteSecondDerivative::RandomPositiveDefiniteSecondDerivative(std::vector<std::string> rng_description):rng_description_(rng_description){
	matrix_cached_.clear();
	matrix_cached_precision_ = 1;
}

RandomPositiveDefiniteSecondDerivative::~RandomPositiveDefiniteSecondDerivative(){
	for(unsigned int i = 0; i < matrix_cached_.size(); i++){
		arbitraryprecisioncalculation::mpftoperations::ChangeNumberOfMpftValuesCached( -(int)(matrix_cached_[i].size()) );
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(matrix_cached_[i]);
		matrix_cached_[i].clear();
	}
}

void RandomPositiveDefiniteSecondDerivative::InitMatrixCached(const std::vector<mpf_t*> & vec){
	unsigned int D = vec.size();
	if(matrix_cached_.size() != D || matrix_cached_precision_ != mpf_get_default_prec()){
		matrix_cached_precision_ = mpf_get_default_prec();
		for(unsigned int i = 0; i < matrix_cached_.size(); i++){
			arbitraryprecisioncalculation::mpftoperations::ChangeNumberOfMpftValuesCached( -(int)(matrix_cached_[i].size()) );
			arbitraryprecisioncalculation::vectoroperations::ReleaseValues(matrix_cached_[i]);
		}

		arbitraryprecisioncalculation::RandomNumberGenerator* rng = NULL;
		{
			unsigned int parsed = 0;
			rng = arbitraryprecisioncalculation::parse::ParseRandomNumberGenerator(rng_description_, parsed);
		}
		std::vector<std::vector<mpf_t*> > tmp(D, std::vector<mpf_t*>(D, (mpf_t*) NULL));
		for(unsigned int i = 0; i < D; i++){
			for(unsigned int j = 0; j < D; j++){
				int neg = 1;
				if(rng->RandomLongLong()&1)neg = -neg;
				tmp[i][j] = arbitraryprecisioncalculation::mpftoperations::ToMpft(neg * rng->RandomLongLong());
			}
		}
		matrix_cached_ = std::vector<std::vector<mpf_t*> >(D, std::vector<mpf_t*>(D, (mpf_t*) NULL));
		for(unsigned int i = 0; i < D; i++){
			for(unsigned int j = 0; j < D; j++){
				matrix_cached_[i][j] = arbitraryprecisioncalculation::mpftoperations::ToMpft(0.0);
				for(unsigned int k = 0; k < D; k++){
					mpf_t* next1 = arbitraryprecisioncalculation::mpftoperations::Multiply(tmp[i][k], tmp[j][k]);
					mpf_t* next2 = arbitraryprecisioncalculation::mpftoperations::Add(matrix_cached_[i][j], next1);
					arbitraryprecisioncalculation::mpftoperations::ReleaseValue(matrix_cached_[i][j]);
					arbitraryprecisioncalculation::mpftoperations::ReleaseValue(next1);
					matrix_cached_[i][j] = next2;
				}
			}
		}
		for(unsigned int i = 0; i < D; i++){
			for(unsigned int j = 0; j < D; j++){
				arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp[i][j]);
			}
		}
		arbitraryprecisioncalculation::mpftoperations::ChangeNumberOfMpftValuesCached( D * D );
	}
}

mpf_t* RandomPositiveDefiniteSecondDerivative::Eval(const std::vector<mpf_t*> & vec) {
	InitMatrixCached(vec);
	unsigned int D = vec.size();
	mpf_t* res = NULL;
	for(unsigned int i = 0; i < D; i++){
		for(unsigned int j = 0;j < D; j++){
			mpf_t* cur = arbitraryprecisioncalculation::mpftoperations::Multiply(matrix_cached_[i][j], vec[i]);
			mpf_t* next = arbitraryprecisioncalculation::mpftoperations::Multiply(cur, vec[j]);
			arbitraryprecisioncalculation::mpftoperations::ReleaseValue(cur);
			cur = next;
			if(res == NULL)res = cur;
			else {
				next = arbitraryprecisioncalculation::mpftoperations::Add(cur, res);
				arbitraryprecisioncalculation::mpftoperations::ReleaseValue(cur);
				arbitraryprecisioncalculation::mpftoperations::ReleaseValue(res);
				res = next;
			}
		}
	}
	return res;
}

std::string RandomPositiveDefiniteSecondDerivative::GetName(){
	return "RandPosDef";
}

mpf_t* RandomPositiveDefiniteSecondDerivative::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	InitMatrixCached(pos);
	int D = pos.size();
	AssertCondition(D == (int)matrix_cached_.size(), "randomposdef: Initialization failed.");
	mpf_t* B = arbitraryprecisioncalculation::mpftoperations::ToMpft(0.0);
	for(int d2 = 0; d2 < D; d2++)if(d2 != d){
		mpf_t* f = matrix_cached_[d][d2];
		mpf_t* b = arbitraryprecisioncalculation::mpftoperations::Multiply(f,pos[d2]);
		mpf_t* newB = arbitraryprecisioncalculation::mpftoperations::Add(b,B);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(b);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(B);
		B = newB;
	}
	mpf_t* best = arbitraryprecisioncalculation::mpftoperations::Divide(B,matrix_cached_[d][d]);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(B);
	mpf_t* res = arbitraryprecisioncalculation::mpftoperations::Add(best, pos[d]);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(best);
	mpf_t* res_abs = arbitraryprecisioncalculation::mpftoperations::Abs(res);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(res);
	return res_abs;
}

} // namespace highprecisionpso
