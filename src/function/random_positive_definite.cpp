/**
* @file   random_positive_definite.cpp 
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

#include <general/random_number_generator.h>
#include "function/random_positive_definite.h"

#include "general/operations.h"
#include "general/parse.h"
#include "general/check_condition.h"

namespace highprecisionpso {

RandomPositiveDefiniteSecondDerivative::RandomPositiveDefiniteSecondDerivative(std::vector<std::string> rng_description):rng_description_(rng_description){
	matrix_cached_.clear();
	matrix_cached_precision_ = 1;
}

RandomPositiveDefiniteSecondDerivative::~RandomPositiveDefiniteSecondDerivative(){
	for(unsigned int i = 0; i < matrix_cached_.size(); i++){
		mpftoperations::ChangeNumberOfMpftValuesCached( -(int)(matrix_cached_[i].size()) );
		vectoroperations::ReleaseValues(matrix_cached_[i]);
		matrix_cached_[i].clear();
	}
}

void RandomPositiveDefiniteSecondDerivative::InitMatrixCached(const std::vector<mpf_t*> & vec){
	unsigned int D = vec.size();
	if(matrix_cached_.size() != D || matrix_cached_precision_ != mpf_get_default_prec()){
		matrix_cached_precision_ = mpf_get_default_prec();
		for(unsigned int i = 0; i < matrix_cached_.size(); i++){
			mpftoperations::ChangeNumberOfMpftValuesCached( -(int)(matrix_cached_[i].size()) );
			vectoroperations::ReleaseValues(matrix_cached_[i]);
		}

		RandomNumberGenerator* rng = NULL;
		{
			unsigned int parsed = 0;
			rng = parse::ParseRandomNumberGenerator(rng_description_, parsed);
		}
		std::vector<std::vector<mpf_t*> > tmp(D, std::vector<mpf_t*>(D, (mpf_t*) NULL));
		for(unsigned int i = 0; i < D; i++){
			for(unsigned int j = 0; j < D; j++){
				int neg = 1;
				if(rng->RandomLongLong()&1)neg = -neg;
				tmp[i][j] = mpftoperations::ToMpft(neg * rng->RandomLongLong());
			}
		}
		matrix_cached_ = std::vector<std::vector<mpf_t*> >(D, std::vector<mpf_t*>(D, (mpf_t*) NULL));
		for(unsigned int i = 0; i < D; i++){
			for(unsigned int j = 0; j < D; j++){
				matrix_cached_[i][j] = mpftoperations::ToMpft(0.0);
				for(unsigned int k = 0; k < D; k++){
					mpf_t* next1 = mpftoperations::Multiply(tmp[i][k], tmp[j][k]);
					mpf_t* next2 = mpftoperations::Add(matrix_cached_[i][j], next1);
					mpftoperations::ReleaseValue(matrix_cached_[i][j]);
					mpftoperations::ReleaseValue(next1);
					matrix_cached_[i][j] = next2;
				}
			}
		}
		for(unsigned int i = 0; i < D; i++){
			for(unsigned int j = 0; j < D; j++){
				mpftoperations::ReleaseValue(tmp[i][j]);
			}
		}
		mpftoperations::ChangeNumberOfMpftValuesCached( D * D );
	}
}

mpf_t* RandomPositiveDefiniteSecondDerivative::Eval(const std::vector<mpf_t*> & vec) {
	InitMatrixCached(vec);
	unsigned int D = vec.size();
	mpf_t* res = NULL;
	for(unsigned int i = 0; i < D; i++){
		for(unsigned int j = 0;j < D; j++){
			mpf_t* cur = mpftoperations::Multiply(matrix_cached_[i][j], vec[i]);
			mpf_t* next = mpftoperations::Multiply(cur, vec[j]);
			mpftoperations::ReleaseValue(cur);
			cur = next;
			if(res == NULL)res = cur;
			else {
				next = mpftoperations::Add(cur, res);
				mpftoperations::ReleaseValue(cur);
				mpftoperations::ReleaseValue(res);
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
	mpf_t* B = mpftoperations::ToMpft(0.0);
	for(int d2 = 0; d2 < D; d2++)if(d2 != d){
		mpf_t* f = matrix_cached_[d][d2];
		mpf_t* b = mpftoperations::Multiply(f,pos[d2]);
		mpf_t* newB = mpftoperations::Add(b,B);
		mpftoperations::ReleaseValue(b);
		mpftoperations::ReleaseValue(B);
		B = newB;
	}
	mpf_t* best = mpftoperations::Divide(B,matrix_cached_[d][d]);
	mpftoperations::ReleaseValue(B);
	mpf_t* res = mpftoperations::Add(best, pos[d]);
	mpftoperations::ReleaseValue(best);
	mpf_t* res_abs = mpftoperations::Abs(res);
	mpftoperations::ReleaseValue(res);
	return res_abs;
}

} // namespace highprecisionpso
