/**
* @file   function/sorting_function.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   August, 2015
* @brief  This file contains the description of a function which checks how sorted the position entries are.
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

#include "function/sorting_function.h"

#include "arbitrary_precision_calculation/operations.h"

namespace highprecisionpso {

mpf_t* SortingFunction::Eval(const std::vector<mpf_t*> & vec) {
	mpf_t* result = arbitraryprecisioncalculation::mpftoperations::ToMpft(0.0);
	int res = 0;
	for(unsigned int i = 0; i < vec.size(); i++){
		for(unsigned int j = 0; j < i; j++){
			if(arbitraryprecisioncalculation::mpftoperations::Compare((vec[i]), (vec[j])) <= 0){
				++res;
			}
		}
	}
	mpf_t* tmp1 = arbitraryprecisioncalculation::mpftoperations::ToMpft((double)res);
	mpf_t* tmp2 = arbitraryprecisioncalculation::mpftoperations::Add(tmp1, result);
	std::swap(result, tmp2);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp1);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp2);
	return result;
}

std::string SortingFunction::GetName(){
	return "Sort";
}

mpf_t* SortingFunction::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	return arbitraryprecisioncalculation::mpftoperations::ToMpft(0.0);
}

} // namespace highprecisionpso
