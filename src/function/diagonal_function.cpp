/**
* @file   function/diagonal_function.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   January, 2014
* @brief  This file contains the description of a function with heavy diagonal eigenvector in the second derivative matrix.
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

#include "diagonal_function.h"

#include <sstream>

#include "arbitrary_precision_calculation/operations.h"

namespace highprecisionpso {

// f(x) = scale * (sum x_i)^2 + sum (x_i^2)

DiagonalFunction::DiagonalFunction(double scale):scale_(scale){
}

mpf_t* DiagonalFunction::Eval(const std::vector<mpf_t*> & vec) {
	mpf_t* vecSum = arbitraryprecisioncalculation::vectoroperations::Add(vec);
	mpf_t* vecSum2 = arbitraryprecisioncalculation::mpftoperations::Multiply(vecSum, vecSum);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(vecSum);
	mpf_t* diagPart = arbitraryprecisioncalculation::mpftoperations::Multiply(vecSum2, scale_);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(vecSum2);
	std::vector<mpf_t*> vec2 = arbitraryprecisioncalculation::vectoroperations::Multiply(vec, vec);
	mpf_t* spherePart = arbitraryprecisioncalculation::vectoroperations::Add(vec2);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(vec2);
	mpf_t* res = arbitraryprecisioncalculation::mpftoperations::Add(diagPart, spherePart);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(diagPart);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(spherePart);
	return res;
}

std::string DiagonalFunction::GetName(){
	std::ostringstream os;
	os << "DiagV" << scale_;
	return os.str();
}

mpf_t* DiagonalFunction::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	int D = pos.size();
	mpf_t* B = arbitraryprecisioncalculation::mpftoperations::ToMpft(0.0);
	for(int d2 = 0; d2 < D; d2++)if(d2 != d){
		mpf_t* f = arbitraryprecisioncalculation::mpftoperations::ToMpft(scale_);
		mpf_t* b = arbitraryprecisioncalculation::mpftoperations::Multiply(f,pos[d2]);
		mpf_t* newB = arbitraryprecisioncalculation::mpftoperations::Add(b,B);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(f);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(b);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(B);
		B = newB;
	}
	mpf_t* A = arbitraryprecisioncalculation::mpftoperations::ToMpft(scale_ + 1.0);
	mpf_t* best = arbitraryprecisioncalculation::mpftoperations::Divide(B,A);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(B);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(A);
	mpf_t* res = arbitraryprecisioncalculation::mpftoperations::Add(best, pos[d]);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(best);
	mpf_t* res_abs = arbitraryprecisioncalculation::mpftoperations::Abs(res);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(res);
	return res_abs;
}

} // namespace highprecisionpso
