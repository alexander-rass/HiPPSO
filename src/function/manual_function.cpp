/**
* @file   manual_function.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains abstract classes for function generation.
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

#include "function/manual_function.h"

#include <sstream>

#include "general/operations.h"

namespace highprecisionpso {

FunctionReduceOperator::FunctionReduceOperator(VectorReduceOperation* vector_reduce_operation, SpecificFunction* specific_function):vector_reduce_operation_(vector_reduce_operation), specific_function_(specific_function){}

mpf_t* FunctionReduceOperator::Eval(const std::vector<mpf_t*> & vec){
	std::vector<mpf_t*> tmp = specific_function_->Eval(vec);
	mpf_t* res = vector_reduce_operation_->Evaluate(tmp);
	vectoroperations::ReleaseValues(tmp);
	return res;
}

std::string FunctionReduceOperator::GetName(){
	std::ostringstream os;
	os << vector_reduce_operation_->GetName() << specific_function_->GetName();
	return os.str();
}

OperatedFunction::OperatedFunction(Operation* operation, Function* function):operation_(operation), function_(function){}

mpf_t* OperatedFunction::Eval(const std::vector<mpf_t*> & vec) {
	mpf_t* tmp = function_->Eval(vec);
	mpf_t* res = operation_->Operate(tmp);
	mpftoperations::ReleaseValue(tmp);
	return res;
}

std::string OperatedFunction::GetName(){
	std::ostringstream os;
	os << operation_->GetName() << function_->GetName();
	return os.str();
}

CombineFunction::CombineFunction(PairReduceOperation* operation, Function* operator1, Function* operator2):operation_(operation), operator1_(operator1), operator2_(operator2){}

mpf_t* CombineFunction::Eval(const std::vector<mpf_t*> & vec){
	mpf_t* res1 = operator1_->Eval(vec);
	mpf_t* res2 = operator2_->Eval(vec);
	mpf_t* res = operation_->Evaluate(res1, res2);
	mpftoperations::ReleaseValue(res1);
	mpftoperations::ReleaseValue(res2);
	return res;
}

std::string CombineFunction::GetName(){
	std::ostringstream os;
	os << operation_->GetName() << operator1_->GetName() << operator2_->GetName();
	return os.str();
}

ConstantFunction::ConstantFunction(ConstantEvaluation* constantEvaluation):constant_evaluation_(constantEvaluation){}

mpf_t* ConstantFunction::Eval(const std::vector<mpf_t*> & vec){
	return  constant_evaluation_->Evaluate();
}

std::string ConstantFunction::GetName(){
	return constant_evaluation_->GetName();
}

} // namespace highprecisionpso
