/**
* @file   specific_functions.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains specific functions.
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

#include "function/specific_functions.h"

#include <sstream>

#include "general/check_condition.h"
#include "general/operations.h"

namespace highprecisionpso {

ConstantSpecificFunction::ConstantSpecificFunction(ConstantEvaluation* constantEvaluation):constant_evaluation_(constantEvaluation){}

std::vector<mpf_t*> ConstantSpecificFunction::Eval(const std::vector<mpf_t*> & vec){
	mpf_t* tmp = constant_evaluation_->Evaluate();
	std::vector<mpf_t*> result = vectoroperations::GetConstantVector(vec.size(), tmp);
	mpftoperations::ReleaseValue(tmp);
	return result;
}

std::string ConstantSpecificFunction::GetName(){
	return constant_evaluation_->GetName();
}

std::vector<mpf_t*> IdentitySpecificFunction::Eval(const std::vector<mpf_t*> & vec){
	return vectoroperations::Clone(vec);
}

std::string IdentitySpecificFunction::GetName(){
	return "x";
}

OperatedSpecificFunction::OperatedSpecificFunction(Operation* operation, SpecificFunction* specificFunction):operation_(operation),specific_function_(specificFunction){}

std::vector<mpf_t*> OperatedSpecificFunction::Eval(const std::vector<mpf_t*> & vec){
	std::vector<mpf_t*> res1 = specific_function_->Eval(vec);
	std::vector<mpf_t*> res;
	for(unsigned int i = 0; i < res1.size(); i++){
		res.push_back(operation_->Operate(res1[i]));
	}
	vectoroperations::ReleaseValues(res1);
	return res;
}

std::string OperatedSpecificFunction::GetName(){
	std::ostringstream os;
	os << operation_->GetName() << specific_function_->GetName();
	return os.str();
}

CombineSpecificFunction::CombineSpecificFunction(PairCombinationOperation* operation, SpecificFunction* operator1, SpecificFunction* operator2):operation_(operation), operator1_(operator1), operator2_(operator2){}

std::vector<mpf_t*> CombineSpecificFunction::Eval(const std::vector<mpf_t*> & vec){
	std::vector<mpf_t*> res1 = operator1_->Eval(vec);
	std::vector<mpf_t*> res2 = operator2_->Eval(vec);
	std::vector<mpf_t*> res;
	AssertCondition(res1.size() == res2.size(), "A combine operation is specified which receives vectors with different length. Please check your function definition.");
	for(unsigned int i = 0; i < res1.size(); i++){
		res.push_back(operation_->Evaluate(res1[i], res2[i]));
	}
	vectoroperations::ReleaseValues(res1);
	vectoroperations::ReleaseValues(res2);
	return res;
}

std::string CombineSpecificFunction::GetName(){
	std::ostringstream os;
	os << operation_->GetName() << operator1_->GetName() << operator2_->GetName();
	return os.str();
}

} // namespace highprecisionpso
