/**
* @file   function/rosenbrock.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2015
* @brief  This file contains the description of the rosenbrock function and a moved version.
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

#include "function/rosenbrock.h"

#include "arbitrary_precision_calculation/operations.h"

namespace highprecisionpso {

Rosenbrock::Rosenbrock() : Function ( -30.0, 30.0) {}

mpf_t* eval_rosenbrock(const std::vector<mpf_t*> & vec){
	unsigned int D = vec.size();
	std::vector<mpf_t*> sumUpValues(D-1);
	mpf_t* n1 = arbitraryprecisioncalculation::mpftoperations::ToMpft(1.0);
	mpf_t* n100 = arbitraryprecisioncalculation::mpftoperations::ToMpft(100.0);
	for(unsigned int i = 0; i+1 < D; i++){
		mpf_t* x = vec[i];
		mpf_t* xn = vec[i+1];

		mpf_t* xsq = arbitraryprecisioncalculation::mpftoperations::Multiply(x, x);
		mpf_t* p1 = arbitraryprecisioncalculation::mpftoperations::Subtract(xn, xsq);
		mpf_t* p1sq = arbitraryprecisioncalculation::mpftoperations::Multiply(p1, p1);
		mpf_t* t1 = arbitraryprecisioncalculation::mpftoperations::Multiply(n100, p1sq);

		mpf_t* p2 = arbitraryprecisioncalculation::mpftoperations::Subtract(n1, x);
		mpf_t* t2 = arbitraryprecisioncalculation::mpftoperations::Multiply(p2, p2);

		sumUpValues[i] = arbitraryprecisioncalculation::mpftoperations::Add(t1, t2);

		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(xsq);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(p1);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(p1sq);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(t1);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(p2);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(t2);
	}
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(n1);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(n100);
	mpf_t* res = arbitraryprecisioncalculation::vectoroperations::Add(sumUpValues);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(sumUpValues);
	return res;
}

mpf_t* Rosenbrock::Eval(const std::vector<mpf_t*> & vec) {
	return eval_rosenbrock(vec);
}

std::string Rosenbrock::GetName(){
	return "Rosenbrock";
}


MovedRosenbrock::MovedRosenbrock() : Function ( -31.0, 29.0 ) {}

mpf_t* MovedRosenbrock::Eval(const std::vector<mpf_t*> & vec) {
	std::vector<mpf_t*> help;
	unsigned int D = vec.size();
	mpf_t* n1 = arbitraryprecisioncalculation::mpftoperations::ToMpft(1.0);
	for(unsigned int d = 0; d < D; d++){
		help.push_back(arbitraryprecisioncalculation::mpftoperations::Add(n1, vec[d]));
	}
	mpf_t* res = eval_rosenbrock(help);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(n1);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(help);
	return res;
}

std::string MovedRosenbrock::GetName(){
	return "MovedRosenbrock";
}

} // namespace highprecisionpso
