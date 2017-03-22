/**
* @file   general_objects.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file contains various general objects which can be used by or contained in other objects.
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

#include "general/general_objects.h"

#include <sstream>

#include "general/check_condition.h"
#include "general/configuration.h"
#include "general/operations.h"

namespace highprecisionpso {

ProgramVersion::ProgramVersion(int version, int version_minor, int version_patch_level):version(version), version_minor(version_minor), version_patch_level(version_patch_level){}
ProgramVersion::ProgramVersion(std::string version_string){
	int count_dots = 0;
	int digits = 0;
	bool ok = true;
	for(unsigned int i = 0; i < version_string.size(); i++){
		if(version_string[i] == '.'){
			++count_dots;
			version_string[i] = ' ';
			if(digits == 0 || digits > 9){
				ok = false;
			}
			digits = 0;
		} else if(version_string[i] >= '0' || version_string[i] <= '9'){
			++digits;
		} else {
			ok = false;
		}
	}
	if(digits == 0 || digits > 9)ok = false;
	if(!ok){
		version = -1;
		version_minor = -1;
		version_patch_level = -1;
	} else {
		std::istringstream is(version_string);
		is >> version >> version_minor >> version_patch_level;
	}
}
std::string ProgramVersion::GetCompleteVersion() const {
	std::ostringstream os;
	os << version << "." << version_minor << "." << version_patch_level;
	return os.str();
}

bool operator>(const ProgramVersion & a, const ProgramVersion & b){
	return b < a;
}

bool operator<=(const ProgramVersion & a, const ProgramVersion & b){
	return !(a > b);
}

bool operator>=(const ProgramVersion & a, const ProgramVersion & b){
	return !(a < b);
}

bool operator==(const ProgramVersion & a, const ProgramVersion & b){
	return (a <= b) && (a >= b);
}

bool operator<(const ProgramVersion & a, const ProgramVersion & b){
	if(a.version != b.version)return a.version < b.version;
	if(a.version_minor != b.version_minor) return a.version_minor < b.version_minor;
	return a.version_patch_level < b.version_patch_level;
}

SpecificIdReduceOperation::SpecificIdReduceOperation (int id):id_(id){}

mpf_t* SpecificIdReduceOperation::Evaluate(const std::vector<mpf_t*> & vec){
	AssertCondition(id_ >= 0 && id_ < (int) vec.size(), "Id of reduce operation specific is out of range.");
	return mpftoperations::Clone(vec[id_]);
}

std::string SpecificIdReduceOperation::GetName(){
	std::ostringstream os;
	os << "Id" << id_;
	return os.str();
}

IncreasingOrderNthObjectReduceOperation::IncreasingOrderNthObjectReduceOperation (int order_id): order_id_(order_id){}

mpf_t* IncreasingOrderNthObjectReduceOperation::Evaluate(const std::vector<mpf_t*> & vec){
	std::vector<mpf_t*> data = vectoroperations::Clone(vec);
	vectoroperations::Sort(&data);
	AssertCondition(order_id_ >= 0 && order_id_ < (int) data.size(), "Id of reduce operation increasingOrderNthObject is out of range.");
	mpf_t* result = mpftoperations::Clone(data[order_id_]);
	vectoroperations::ReleaseValues(data);
	return result;
}

std::string IncreasingOrderNthObjectReduceOperation::GetName(){
	std::ostringstream os;
	os << "Id" << order_id_ << "Sorted";
	return os.str();
}

ArithmeticAverageReduceOperation::ArithmeticAverageReduceOperation(){}

mpf_t* ArithmeticAverageReduceOperation::Evaluate(const std::vector<mpf_t*> & vec){
	mpf_t* sum = vectoroperations::Add(vec);
	mpf_t* dividend = mpftoperations::ToMpft((unsigned int)vec.size());
	mpf_t* result = mpftoperations::Divide(sum, dividend);
	mpftoperations::ReleaseValue(sum);
	mpftoperations::ReleaseValue(dividend);
	return result;
}

std::string ArithmeticAverageReduceOperation::GetName(){
	std::ostringstream os;
	os << "ArithAv";
	return os.str();
}

GeometricAverageReduceOperation::GeometricAverageReduceOperation(){}

mpf_t* GeometricAverageReduceOperation::Evaluate(const std::vector<mpf_t*> & vec){
	mpf_t* product = mpftoperations::ToMpft(1.0);
	for( unsigned int i = 0; i < vec.size(); i++ ) {
		mpf_t* tmp = mpftoperations::Multiply(product, vec[i]);
		std::swap(tmp, product);
		mpftoperations::ReleaseValue(tmp);
	}
	mpf_t* one = mpftoperations::ToMpft(1.0);
	mpf_t* exponent_denominator = mpftoperations::ToMpft((unsigned int)vec.size());
	mpf_t* exponent = mpftoperations::Divide(one, exponent_denominator);
	mpf_t* result= mpftoperations::Pow(product, exponent);
	mpftoperations::ReleaseValue(one);
	mpftoperations::ReleaseValue(exponent_denominator);
	mpftoperations::ReleaseValue(exponent);
	mpftoperations::ReleaseValue(product);
	return result;
}

std::string GeometricAverageReduceOperation::GetName() {
	std::ostringstream os;
	os << "GeomAv";
	return os.str();
}

SumReduceOperation::SumReduceOperation(){}

mpf_t* SumReduceOperation::Evaluate(const std::vector<mpf_t*> & vec){
	return vectoroperations::Add(vec);
}

std::string SumReduceOperation::GetName(){
	std::ostringstream os;
	os << "Sum";
	return os.str();
}

ProductReduceOperation::ProductReduceOperation(){}

mpf_t* ProductReduceOperation::Evaluate(const std::vector<mpf_t*> & vec){
	mpf_t* product = mpftoperations::ToMpft(1.0);
	for( unsigned int i = 0; i < vec.size(); i++ ) {
		mpf_t* tmp = mpftoperations::Multiply(product, vec[i]);
		std::swap(tmp, product);
		mpftoperations::ReleaseValue(tmp);
	}
	return product;
}

std::string ProductReduceOperation::GetName(){
	std::ostringstream os;
	os << "GeomAv";
	return os.str();
}

MaximalValueReduceOperation::MaximalValueReduceOperation(){}

mpf_t* MaximalValueReduceOperation::Evaluate(const std::vector<mpf_t*> & vec){
	mpf_t* res = mpftoperations::GetMinusInfinity();
	for( unsigned int i = 0; i < vec.size(); i++ ){
		mpf_t* next_res = mpftoperations::Max(res, vec[i]);
		mpftoperations::ReleaseValue(res);
		res = next_res;
	}
	return res;
}

std::string MaximalValueReduceOperation::GetName(){
	std::ostringstream os;
	os << "Max";
	return os.str();
}

MinimalValueReduceOperation::MinimalValueReduceOperation(){}

mpf_t* MinimalValueReduceOperation::Evaluate(const std::vector<mpf_t*> & vec){
	mpf_t* res = mpftoperations::GetPlusInfinity();
	for( unsigned int i = 0; i < vec.size(); i++ ){
		mpf_t* next_res = mpftoperations::Min(res, vec[i]);
		mpftoperations::ReleaseValue(res);
		res = next_res;
	}
	return res;
}

std::string MinimalValueReduceOperation::GetName(){
	std::ostringstream os;
	os << "Min";
	return os.str();
}


FunctionEvaluationReduceOperation::FunctionEvaluationReduceOperation(Function* function):function_(function){}

mpf_t* FunctionEvaluationReduceOperation::Evaluate(const std::vector<mpf_t*> & vec){
	return function_->Eval(vec);
}

std::string FunctionEvaluationReduceOperation::GetName(){
	std::ostringstream os;
	os << "FuncEval" << function_->GetName();
	return os.str();
}

ObjectiveFunctionEvaluationReduceOperation::ObjectiveFunctionEvaluationReduceOperation(){}

mpf_t* ObjectiveFunctionEvaluationReduceOperation::Evaluate(const std::vector<mpf_t*> & vec){
	return configuration::g_function->Evaluate(vec);
}

std::string ObjectiveFunctionEvaluationReduceOperation::GetName(){
	std::ostringstream os;
	os << "ObFunc" << configuration::g_function->GetName();
	return os.str();
}





mpf_t* AddOperation::Evaluate(mpf_t* a, mpf_t* b){
	return mpftoperations::Add(a,b);
}
std::string AddOperation::GetName(){
	return "Add";
}

mpf_t* SubtractOperation::Evaluate(mpf_t* a, mpf_t* b){
	return mpftoperations::Subtract(a,b);
}
std::string SubtractOperation::GetName(){
	return "Sub";
}

mpf_t* MultiplyOperation::Evaluate(mpf_t* a, mpf_t* b){
	return mpftoperations::Multiply(a,b);
}
std::string MultiplyOperation::GetName(){
	return "Mul";
}

mpf_t* DivideOperation::Evaluate(mpf_t* a, mpf_t* b){
	return mpftoperations::Divide(a,b);
}
std::string DivideOperation::GetName(){
	return "Div";
}

mpf_t* MinOperation::Evaluate(mpf_t* a, mpf_t* b){
	return mpftoperations::Min(a,b);
}
std::string MinOperation::GetName(){
	return "Min";
}

mpf_t* MaxOperation::Evaluate(mpf_t* a, mpf_t* b){
	return mpftoperations::Max(a,b);
}
std::string MaxOperation::GetName(){
	return "Max";
}


DoubleConstantEvaluation::DoubleConstantEvaluation(double v):value_(v){}

mpf_t* DoubleConstantEvaluation::Evaluate(){
	return mpftoperations::ToMpft(value_);
}

std::string DoubleConstantEvaluation::GetName(){
	std::ostringstream os;
	os << value_;
	return os.str();
}

mpf_t* EConstantEvaluation::Evaluate(){
	return mpftoperations::GetE();
}

std::string EConstantEvaluation::GetName(){
	return "E";
}

mpf_t* PiConstantEvaluation::Evaluate(){
	return mpftoperations::GetPi();
}

std::string PiConstantEvaluation::GetName(){
	return "Pi";
}

mpf_t* PlusInfinityConstantEvaluation::Evaluate(){
	return mpftoperations::GetPlusInfinity();
}

std::string PlusInfinityConstantEvaluation::GetName(){
	return "oo";
}

mpf_t* MinusInfinityConstantEvaluation::Evaluate(){
	return mpftoperations::GetMinusInfinity();
}

std::string MinusInfinityConstantEvaluation::GetName(){
	return "-oo";
}

mpf_t* SqrtOperation::Operate( mpf_t* v ){
	return mpftoperations::Sqrt( v );
}

std::string SqrtOperation::GetName(){
	return "Sqrt";
}

PowOperation::PowOperation(double exponent): exponent_(exponent){}

mpf_t* PowOperation::Operate( mpf_t* v ){
	return mpftoperations::Pow( v, exponent_ );
}

std::string PowOperation::GetName(){
	std::ostringstream os;
	os << "Pow" << exponent_;
	return os.str();
}

mpf_t* Log2Operation::Operate( mpf_t* v ){
	mpf_t* two = mpftoperations::ToMpft(2.0);
	mpf_t* ln2 = mpftoperations::LogE(two);
	mpf_t* lnv = mpftoperations::LogE(v);
	mpf_t* res = mpftoperations::Divide(lnv, ln2);
	mpftoperations::ReleaseValue(two);
	mpftoperations::ReleaseValue(ln2);
	mpftoperations::ReleaseValue(lnv);
	return res;
}

std::string Log2Operation::GetName(){
	return "Log2";
}

mpf_t* Log2DoubleOperation::Operate( mpf_t* v ){
	return mpftoperations::ToMpft(mpftoperations::Log2Double(v));
}

std::string Log2DoubleOperation::GetName(){
	return "Log2dbl";
}

mpf_t* AbsOperation::Operate( mpf_t* v ){
	return mpftoperations::Abs(v);
}

std::string AbsOperation::GetName(){
	return "Abs";
}

mpf_t* ExpOperation::Operate( mpf_t* v ){
	return mpftoperations::Exp(v);
}

std::string ExpOperation::GetName(){
	return "Exp";
}

mpf_t* SinOperation::Operate( mpf_t* v ){
	return mpftoperations::Sin(v);
}

std::string SinOperation::GetName(){
	return "Sin";
}

mpf_t* CosOperation::Operate( mpf_t* v ){
	return mpftoperations::Cos(v);
}

std::string CosOperation::GetName(){
	return "Cos";
}

mpf_t* TanOperation::Operate( mpf_t* v ){
	return mpftoperations::Tan(v);
}

std::string TanOperation::GetName(){
	return "Tan";
}

mpf_t* ArcsinOperation::Operate( mpf_t* v ){
	return mpftoperations::Arcsin(v);
}

std::string ArcsinOperation::GetName(){
	return "Arcsin";
}

mpf_t* ArccosOperation::Operate( mpf_t* v ){
	return mpftoperations::Arccos(v);
}

std::string ArccosOperation::GetName(){
	return "Arccos";
}

mpf_t* ArctanOperation::Operate( mpf_t* v ){
	return mpftoperations::Arctan(v);
}

std::string ArctanOperation::GetName(){
	return "Arctan";
}

mpf_t* LogEOperation::Operate( mpf_t* v ){
	return mpftoperations::LogE(v);
}

std::string LogEOperation::GetName(){
	return "LogE";
}

std::vector<mpf_t*> DistTo1DOptimumOperation::Operate(std::vector<mpf_t*> pos){
	std::vector<mpf_t*> res;
	for(unsigned int d = 0; d < pos.size(); d++) {
		res.push_back(configuration::g_function->DistanceTo1DLocalOptimum(pos, d));
	}
	return res;
}

std::string DistTo1DOptimumOperation::GetName(){
	return "Dist1DOpt";
}

} // namespace highprecisionpso
