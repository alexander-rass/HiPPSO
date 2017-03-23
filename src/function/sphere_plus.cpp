/**
* @file   sphere_plus.cpp
* @author Manuel Schmitt
* @date   July, 2014
* @brief  This file contains the description of the sphere plus function.
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

#include "function/sphere_plus.h"

#include "arbitrary_precision_calculation/operations.h"

namespace highprecisionpso {

mpf_t* SpherePlus::Eval(const std::vector<mpf_t*> & vec) {
	unsigned int D = vec.size();
	mpf_t* res = NULL;
	for(unsigned int d = 0; d < D; d++){
		if (mpftoperations::Compare((vec[d]), 0.0) < 0){
			return mpftoperations::GetPlusInfinity();
		}
	}
	mpf_t* cursum = mpftoperations::ToMpft(0.0);
	for(unsigned int i = 0; i < D; i++){
		mpf_t* tmp = mpftoperations::Multiply(vec[i], vec[i]);
		mpf_t* tmp2 = mpftoperations::Add(cursum, tmp);
		mpftoperations::ReleaseValue(tmp);
		mpftoperations::ReleaseValue(cursum);
		cursum = tmp2;
	}
	res = cursum;
	return res;
}

std::string SpherePlus::GetName(){
	return "spherePlus";
}

mpf_t* SpherePlus::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	return mpftoperations::Abs(pos[d]);
}

} // namespace highprecisionpso
