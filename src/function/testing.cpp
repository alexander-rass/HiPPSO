/**
* @file   testing.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2015
* @brief  This file contains the description of an experimental testing function.
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

#include "arbitrary_precision_calculation/operations.h"
#include "testing.h"

namespace highprecisionpso {

TestingFunction::TestingFunction(){}

mpf_t* TestingFunction::Eval(const std::vector<mpf_t*> & vec) {
	unsigned int D = vec.size();
	std::vector<mpf_t*> values(D, (mpf_t*)NULL);
	for(unsigned int i = 0; i < D; i++){
		values[i] = mpftoperations::Sin(vec[i]);
	}

	// move by constant
#if 1
	{
		mpf_t* half = mpftoperations::ToMpft(0.5);
		mpf_t* move = mpftoperations::Sqrt(half);
		mpftoperations::ReleaseValue(half);
		std::vector<mpf_t*> moveVector = std::vector<mpf_t*>(vec.size(), move);
		std::vector<mpf_t*> tmpVector = vectoroperations::Subtract(values, moveVector);
		vectoroperations::ReleaseValues(values);
		values = tmpVector;
		mpftoperations::ReleaseValue(move);
	}
#endif

	// square result
#if 1
	{
		std::vector<mpf_t*> squares = vectoroperations::Multiply(values, values);
		vectoroperations::ReleaseValues(values);
		values = squares;
	}
#endif

	mpf_t* res = vectoroperations::Add(values);
	vectoroperations::ReleaseValues(values);

	// move by constant
#if 0
	{
		mpf_t* move = mpftoperations::ToMpft((double)D);
		mpf_t* nres = mpftoperations::add(move, res);
		mpftoperations::ReleaseValue(move);
		mpftoperations::ReleaseValue(res);
		res = nres;
	}
#endif
	return res;
}

std::string TestingFunction::GetName(){
	return "Test";
}

} // namespace highprecisionpso
