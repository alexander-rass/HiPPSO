/**
* @file   function/inclined_plane.cpp
* @author Manuel Schmitt
* @date   July, 2014
* @brief  This file contains the description of the inclined plane functions.
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

#include "function/inclined_plane.h"

#include <sstream>

#include "arbitrary_precision_calculation/operations.h"

namespace highprecisionpso {

// InclinedPlane InclinedPlane InclinedPlane InclinedPlane InclinedPlane InclinedPlane
InclinedPlane::InclinedPlane(){}

mpf_t* InclinedPlane::Eval(const std::vector<mpf_t*> & vec) {
	mpf_t* res = arbitraryprecisioncalculation::vectoroperations::Add(vec);
	mpf_t* tmp = arbitraryprecisioncalculation::mpftoperations::Negate(res);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(res);
	res = tmp;
	return res;
}

std::string InclinedPlane::GetName(){
	std::ostringstream os;
	os << "inclinedPlane";
	return os.str();
}

mpf_t* InclinedPlane::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	return arbitraryprecisioncalculation::mpftoperations::GetPlusInfinity();
}

// InclinedPlaneAsym InclinedPlaneAsym InclinedPlaneAsym InclinedPlaneAsym InclinedPlaneAsym
InclinedPlaneAsym::InclinedPlaneAsym(){}

mpf_t* InclinedPlaneAsym::Eval(const std::vector<mpf_t*> & vec) {
	unsigned int D = vec.size();
	std::vector<double> scalePerDimension(D);
	for(unsigned int i = 0; i < D; i++){
		scalePerDimension[i] = i + 1.0;
	}
	std::vector<mpf_t*> sumUpValues = arbitraryprecisioncalculation::vectoroperations::Multiply(vec, scalePerDimension);
	mpf_t* res = arbitraryprecisioncalculation::vectoroperations::Add(sumUpValues);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(sumUpValues);
	mpf_t* tmp = arbitraryprecisioncalculation::mpftoperations::Negate(res);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(res);
	res = tmp;
	return res;
}

std::string InclinedPlaneAsym::GetName(){
	std::ostringstream os;
	os << "inclinedPlaneAsym";
	return os.str();
}

mpf_t* InclinedPlaneAsym::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	return arbitraryprecisioncalculation::mpftoperations::GetPlusInfinity();
}

} // namespace highprecisionpso
