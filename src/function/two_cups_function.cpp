/**
* @file   two_cups_function.cpp
* @author Manuel Schmitt
* @date   July, 2014
* @brief  This file contains the description of the two cups function.
*
* @copyright
* This project is released under the MIT License (MIT).
*
* @copyright
* The MIT License (MIT)
*
* @copyright
* Copyright (c) 2016 by Friedrich-Alexander-Universität Erlangen-Nürnberg and
* Alexander Raß and Manuel Schmitt
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

#include "function/two_cups_function.h"

#include "arbitrary_precision_calculation/operations.h"

namespace highprecisionpso {

mpf_t* TwoCupsFunction::Eval(const std::vector<mpf_t*> & vec) {
	int D = vec.size();
	mpf_t* res = mpftoperations::ToMpft(0.0);
	mpf_t* vtmp = NULL;
	mpf_t* n1 = mpftoperations::ToMpft(1.0);
	for(int d = 0; d < D; d++){
		mpf_t* cres;
		if (mpftoperations::Compare((vec[d]), 0.0) <= 0){
			vtmp = mpftoperations::Add(vec[d], n1);
			cres = mpftoperations::Pow(vtmp,2);
		}	else {
			vtmp = mpftoperations::Subtract(vec[d], n1);
			cres = mpftoperations::Pow(vtmp,4);
		}
		mpftoperations::ReleaseValue(vtmp);
		vtmp = mpftoperations::Add(cres, res);
		mpftoperations::ReleaseValue(cres);
		mpftoperations::ReleaseValue(res);
		res = vtmp;
		vtmp = NULL;
	}
	mpftoperations::ReleaseValue(n1);
	return res;
}

std::string TwoCupsFunction::GetName(){
	return "twoCups";
}

mpf_t* TwoCupsFunction::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	mpf_t* res = NULL;
	mpf_t* v1 = NULL;
	mpf_t* n1 = mpftoperations::ToMpft(1.0);
	if (mpftoperations::Compare((pos[d]), 0.0) <= 0){
		v1 = mpftoperations::Add(pos[d], n1);
	}	else {
		v1 = mpftoperations::Subtract(pos[d], n1);
	}
	res = mpftoperations::Abs(v1);
	mpftoperations::ReleaseValue(n1);
	mpftoperations::ReleaseValue(v1);
	return res;
}

} // namespace highprecisionpso
