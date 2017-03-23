/**
* @file   norm.cpp 
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains the description of various norm functions.
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

#include "function/norm.h"

#include <sstream>

#include "arbitrary_precision_calculation/operations.h"
#include "general/check_condition.h"

namespace highprecisionpso {

// Norm1 Norm1 Norm1 Norm1 Norm1 Norm1 Norm1 Norm1 Norm1 Norm1 Norm1 Norm1
mpf_t* Norm1::Eval(const std::vector<mpf_t*> & vec) {
	std::vector<mpf_t*> vecAbs(vec.size());
	for(unsigned int i = 0; i < vec.size(); i++){
		vecAbs[i] = mpftoperations::Abs(vec[i]);
	}
	mpf_t* res = vectoroperations::Add(vecAbs);
	vectoroperations::ReleaseValues(vecAbs);
	return res;
}

std::string Norm1::GetName(){
	return "1N";
}

mpf_t* Norm1::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	return mpftoperations::Abs(pos[d]);
}

// NormOO NormOO NormOO NormOO NormOO NormOO NormOO NormOO NormOO NormOO
mpf_t* NormInfinity::Eval(const std::vector<mpf_t*> & vec) {
	mpf_t* res = NULL;
	for(unsigned int i = 0; i < vec.size(); i++){
		if(res == NULL){
			res = mpftoperations::Abs(vec[i]);
		} else {
			mpf_t* v1 = res;
			mpf_t* v2 = mpftoperations::Abs(vec[i]);
			res = mpftoperations::Max(v1, v2);
			mpftoperations::ReleaseValue(v1);
			mpftoperations::ReleaseValue(v2);
		}
	}
	return res;
}

std::string NormInfinity::GetName(){
	return "ooN";
}

mpf_t* NormInfinity::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	return mpftoperations::Abs(pos[d]);
}

// Norm2pk Norm2pk Norm2pk Norm2pk Norm2pk Norm2pk Norm2pk Norm2pk Norm2pk
Norm2PowerK::Norm2PowerK(int exponent):exponent_(exponent){
	AssertCondition(exponent_ > 0, "Each Norm2PowerK object needs to be initialized with positive exponent");
}

mpf_t* Norm2PowerK::Eval(const std::vector<mpf_t*> & vec) {
	std::vector<mpf_t*> curVec = vectoroperations::Clone(vec);
	for(int j = 0; j < exponent_; j++){
		std::vector<mpf_t*> nextVec = vectoroperations::Multiply(curVec, curVec);
		vectoroperations::ReleaseValues(curVec);
		curVec = nextVec;
	}
	mpf_t* res = vectoroperations::Add(curVec);
	vectoroperations::ReleaseValues(curVec);
	return res;
}

std::string Norm2PowerK::GetName(){
	std::ostringstream os;
	if(exponent_ <= 3) {
		os << (1<<exponent_);
	} else {
		os << "2p" << exponent_;
	}
	os << "N";
	return os.str();
}

mpf_t* Norm2PowerK::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	return mpftoperations::Abs(pos[d]);
}

Norm2::Norm2():Norm2PowerK(1){}
Norm4::Norm4():Norm2PowerK(2){}
Norm8::Norm8():Norm2PowerK(3){}
Sphere::Sphere():Norm2PowerK(1){}

// Norm1pl2pmk Norm1pl2pmk Norm1pl2pmk Norm1pl2pmk Norm1pl2pmk Norm1pl2pmk
Norm1Plus2PowerMinusK::Norm1Plus2PowerMinusK(int exponent):exponent_(exponent){
	AssertCondition(exponent_ > 0, "Each Norm1Plus2PowerMinusK object needs to be initialized with positive exponent");
}

mpf_t* Norm1Plus2PowerMinusK::Eval(const std::vector<mpf_t*> & vec) {
	mpf_t* res = NULL;
	for(unsigned int i = 0; i < vec.size(); i++){
		mpf_t* cres = mpftoperations::Abs(vec[i]);
		mpf_t* v1 = mpftoperations::Sqrt(cres);
		mpf_t* v2;
		for(int j = 1; j < exponent_; j++){
			v2 = mpftoperations::Sqrt(v1);
			mpftoperations::ReleaseValue(v1);
			v1 = v2;
		}
		v2 = cres;
		cres = mpftoperations::Multiply(v1, v2);
		mpftoperations::ReleaseValue(v1);
		mpftoperations::ReleaseValue(v2);
		if(res == NULL){
			res = cres;
		} else {
			v1 = cres;
			v2 = res;
			res = mpftoperations::Add(v1, v2);
			mpftoperations::ReleaseValue(v1);
			mpftoperations::ReleaseValue(v2);
		}
	}
	return res;
}

std::string Norm1Plus2PowerMinusK::GetName(){
	std::ostringstream os;
	os << "1+2p-" << exponent_ << "N";
	return os.str();
}

mpf_t* Norm1Plus2PowerMinusK::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	return mpftoperations::Abs(pos[d]);
}

} // namespace highprecisionpso
