/**
* @file   function/mono_sphere.cpp
* @author Manuel Schmitt
* @date   September, 2015
* @brief  This file contains the description of the mono sphere function.
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

#include "function/mono_sphere.h"

#include "arbitrary_precision_calculation/operations.h"

namespace highprecisionpso {

MonoSphere::MonoSphere(){}

mpf_t* MonoSphere::Eval(const std::vector<mpf_t*> & vec) {
	mpf_t* res = arbitraryprecisioncalculation::mpftoperations::Multiply(vec[0], vec[0]);
	return res;
}

std::string MonoSphere::GetName(){
	return "MS";
}

mpf_t* MonoSphere::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	if (d==0){
		return arbitraryprecisioncalculation::mpftoperations::Abs(pos[d]);
	} else {
		return arbitraryprecisioncalculation::mpftoperations::ToMpft(0.0);
	}
}

} // namespace highprecisionpso
