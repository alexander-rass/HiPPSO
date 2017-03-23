/**
* @file   rastrigin.cpp 
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2015
* @brief  This file contains the description of the rastrigin function.
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

#include "function/rastrigin.h"

#include "arbitrary_precision_calculation/operations.h"

namespace highprecisionpso {

Rastrigin::Rastrigin():Function( -5.12, 5.12 ){}

mpf_t* Rastrigin::Eval(const std::vector<mpf_t*> & vec) {
	unsigned int D = vec.size();
	std::vector<mpf_t*> sumUpValues(D);
	mpf_t* pi = mpftoperations::GetPi();
	mpf_t* pi_n2 = mpftoperations::Add(pi, pi);
	mpftoperations::ReleaseValue(pi);
	mpf_t* n10 = mpftoperations::ToMpft(10.0);
	for(unsigned int i = 0; i < D; i++){
		mpf_t* x = vec[i];

		mpf_t* t1 = mpftoperations::Multiply(x, x);
		mpf_t* icos = mpftoperations::Multiply(x, pi_n2);
		mpf_t* tcos = mpftoperations::Cos(icos);
		mpf_t* t2 = mpftoperations::Multiply(tcos, n10);

		mpf_t* t12 = mpftoperations::Subtract(t1, t2);

		sumUpValues[i] = mpftoperations::Add(t12, n10);

		mpftoperations::ReleaseValue(t1);
		mpftoperations::ReleaseValue(icos);
		mpftoperations::ReleaseValue(tcos);
		mpftoperations::ReleaseValue(t2);
		mpftoperations::ReleaseValue(t12);
	}
	mpftoperations::ReleaseValue(pi_n2);
	mpftoperations::ReleaseValue(n10);
	mpf_t* res = vectoroperations::Add(sumUpValues);
	vectoroperations::ReleaseValues(sumUpValues);
	return res;
}

std::string Rastrigin::GetName(){
	return "Rastrigin";
}

} // namespace highprecisionpso
