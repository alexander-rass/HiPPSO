/**
* @file   schwefel.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   January, 2014
* @brief  This file contains two different versions of the schwefel test function.
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

#include "general/operations.h"
#include "schwefel.h"

namespace highprecisionpso {

mpf_t* Schwefel1::Eval(const std::vector<mpf_t*> & vec) {
	unsigned int D = vec.size();
	std::vector<mpf_t*> partialSums(D);
	partialSums[0] = mpftoperations::Clone(vec[0]);
	for(unsigned int i = 1; i < D; i++){
		partialSums[i] = mpftoperations::Add(partialSums[i-1], vec[i]);
	}
	std::vector<mpf_t*> sumUpValues = vectoroperations::Multiply(partialSums, partialSums);
	mpf_t* res = vectoroperations::Add(sumUpValues);
	vectoroperations::ReleaseValues(partialSums);
	vectoroperations::ReleaseValues(sumUpValues);
	return res;
}

std::string Schwefel1::GetName(){
	return "Schwefel";
}

mpf_t* Schwefel1::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	int D = pos.size();
	mpf_t* A = mpftoperations::ToMpft((double)(D-d));
	mpf_t* B = mpftoperations::ToMpft(0.0);
	for(int d2 = 0; d2 < D; d2++)if(d2 != d){
		mpf_t* f = mpftoperations::ToMpft((double)(D-std::max(d,d2)));
		mpf_t* b = mpftoperations::Multiply(f,pos[d2]);
		mpftoperations::ReleaseValue(f);
		mpf_t* newB = mpftoperations::Add(b,B);
		mpftoperations::ReleaseValue(b);
		mpftoperations::ReleaseValue(B);
		B = newB;
	}
	mpf_t* best = mpftoperations::Divide(B,A);
	mpftoperations::ReleaseValue(A);
	mpftoperations::ReleaseValue(B);
	mpf_t* res = mpftoperations::Add(best, pos[d]);
	mpftoperations::ReleaseValue(best);
	mpf_t* res_abs = mpftoperations::Abs(res);
	mpftoperations::ReleaseValue(res);
	return res_abs;
}


Schwefel2::Schwefel2() : Function ( -500.0, 500.0 ) {}

mpf_t* Schwefel2::Eval(const std::vector<mpf_t*> & vec) {
	unsigned int D = vec.size();
	std::vector<mpf_t*> sumUpValues(D);
	for(unsigned int i = 0; i < D; i++){
		mpf_t* x = vec[i];
		mpf_t* absx = mpftoperations::Abs(x);
		mpf_t* sqx = mpftoperations::Sqrt(absx);
		mpf_t* sinx = mpftoperations::Sin(sqx);
		mpf_t* mul = mpftoperations::Multiply(x, sinx);
		sumUpValues[i] = mpftoperations::Negate(mul);
		mpftoperations::ReleaseValue(absx);
		mpftoperations::ReleaseValue(sqx);
		mpftoperations::ReleaseValue(sinx);
		mpftoperations::ReleaseValue(mul);
	}
	mpf_t* res = vectoroperations::Add(sumUpValues);
	vectoroperations::ReleaseValues(sumUpValues);
	return res;
}

std::string Schwefel2::GetName(){
	return "Schw2";
}

} // namespace highprecisionpso
