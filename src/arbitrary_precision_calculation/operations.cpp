/**
* @file   arbitrary_precision_calculation/operations.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains a large set of operations for calculations with the mpf_t data type.
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

#include <arbitrary_precision_calculation/random_number_generator.h>
#include "arbitrary_precision_calculation/operations.h"

#include <algorithm>
#include <fstream>
#include <queue>
#include <sstream>
#include <string.h>
#include <unordered_set>
#include <cmath>

#include "arbitrary_precision_calculation/check_condition.h"
#include "arbitrary_precision_calculation/configuration.h"

// If ARBITRARY_PRECISION_CALCULATION_OPERATIONS_CPP_USE_TAYLOR_APPROXIMATION_FOR_TRIGONOMETRC_FUNCTIONS_
// is set to one then taylor approximation is used for trigonometric functions.
// If it is set to zero then the CORDIC algorithm is used.
// Here it is set to one, because even for large precision values the taylor approximation seems to be faster.
#define ARBITRARY_PRECISION_CALCULATION_OPERATIONS_CPP_USE_TAYLOR_APPROXIMATION_FOR_TRIGONOMETRC_FUNCTIONS_ 1

namespace arbitraryprecisioncalculation {
namespace vectoroperations {

std::vector<mpf_t*> Add(const std::vector<mpf_t*> & a, const std::vector<mpf_t*> & b) {
	AssertCondition(a.size() == b.size(), "Addition of vectors with different size has been executed.");
	std::vector<mpf_t*> res(a.size(), (mpf_t*)NULL);
	for (unsigned int i = 0; i < a.size(); i++) {
		res[i] = mpftoperations::Add(a[i], b[i]);
	}
	return res;
}

std::vector<mpf_t*> Subtract(const std::vector<mpf_t*> & a, const std::vector<mpf_t*> & b) {
	AssertCondition(a.size() == b.size(), "Subtraction of vectors with different size has been executed.");
	std::vector<mpf_t*> res(a.size(), (mpf_t*)NULL);
	for (unsigned int i = 0; i < a.size(); i++) {
		res[i] = mpftoperations::Subtract(a[i], b[i]);
	}
	return res;
}

std::vector<mpf_t*> Divide(const std::vector<mpf_t*> & a, const std::vector<mpf_t*> & b) {
	AssertCondition(a.size() == b.size(), "Division of vectors with different size has been executed.");
	std::vector<mpf_t*> res(a.size(), (mpf_t*)NULL);
	for (unsigned int i = 0; i < a.size(); i++) {
		res[i] = mpftoperations::Divide(a[i], b[i]);
	}
	return res;
}

std::vector<mpf_t*> Multiply(const std::vector<mpf_t*> & a, const std::vector<mpf_t*> & b) {
	AssertCondition(a.size() == b.size(), "Multiplication of vectors with different size has been executed.");
	std::vector<mpf_t*> res(a.size(), (mpf_t*)NULL);
	for (unsigned int i = 0; i < a.size(); i++) {
		res[i] = mpftoperations::Multiply(a[i], b[i]);
	}
	return res;
}

std::vector<mpf_t*> Multiply(const std::vector<mpf_t*> & a, const std::vector<double> & b) {
	AssertCondition(a.size() == b.size(), "Multiplication of vectors with different size has been executed.");
	std::vector<mpf_t*> bvec(a.size(), (mpf_t*)NULL);
	for (unsigned int i = 0; i < a.size(); i++) {
		bvec[i] = mpftoperations::ToMpft(b[i]);
	}
	std::vector<mpf_t*> res = vectoroperations::Multiply(a, bvec);
	vectoroperations::ReleaseValues(bvec);
	return res;
}

std::vector<mpf_t*> Multiply(const std::vector<mpf_t*> & a, const double & b) {
	mpf_t* mpf_t_b = mpftoperations::ToMpft(b);
	std::vector<mpf_t*> bvec(a.size(), mpf_t_b);
	std::vector<mpf_t*> res = vectoroperations::Multiply(a, bvec);
	mpftoperations::ReleaseValue(mpf_t_b);
	return res;
}

std::vector<mpf_t*> Multiply(const std::vector<mpf_t*> & a, mpf_t* b) {
	std::vector<mpf_t*> bvec(a.size(), b);
	std::vector<mpf_t*> res = vectoroperations::Multiply(a, bvec);
	return res;
}

mpf_t* Add(const std::vector<mpf_t*> & a){
	mpf_t* res = mpftoperations::ToMpft(0.0);
	for(unsigned int i = 0; i < a.size(); i++){
		mpf_t* tmp = res;
		res = mpftoperations::Add(a[i], tmp);
		mpftoperations::ReleaseValue(tmp);
	}
	return res;
}

std::vector<mpf_t*> GetConstantVector(int dimension, const double & v) {
	mpf_t* val = mpftoperations::ToMpft(v);
	std::vector<mpf_t*> res(dimension, (mpf_t*)NULL);
	for (int i = 0; i < dimension; i++) {
		if (i == 0)
			res[i] = val;
		else
			res[i] = mpftoperations::Clone(val);
	}
	return res;
}

std::vector<mpf_t*> GetConstantVector(int dimension, const mpf_t* val) {
	std::vector<mpf_t*> res(dimension, (mpf_t*)NULL);
	for (int i = 0; i < dimension; i++) {
		res[i] = mpftoperations::Clone(val);
	}
	return res;
}

std::vector<mpf_t*> GetVector(const std::vector<double> & a) {
	std::vector<mpf_t*> res(a.size(), (mpf_t*)NULL);
	for (unsigned int i = 0; i < a.size(); i++) {
		res[i] = mpftoperations::ToMpft(a[i]);
	}
	return res;
}

void ReleaseValues(const std::vector<mpf_t*> & a) {
	for (unsigned int i = 0; i < a.size(); i++) {
		mpftoperations::ReleaseValue(a[i]);
	}
}

std::vector<mpf_t*> Clone(const std::vector<mpf_t*> & a) {
	std::vector<mpf_t*> res(a.size(), (mpf_t*)NULL);
	for (unsigned int i = 0; i < a.size(); i++) {
		res[i] = mpftoperations::Clone(a[i]);
	}
	return res;
}

std::vector<mpf_t*> Randomize(const std::vector<mpf_t*> & a) {
	std::vector<mpf_t*> randomVector = GetRandomVector(a.size());
	std::vector<mpf_t*> res = vectoroperations::Multiply(a, randomVector);
	ReleaseValues(randomVector);
	return res;
}

std::vector<mpf_t*> GetRandomVector(int dimensions) {
	std::vector<mpf_t*> res(dimensions, (mpf_t*) NULL);
	for(int i = 0; i < dimensions; i++){
		res[i] = mpftoperations::GetRandomMpft();
	}
	return res;
}

std::vector<double> MpftToDouble(const std::vector<mpf_t*> & a){
	std::vector<double> res(a.size());
	for(unsigned int i = 0; i < a.size(); i++){
		res[i] = (mpftoperations::MpftToDouble(a[i]));
	}
	return res;
}

bool Equals(const std::vector<mpf_t*> & a, const std::vector<mpf_t*> & b){
	AssertCondition(a.size() == b.size(), "Equality check of vectors with different size has been executed.");
	for(unsigned int i = 0; i < a.size(); i++){
		if(mpf_cmp(*a[i], *b[i]) != 0) return false;
	}
	return true;
}

// orthogonally projects the vector vec on the vector projection_vector
std::vector<mpf_t*> OrthogonalProjection(const std::vector<mpf_t*> & vec, const std::vector<mpf_t*> & projection_vector){
	std::vector<mpf_t*> tmp_v1, tmp_v2, result;
	mpf_t* tmp_m1;
	mpf_t* tmp_m2;
	mpf_t* tmp_m3;
	tmp_v1 = Multiply(vec, projection_vector);
	tmp_m1 = Add(tmp_v1);
	tmp_v2 = Multiply(projection_vector, projection_vector);
	tmp_m2 = Add(tmp_v2);
	if(mpftoperations::Compare(tmp_m2, 0.0) == 0) {
		tmp_m3 = mpftoperations::Clone(tmp_m1);
	} else {
		tmp_m3 = mpftoperations::Divide(tmp_m1, tmp_m2);
	}
	result = Multiply(projection_vector, tmp_m3);
	mpftoperations::ReleaseValue(tmp_m1);
	mpftoperations::ReleaseValue(tmp_m2);
	mpftoperations::ReleaseValue(tmp_m3);
	vectoroperations::ReleaseValues(tmp_v1);
	vectoroperations::ReleaseValues(tmp_v2);
	return result;
}

mpf_t* SquaredEuclideanLength(const std::vector<mpf_t*> & vec){
	std::vector<mpf_t*> help = Multiply(vec, vec);
	mpf_t* res = Add(help);
	ReleaseValues(help);
	return res;
}

void Sort(std::vector<mpf_t*> * vec){
	std::sort(vec->begin(), vec->end(), [](mpf_t* a, mpf_t* b)
			{
			return mpftoperations::Compare(a, b) < 0; 
			}
			);
}

} // namespace vectoroperations

namespace mpftoperations {

int number_of_mpf_t_values_in_use_ = 0;
int number_of_mpf_t_values_cached_ = 0;
bool statistical_calculations_active_ = false;
int statistical_calculations_active_depth_ = 0;
std::unordered_set<mpf_t*> available_mpf_t_;
std::queue<mpf_t*> available_mpf_t_queue_;
int last_number_of_added_mpf_t_pointers_ = 1;

int GetNumberOfMpftValuesInUse() {
	return number_of_mpf_t_values_in_use_;
}

int GetNumberOfMpftValuesCached() {
	return number_of_mpf_t_values_cached_;
}

void ChangeNumberOfMpftValuesCached(int change){
	number_of_mpf_t_values_cached_ += change;
}

void StatisticalCalculationsStart(){
	++statistical_calculations_active_depth_;
	statistical_calculations_active_ = true;
}

void StatisticalCalculationsEnd() {
	if( 0 == --statistical_calculations_active_depth_ )
		statistical_calculations_active_ = false;
}



mpf_t* add_NO_CHECK(const mpf_t* a, const mpf_t* b) {
	mpf_t* res = GetResultPointer();
	mpf_add(*res, *a, *b);
	return res;
}

mpf_t* subtract_NO_CHECK(const mpf_t* a, const mpf_t* b) {
	mpf_t* res = GetResultPointer();
	mpf_sub(*res, *a, *b);
	return res;
}

void IncreasePrecision(){
	int curprec = mpf_get_default_prec();
	mpf_set_default_prec(curprec + 10);
}

mpf_t* GetResultPointer() {
	++number_of_mpf_t_values_in_use_;
	if (available_mpf_t_queue_.empty()) {
		last_number_of_added_mpf_t_pointers_ *= 2;
		mpf_t *next_mpf_t = new mpf_t[last_number_of_added_mpf_t_pointers_];
		for (int i = 0; i < last_number_of_added_mpf_t_pointers_; i++) {
			available_mpf_t_.insert(next_mpf_t + i);
			available_mpf_t_queue_.push(next_mpf_t + i);
		}
	}

	mpf_t* res = available_mpf_t_queue_.front();
	available_mpf_t_queue_.pop();
	AssertCondition(available_mpf_t_.find(res) != available_mpf_t_.end(), "Internal Error. New generated mpf_t pointer is not valid.");
	available_mpf_t_.erase(res);
	mpf_init(*res);
	return res;
}

mpf_t* ToMpft(double v) {
	if(std::isnan(v))return GetUndefined();
	if(std::isinf(v)){
		if(v < 0)return GetMinusInfinity();
		else return GetPlusInfinity();
	}

	mpf_t* res = GetResultPointer();
	mpf_set_d(*res, v);
	return res;
}

mpf_t* ToMpft(long long v) {
	if(std::abs(v) < (1LL << 30)){
		long c = (long)v;
		mpf_t* res = GetResultPointer();
		mpf_set_si(*res, c);
		return res;
	}
	long long large_part = v / (1LL << 28);
	long long remaining = v - large_part * (1LL << 28);
	mpf_t* large_part_mpf_t = ToMpft(large_part);
	mpf_t* multiplyer = ToMpft(1LL << 28);
	mpf_t* small_part_mpf_t = ToMpft(remaining);
	mpf_t* res_large = mpftoperations::Multiply(large_part_mpf_t, multiplyer);
	mpf_t* res = mpftoperations::Add(res_large, small_part_mpf_t);
	mpftoperations::ReleaseValue(large_part_mpf_t);
	mpftoperations::ReleaseValue(multiplyer);
	mpftoperations::ReleaseValue(small_part_mpf_t);
	mpftoperations::ReleaseValue(res_large);
	return res;
}

mpf_t* ToMpft(unsigned long long v) {
	if(v < (1LL << 30)){
		long c = (long)v;
		mpf_t* res = GetResultPointer();
		mpf_set_si(*res, c);
		return res;
	}
	unsigned long long large_part = v / (1ULL << 28);
	unsigned long long remaining = v - large_part * (1ULL << 28);
	mpf_t* large_part_mpf_t = ToMpft(large_part);
	mpf_t* multiplyer = ToMpft(1LL << 28);
	mpf_t* small_part_mpf_t = ToMpft(remaining);
	mpf_t* res_large = mpftoperations::Multiply(large_part_mpf_t, multiplyer);
	mpf_t* res = mpftoperations::Add(res_large, small_part_mpf_t);
	mpftoperations::ReleaseValue(large_part_mpf_t);
	mpftoperations::ReleaseValue(multiplyer);
	mpftoperations::ReleaseValue(small_part_mpf_t);
	mpftoperations::ReleaseValue(res_large);
	return res;
}

mpf_t* ToMpft(unsigned int v) {
	return ToMpft((unsigned long long) v);
}

mpf_t* ToMpft(int v) {
	return ToMpft((long long) v);
}

void ReleaseValue(mpf_t* a) {
	if (a == NULL)
		return;
	if (IsInfinite(a)){
		return;
	}
	if (IsUndefined(a)){
		return;
	}
	AssertCondition(available_mpf_t_.find(a) == available_mpf_t_.end(), "Multiple frees on same mpf_t.");
	number_of_mpf_t_values_in_use_--;
	mpf_clear(*a);
	available_mpf_t_queue_.push(a);
	available_mpf_t_.insert(a);
}

inline void checkPrecisionOnAdd(const mpf_t* a, const mpf_t* b){
	if(Configuration::isIncreasePrecisionRecommended()) return;
	if(Configuration::getCheckPrecisionProbability() <= 0)return;
	bool doit = (Configuration::getCheckPrecisionMode() == configuration::CHECK_PRECISION_ALWAYS ||
			(Configuration::getCheckPrecisionMode() == configuration::CHECK_PRECISION_ALWAYS_EXCEPT_STATISTICS
			 && !statistical_calculations_active_));
	if(!doit)return;
	{
		if(IsInfinite(a) || IsInfinite(b)) return;
		if(IsUndefined(a) || IsUndefined(b)) return;
	}
	if(Configuration::getCheckPrecisionProbability() < 1){
		double memCheckPrecisionProbability = Configuration::getCheckPrecisionProbability();
		if(memCheckPrecisionProbability < 1.0){
			Configuration::setCheckPrecisionProbability(0.0);
		}
		mpf_t* rand_value = GetRandomMpft();
		Configuration::setCheckPrecisionProbability(memCheckPrecisionProbability);
		doit = (mpftoperations::Compare(rand_value, Configuration::getCheckPrecisionProbability()) < 0);
		mpftoperations::ReleaseValue(rand_value);
	}
	if(!doit)return;

	mpf_t* tmp = GetResultPointer();
	mpf_add(*tmp, *a, *b);
	std::vector<mpf_t*> v = {Abs(tmp), Abs(a), Abs(b)};
	ReleaseValue(tmp);
	std::sort(v.begin(), v.end(), [](mpf_t* a, mpf_t* b)
			{
			return mpf_cmp(*a, *b) < 0; 
			}
			);

	if(mpf_cmp_d(*v[0], 0.0) == 0){
		if(mpf_cmp(*v[1], *v[2]) != 0){
			Configuration::RecommendIncreasePrecision();
		}
	} else {
		mpf_t* small = mpftoperations::Multiply2Exp(v[0], -Configuration::getPrecisionSafetyMargin());
		mpf_t* addVal = GetResultPointer();
		mpf_add(*addVal, *small, *v[2]);
		if(mpf_cmp(*addVal, *v[2]) == 0){
			Configuration::RecommendIncreasePrecision();
		}
		ReleaseValue(small);
		ReleaseValue(addVal);
	}
	for(int i = 0; i < 3; i++){
		ReleaseValue(v[i]);
	}
}

mpf_t* Add(const mpf_t* a, const mpf_t* b) {
	if(IsUndefined(a) || IsUndefined(b)){
		return GetUndefined();
	}
	if(IsInfinite(a) || IsInfinite(b)){
		if(IsInfinite(a) ^ IsInfinite(b)){
			if(IsInfinite(a))return Clone(a);
			else return Clone(b);
		}
		if(IsPlusInfinity(a) == IsPlusInfinity(b)){
			return Clone(a);
		} else {
			return GetUndefined();
		}
	}
	checkPrecisionOnAdd(a, b);
	mpf_t* res = GetResultPointer();
	mpf_add(*res, *a, *b);
	return res;
}

mpf_t* Subtract(const mpf_t* a, const mpf_t* b) {
	mpf_t* tmp = Negate(b);
	mpf_t* res = Add(a,tmp);
	ReleaseValue(tmp);
	return res;
}

mpf_t* Multiply2Exp(const mpf_t* a, int p){
	if(IsUndefined(a)){
		return GetUndefined();
	}
	if(IsInfinite(a)){
		return Clone(a);
	}
	mpf_t* res = NULL;
	if(p < 0){
		res = GetResultPointer();
		mpf_div_2exp(*res, *a, -p);
	} else if(p > 0){
		res = GetResultPointer();
		mpf_mul_2exp(*res, *a,  p);
	} else {
		res = Clone(a);
	}
	return res;
}

mpf_t* Multiply(const mpf_t* a, const mpf_t* b) {
	if(IsUndefined(a) || IsUndefined(b)){
		return GetUndefined();
	}
	if(IsInfinite(a) || IsInfinite(b)){
		if(!IsInfinite(a))return Multiply(b, a);
		if(IsInfinite(a) && IsInfinite(b)){
			if(IsPlusInfinity(a) ^ IsPlusInfinity(b)){
				return GetMinusInfinity();
			} else {
				return GetPlusInfinity();
			}
		}
		if(Compare(b, 0.0) == 0){
			return GetUndefined();
		}
		if(IsPlusInfinity(a) ^ (Compare(b, 0.0) > 0)){
			return GetMinusInfinity();
		} else {
			return GetPlusInfinity();
		}
	}
	mpf_t* res = GetResultPointer();
	mpf_mul(*res, *a, *b);
	return res;
}

mpf_t* Multiply(const mpf_t* a, const double b) {
	mpf_t* res = GetResultPointer();
	mpf_t* mpf_t_b = ToMpft(b);
	mpf_mul(*res, *a, *mpf_t_b);
	ReleaseValue(mpf_t_b);
	return res;
}

mpf_t* Divide(const mpf_t* a, const mpf_t* b) {
	if(IsUndefined(a) || IsUndefined(b)){
		return GetUndefined();
	}
	if(IsInfinite(a) || IsInfinite(b)){
		if(IsInfinite(a) && IsInfinite(b)){
			return GetUndefined();
		}
		if(IsInfinite(b)){
			return ToMpft(0.0);
		}
		if(Compare(b, 0.0) == 0){
			return GetUndefined();
		}
		if(IsPlusInfinity(a) ^ (Compare(b, 0.0) > 0)){
			return GetMinusInfinity();
		} else {
			return GetPlusInfinity();
		}
	}
	mpf_t* res = GetResultPointer();
	mpf_div(*res, *a, *b);
	return res;
}

mpf_t* Abs(const mpf_t* v) {
	if(IsUndefined(v))return GetUndefined();
	if(IsInfinite(v))return GetPlusInfinity();
	mpf_t* res = GetResultPointer();
	mpf_abs(*res, *v);
	return res;
}

mpf_t* Negate(const mpf_t* v) {
	if(IsUndefined(v))return GetUndefined();
	if(IsInfinite(v)){
		if(IsPlusInfinity(v))return GetMinusInfinity();
		else return GetPlusInfinity();
	}
	mpf_t* res = GetResultPointer();
	mpf_neg(*res, *v);
	return res;
}

mpf_t* Clone(const mpf_t* v) {
	if(IsPlusInfinity(v)){
		return GetPlusInfinity();
	}
	if(IsMinusInfinity(v)){
		return GetMinusInfinity();
	}
	if(IsUndefined(v)){
		return GetUndefined();
	}
	mpf_t* res = GetResultPointer();
	mpf_set(*res, *v);
	AssertCondition((*res)->_mp_prec >= (*v)->_mp_prec, "Cloning of mpf_t failed.");
	(*res)->_mp_prec = (*v)->_mp_prec;

	// check actual equality
	AssertCondition((*v)->_mp_size == (*res)->_mp_size, "Cloning of mpf_t failed.");
	AssertCondition((*v)->_mp_prec == (*res)->_mp_prec, "Cloning of mpf_t failed.");
	AssertCondition((*v)->_mp_exp == (*res)->_mp_exp, "Cloning of mpf_t failed.");
	for(int i = 0; i < std::abs((*v)->_mp_size); i++){
		AssertCondition((*v)->_mp_d[i] == (*res)->_mp_d[i], "Cloning of mpf_t failed.");
	}
	return res;
}

// the generation of gaussian noise (random variable with normal
// distribution) is done by minor variation of Box Muller method.
mpf_t* GetGaussianRandomMpft(double mu, double sigma, RandomNumberGenerator* random){
	mpf_t* u = NULL;
	mpf_t* x = NULL;
	mpf_t* y = NULL;
	mpf_t* one = ToMpft(1.0);
	mpf_t* len2_xy = NULL;
	do {
		ReleaseValue(u);
		u = GetRandomMpft(random);
	} while( Compare(u, 0.0) <= 0 );
	do {
		ReleaseValue(x);
		ReleaseValue(y);
		ReleaseValue(len2_xy);

		mpf_t* tmp1 = GetRandomMpft(random);
		mpf_t* tmp2 = Multiply2Exp(tmp1, 1);
		x = Subtract(tmp2, one); // x = rand(0.0, 1.0) * 2.0 - 1.0
		ReleaseValue(tmp1);
		ReleaseValue(tmp2);

		tmp1 = GetRandomMpft(random);
		tmp2 = Multiply2Exp(tmp1, 1);
		y = Subtract(tmp2, one); // y = rand(0.0, 1.0) * 2.0 - 1.0
		ReleaseValue(tmp1);
		ReleaseValue(tmp2);

		tmp1 = mpftoperations::Multiply(x,x);
		tmp2 = mpftoperations::Multiply(y,y);
		len2_xy = mpftoperations::Add(tmp1, tmp2); // len2_xy = x * x + y * y
		ReleaseValue(tmp1);
		ReleaseValue(tmp2);
	} while( Compare(len2_xy, 0.0) == 0  || Compare(len2_xy, 1.0) > 0 );
	mpf_t* len_xy = Sqrt(len2_xy);
	mpf_t* len_inverse = Divide(one, len_xy); // = 1 / sqrt(x * x + y * y)
	mpf_t* tmp = Multiply(len_inverse, x);
	std::swap(x, tmp);
	ReleaseValue(tmp);
	tmp = Multiply(len_inverse,y);
	std::swap(y, tmp);
	ReleaseValue(tmp);

	ReleaseValue(one);
	ReleaseValue(len2_xy);
	ReleaseValue(len_xy);
	ReleaseValue(len_inverse);
	// now:
	// u is random variable in (0,1]
	// (x,y) is random point at the unit sphere
	mpf_t* log_u = LogE(u);
	mpf_t* minus_log_u = Negate(log_u);
	mpf_t* sqrt_minus_log_u = Sqrt(minus_log_u);
	mpf_t* x_plus_y = Add(x,y);
	mpf_t* mul1 = Multiply(sqrt_minus_log_u, x_plus_y);
	mpf_t* mul2 = Multiply(mul1, sigma);
	mpf_t* mu_mpft = ToMpft(mu);
	mpf_t* res = Add(mul2, mu_mpft);
	ReleaseValue(u);
	ReleaseValue(x);
	ReleaseValue(y);
	ReleaseValue(log_u);
	ReleaseValue(minus_log_u);
	ReleaseValue(sqrt_minus_log_u);
	ReleaseValue(x_plus_y);
	ReleaseValue(mul1);
	ReleaseValue(mul2);
	ReleaseValue(mu_mpft);
	return res; // = sqrt(-log(u)) * (x + y) * sigma + mu
}

mpf_t* GetGaussianRandomMpft(double mu, double sigma){
	return GetGaussianRandomMpft(mu, sigma, Configuration::getStandardRandomNumberGenerator());
}

mpf_t* Randomize(mpf_t* v) {
	return Randomize(v, Configuration::getStandardRandomNumberGenerator());
}

mpf_t* Randomize(mpf_t* v, RandomNumberGenerator* random){
	mpf_t* tmp = GetRandomMpft(random);
	mpf_t* res = Multiply(tmp, v);
	ReleaseValue(tmp);
	return res;
}

mpf_t* GetRandomMpft(){
	return GetRandomMpft(Configuration::getStandardRandomNumberGenerator());
}

mpf_t* GetRandomMpft(RandomNumberGenerator* random){
	return random->RandomMpft();
}

double MpftToDouble(mpf_t* v) {
	if(IsUndefined(v))return NAN;
	if(IsInfinite(v)){
		if(IsMinusInfinity(v))return -INFINITY;
		else return +INFINITY;
	}
	return mpf_get_d(*v);
}

mpf_t* Pow(mpf_t* v, double p){
	if(IsUndefined(v)){
		return GetUndefined();
	}
	if(IsInfinite(v)){
		if(p < 0){
			return ToMpft(0.0);
		} else if(p == 0){
			return ToMpft(1.0);
		} else if(IsPlusInfinity(v)){
			return GetPlusInfinity();
		} else {
			long long pp = p;
			if(pp - p == 0){
				if(pp % 2 == 0)return GetPlusInfinity();
				else return GetMinusInfinity();
			} else {
				return GetUndefined();
			}
		}
	}
	mpf_t* tmp = ToMpft(p);
	mpf_t* res = Pow(v, tmp);
	ReleaseValue(tmp);
	return res;
}

mpf_t* Pow(mpf_t* v, int p) {
	if(IsUndefined(v)){
		return GetUndefined();
	}
	if(IsInfinite(v)){
		if(p < 0){
			return ToMpft(0.0);
		} else if(p == 0){
			return ToMpft(1.0);
		} else if(IsPlusInfinity(v)){
			return GetPlusInfinity();
		} else {
			if(p % 2 == 0)return GetPlusInfinity();
			else return GetMinusInfinity();
		}
	}
	if(p < 0) {
		mpf_t* n1 = ToMpft(1.0);
		mpf_t* tmp = Divide(n1, v);
		mpf_t* res = Pow(tmp, -p);
		ReleaseValue(n1);
		ReleaseValue(tmp);
		return res;
	}
	// not using mpf_pow_ui because it produces different output on different versions!!
    if(p == 0) {
        return ToMpft(1);
    }
    if(p == 1) {
        return Clone(v);
    }
    mpf_t* square_v = Multiply(v, v);
    mpf_t* res = Pow(square_v, p / 2);
    ReleaseValue(square_v);
    if(p % 2 == 1){
        mpf_t* tmp = Multiply(res, v);
        std::swap(tmp, res);
        ReleaseValue(tmp);
    }
	return res;
}

mpf_t* Pow(mpf_t* v, mpf_t* p) {
	if(IsUndefined(v) || IsUndefined(p)){
		return GetUndefined();
	}
	if(IsInfinite(p)){
		return GetUndefined();
	}
	if(IsInfinite(v)){
		if(Compare(p,0.0) < 0){
			return ToMpft(0.0);
		} else if(Compare(p, 0.0) == 0){
			return ToMpft(1.0);
		} else if(IsPlusInfinity(v)){
			return GetPlusInfinity();
		} else {
			return GetUndefined();
		}
	}
	if(Compare(p, 0.0) == 0) {
		return ToMpft(1.0);
	}
	if(Compare(v, 0.0) == 0) {
		return ToMpft(0.0);
	}
	if(Compare(v, 1.0) == 0) {
		return ToMpft(1.0);
	}
	if(Compare(p, 0.0) < 0) {
		mpf_t* n1 = ToMpft(1.0);
		mpf_t* v_tmp = Divide(n1, v);
		mpf_t* p_tmp = Negate(p);
		mpf_t* res = Pow(v_tmp, p_tmp);
		ReleaseValue(n1);
		ReleaseValue(v_tmp);
		ReleaseValue(p_tmp);
		return res;
	}
	if(Compare(p, 1.0) >= 0) {
		std::vector<mpf_t*> powers;
		powers.push_back(Clone(v));
		std::vector<mpf_t*> exponents;
		exponents.push_back(ToMpft(1.0));
		int i = 0;
		while(mpf_cmp(*p, *(exponents[i])) > 0){
			powers.push_back(Multiply(powers[i], powers[i]));
			exponents.push_back(Add(exponents[i], exponents[i]));
			i++;
		}
		mpf_t* p_tmp = Clone(p);
		mpf_t* res = ToMpft(1.0);
		for( ; i >= 0; i--){
			if(mpf_cmp(*p_tmp, *(exponents[i])) >= 0) {
				mpf_t* tmp = Subtract(p_tmp, exponents[i]);
				std::swap(p_tmp, tmp);
				ReleaseValue(tmp);
				tmp = Multiply(res, powers[i]);
				std::swap(tmp, res);
				ReleaseValue(tmp);
			}
		}
		vectoroperations::ReleaseValues(powers);
		vectoroperations::ReleaseValues(exponents);
		powers.clear();
		exponents.clear();
		mpf_t* res2 = Pow(v, p_tmp);
		mpf_t* res1 = res;
		res = Multiply(res1, res2);
		ReleaseValue(res1);
		ReleaseValue(res2);
		ReleaseValue(p_tmp);
		return res;
	}
	// 0 < p < 1, v != 0, v != 1
	if(Compare(v, 0.0) < 0){
		return mpftoperations::GetUndefined();
	}
	// 0 < p < 1, v > 0, v != 1
	{
		mpf_t* n1 = ToMpft(1.0);
		mpf_t* res = ToMpft(1.0);
		mpf_t* v_tmp = Clone(v);
		mpf_t* p_tmp = Clone(p);
		while(Compare(v, 0.5) < 0 || Compare(v, 1.5) > 0){
			mpf_t* n_v = Sqrt(v_tmp);
			mpf_t* n_p = Add(p_tmp, p_tmp);
			ReleaseValue(v_tmp);
			ReleaseValue(p_tmp);
			v_tmp = n_v;
			p_tmp = n_p;
			if(Compare(p_tmp, 1.0) >= 0) {
				mpf_t* tmp = Multiply(res, v_tmp);
				std::swap(tmp, res);
				ReleaseValue(tmp);
				tmp =  Subtract(p_tmp, n1);
				std::swap(tmp, p_tmp);
				ReleaseValue(tmp);
				if(Compare(p_tmp, 0.0) == 0) {
					ReleaseValue(n1);
					ReleaseValue(v_tmp);
					ReleaseValue(p_tmp);
					return res;
				}
			}
		}
		// 0 < p < 1, 0.5 <= v <= 1.5, v != 1

		// taylor approximation:
		mpf_t* x = Subtract(v_tmp, n1);
		mpf_t* t_res = Clone(n1);
		mpf_t* n = Clone(n1);
		mpf_t* factor = Clone(n1);
		while(true){
			// update factor
			mpf_t* tmp = Multiply(factor, x);
			std::swap(factor, tmp);
			ReleaseValue(tmp);
			tmp = Multiply(factor, p_tmp);
			std::swap(factor, tmp);
			ReleaseValue(tmp);
			tmp = Divide(factor, n);
			std::swap(factor, tmp);
			ReleaseValue(tmp);
			// update p_tmp
			tmp = Subtract(p_tmp, n1);
			std::swap(tmp, p_tmp);
			ReleaseValue(tmp);
			// update n
			tmp = Add(n, n1);
			std::swap(n, tmp);
			ReleaseValue(tmp);
			// update res
			mpf_t* n_t_res = add_NO_CHECK(t_res, factor);
			bool same = (mpf_cmp(*t_res, *n_t_res) == 0);
			std::swap(t_res, n_t_res);
			ReleaseValue(n_t_res);
			if(same)break;
		}
		ReleaseValue(x);
		ReleaseValue(n);
		ReleaseValue(factor);
		ReleaseValue(v_tmp);
		ReleaseValue(p_tmp);
		ReleaseValue(n1);
		mpf_t* a_res = Multiply(res, t_res);
		ReleaseValue(res);
		ReleaseValue(t_res);
		return  a_res;
	}
}

mpf_t* Max(mpf_t* a, mpf_t* b) {
	if(IsPlusInfinity(a) || IsPlusInfinity(b))return GetPlusInfinity();
	if(IsUndefined(a) || IsUndefined(b))return GetUndefined();
	if(IsMinusInfinity(a))return Clone(b);
	if(IsMinusInfinity(b))return Clone(a);
	mpf_t* res = GetResultPointer();
	if (Compare(a, b) <= 0) {
		mpf_set(*res, *b);
	} else {
		mpf_set(*res, *a);
	}
	return res;
}

mpf_t* Min(mpf_t* a, mpf_t* b) {
	if(IsMinusInfinity(a) || IsMinusInfinity(b))return GetMinusInfinity();
	if(IsUndefined(a) || IsUndefined(b))return GetUndefined();
	if(IsPlusInfinity(a))return Clone(b);
	if(IsPlusInfinity(b))return Clone(a);
	mpf_t* res = GetResultPointer();
	if (Compare(a, b) >= 0) {
		mpf_set(*res, *b);
	} else {
		mpf_set(*res, *a);
	}
	return res;
}

mpf_t* Floor(mpf_t* v) {
	if(IsUndefined(v)) return GetUndefined();
	if(IsMinusInfinity(v)) return GetMinusInfinity();
	if(IsPlusInfinity(v)) return GetPlusInfinity();
	mpf_t* res = GetResultPointer();
	mpf_floor(*res, *v);
	return res;
}

mpf_t* Ceil(mpf_t* v) {
	if(IsUndefined(v)) return GetUndefined();
	if(IsMinusInfinity(v)) return GetMinusInfinity();
	if(IsPlusInfinity(v)) return GetPlusInfinity();
	mpf_t* res = GetResultPointer();
	mpf_ceil(*res, *v);
	return res;
}

mpf_t* Sqrt(mpf_t* num) {
	if(IsUndefined(num))return GetUndefined();
	if(IsPlusInfinity(num))return GetPlusInfinity();
	if(IsMinusInfinity(num))return GetUndefined();
	if(Compare(num, 0.0) < 0)return GetUndefined();
	mpf_t* res = GetResultPointer();
	mpf_sqrt(*res, *num);
	return res;
}

mpf_t* cos_taylor(mpf_t* num){
	mpf_t* res = ToMpft(0.0);
	mpf_t* square = Multiply(num, num);
	mpf_t* nsquare = Negate(square);
	mpf_t* mul = ToMpft(1.0);
	// use taylor approximation
	for(int n = 0; true; n++){
		mpf_t* f1 = ToMpft(n * 2 + 1.0);
		mpf_t* f2 = ToMpft(n * 2 + 2.0);
		mpf_t* factor = Divide(nsquare, f1);
		mpf_t* tmp = Divide(factor, f2);
		ReleaseValue(factor);
		ReleaseValue(f1);
		ReleaseValue(f2);
		factor = tmp;
		mpf_t* nmul = Multiply(mul, factor);
		ReleaseValue(mul);
		mul = nmul;
		mpf_t* nres = add_NO_CHECK(res, mul);
		bool same = (mpf_cmp(*res, *nres) == 0);
		ReleaseValue(res);
		res = nres;
		ReleaseValue(factor);
		if(same)break;
	}
	ReleaseValue(mul);
	ReleaseValue(square);
	ReleaseValue(nsquare);
	mpf_t* n1 = ToMpft(1.0);
	mpf_t* nres = Add(res, n1); // DO PRECISION CHECK HERE!!
	ReleaseValue(n1);
	ReleaseValue(res);
	res = nres;
	return res;
}

mpf_t* sin_taylor(mpf_t* num){
	mpf_t* mul = Clone(num);
	mpf_t* res = Clone(num);
	mpf_t* square = Multiply(num, num);
	mpf_t* nsquare = Negate(square);
	// use taylor approximation
	for(int n = 0; true; n++){
		mpf_t* f1 = ToMpft(n * 2 + 2.0);
		mpf_t* f2 = ToMpft(n * 2 + 3.0);
		mpf_t* factor = Divide(nsquare, f1);
		mpf_t* tmp = Divide(factor, f2);
		ReleaseValue(factor);
		ReleaseValue(f1);
		ReleaseValue(f2);
		factor = tmp;
		mpf_t* nmul = Multiply(mul, factor);
		ReleaseValue(mul);
		mul = nmul;
		mpf_t* nres = add_NO_CHECK(res, mul);
		bool same = (mpf_cmp(*res, *nres) == 0);
		ReleaseValue(res);
		res = nres;
		ReleaseValue(factor);
		if(same)break;
	}
	ReleaseValue(mul);
	ReleaseValue(square);
	ReleaseValue(nsquare);
	return res;
}

std::vector<mpf_t*> sin_cos_cordic_angles_cached_;
unsigned int sin_cos_cordic_angles_cached_precision_ = 0;
mpf_t* sin_cos_cordic_factor_cached_ = NULL;
unsigned int sin_cos_cordic_factor_cached_precision_ = 0;

std::pair<mpf_t*, mpf_t*> sin_cos_cordic(mpf_t* num){
	if(sin_cos_cordic_angles_cached_precision_ != mpf_get_default_prec()){
		sin_cos_cordic_angles_cached_precision_ = mpf_get_default_prec();
		number_of_mpf_t_values_cached_ -= sin_cos_cordic_angles_cached_.size();
		vectoroperations::ReleaseValues(sin_cos_cordic_angles_cached_);
		sin_cos_cordic_angles_cached_.clear();
		mpf_t* pi = GetPi();
		sin_cos_cordic_angles_cached_.push_back(Multiply2Exp(pi, -2));
		ReleaseValue(pi);
		int p = 0;
		mpf_t* n1 = ToMpft(1.0);
		while(true){
			mpf_t* cv = Multiply2Exp(n1, --p);
			mpf_t* inv = Arctan(cv);
			sin_cos_cordic_angles_cached_.push_back(inv);
			ReleaseValue(cv);
			mpf_t* tmp = add_NO_CHECK(inv, sin_cos_cordic_angles_cached_[0]);
			bool same = (mpf_cmp(*tmp, *(sin_cos_cordic_angles_cached_[0])) == 0);
			ReleaseValue(tmp);
			if(same)break;
		}
		ReleaseValue(n1);
		number_of_mpf_t_values_cached_ += sin_cos_cordic_angles_cached_.size();
	}
	mpf_t* cx = ToMpft(1.0);
	mpf_t* cy = ToMpft(0.0);
	AssertCondition(std::abs(MpftToDouble(num)) < std::acos(0.0) + 1e-7 , "CORDIC precondition not fulfilled. (sin / cos / tan calculations)");
	mpf_t* cnum = Clone(num);
	bool same = false;
	for(unsigned int p = 0; p < sin_cos_cordic_angles_cached_.size() && !same; p++) {
		mpf_t* cxs = Multiply2Exp(cx, -p);
		mpf_t* cys = Multiply2Exp(cy, -p);
		if(Compare(cnum, 0.0) > 0){
			mpf_t* nnum = subtract_NO_CHECK(cnum, sin_cos_cordic_angles_cached_[p]);
			ReleaseValue(cnum);
			cnum = nnum;
			mpf_t* nx = subtract_NO_CHECK(cx, cys);
			mpf_t* ny = add_NO_CHECK(cy, cxs);
			same = (mpf_cmp(*cx, *nx) == 0 && mpf_cmp(*cy, *ny) == 0);
			ReleaseValue(cx);
			ReleaseValue(cy);
			cx = nx;
			cy = ny;
		} else {
			mpf_t* nnum = add_NO_CHECK(cnum, sin_cos_cordic_angles_cached_[p]);
			ReleaseValue(cnum);
			cnum = nnum;
			mpf_t* nx = add_NO_CHECK(cx, cys);
			mpf_t* ny = subtract_NO_CHECK(cy, cxs);
			same = (mpf_cmp(*cx, *nx) == 0 && mpf_cmp(*cy, *ny) == 0);
			ReleaseValue(cx);
			ReleaseValue(cy);
			cx = nx;
			cy = ny;
		}
		ReleaseValue(cxs);
		ReleaseValue(cys);
	}
	ReleaseValue(cnum);
	if(sin_cos_cordic_factor_cached_precision_ != mpf_get_default_prec()){
		sin_cos_cordic_factor_cached_precision_ = mpf_get_default_prec();
		if(sin_cos_cordic_factor_cached_ == NULL)++number_of_mpf_t_values_cached_;
		ReleaseValue(sin_cos_cordic_factor_cached_);

		mpf_t* xx = Multiply(cx, cx);
		mpf_t* yy = Multiply(cy, cy);
		mpf_t* slen = add_NO_CHECK(xx, yy);
		mpf_t* sq = Sqrt(slen);
		mpf_t* n1 = ToMpft(1.0);
		sin_cos_cordic_factor_cached_ = Divide(n1, sq);
		ReleaseValue(xx);
		ReleaseValue(yy);
		ReleaseValue(slen);
		ReleaseValue(sq);
		ReleaseValue(n1);
	}
	mpf_t* nx = Multiply(cx, sin_cos_cordic_factor_cached_);
	mpf_t* ny = Multiply(cy, sin_cos_cordic_factor_cached_);
	ReleaseValue(cx);
	ReleaseValue(cy);
	return std::make_pair(nx, ny);
}

mpf_t* sin_CORDIC(mpf_t* num){
	std::pair<mpf_t*, mpf_t*> sc = sin_cos_cordic(num);
	ReleaseValue(sc.first);
	return sc.second;
}

mpf_t* cos_CORDIC(mpf_t* num){
	std::pair<mpf_t*, mpf_t*> sc = sin_cos_cordic(num);
	ReleaseValue(sc.second);
	return sc.first;
}

mpf_t* arctan_taylor(mpf_t* num){
	mpf_t* mul = Clone(num);
	mpf_t* res = Clone(num);
	mpf_t* square = Multiply(num, num);
	mpf_t* nsquare = Negate(square);
	// use taylor approximation
	for(int n = 0; true; n++){
		mpf_t* nmul = Multiply(mul, nsquare);
		ReleaseValue(mul);
		mul = nmul;
		mpf_t* f = ToMpft(n * 2 + 3.0);
		mpf_t* part = Divide(mul, f);
		ReleaseValue(f);
		mpf_t* nres = add_NO_CHECK(res, part);
		bool same = (mpf_cmp(*res, *nres) == 0);
		ReleaseValue(res);
		res = nres;
		ReleaseValue(part);
		if(same)break;
	}
	ReleaseValue(mul);
	ReleaseValue(square);
	ReleaseValue(nsquare);
	return res;
}

mpf_t* Arctan(mpf_t* num){
	if(IsUndefined(num))return GetUndefined();
	if(IsInfinite(num)){
		mpf_t* pi = GetPi();
		mpf_t* pi_half = mpftoperations::Multiply(pi, 0.5);
		mpftoperations::ReleaseValue(pi);
		mpf_t* res = NULL;
		if(IsPlusInfinity(num))res = Clone(pi_half);
		else res = Negate(pi_half);
		mpftoperations::ReleaseValue(pi_half);
		return res;
	}
	if(Compare(num, 0.0) < 0) {
		// arctan is symmetric to the origin
		mpf_t* nnum = Negate(num);
		mpf_t* nres = Arctan(nnum);
		mpf_t* res = Negate(nres);
		ReleaseValue(nnum);
		ReleaseValue(nres);
		return res;
	} else if(Compare(num, 0.5) > 0) {
		// application of formula:
		// arctan(x)=2*arctan(x/(1+sqrt(1+x*x)))
		// this leads to smaller arguments such that
		// taylor approximation converges much faster
		mpf_t* square = Multiply(num, num);
		mpf_t* n1 = ToMpft(1.0);
		mpf_t* t1 = Add(square, n1);
		mpf_t* t2 = Sqrt(t1);
		mpf_t* den = Add(n1, t2);
		mpf_t* nnum = Divide(num, den);
		ReleaseValue(square);
		ReleaseValue(n1);
		ReleaseValue(t1);
		ReleaseValue(t2);
		ReleaseValue(den);
		mpf_t* tmpres = Arctan(nnum);
		mpf_t* res = Add(tmpres, tmpres);
		ReleaseValue(tmpres);
		ReleaseValue(nnum);
		return res;
	} else {
		return arctan_taylor(num);
	}
}

mpf_t* Arcsin(mpf_t* num){
	if(IsUndefined(num))return GetUndefined();
	if(IsInfinite(num))return GetUndefined();
	// case num = -1 or num = +1
	if(Compare(num, 1.0) == 0
			|| Compare(num, -1.0) == 0) {
		mpf_t* pi = GetPi();
		mpf_t* n2 = ToMpft(2.0);
		mpf_t* res = Divide(pi, n2);
		ReleaseValue(pi);
		ReleaseValue(n2);
		if(Compare(num, -1.0) == 0){
			mpf_t* tmp = Negate(res);
			ReleaseValue(res);
			res = tmp;
		}
		return res;
	}
	if(Compare(num, -1.0) < 0){
		return GetUndefined();
	}
	if(Compare(num,  1.0) > 0){
		return GetUndefined();
	}
	// application of formula:
	// arcsin(x) = arctan(x / sqrt(1 - x * x))
	mpf_t* n1 = ToMpft(1.0);
	mpf_t* squ = Multiply(num, num);
	mpf_t* sqDen = Subtract(n1, squ);
	mpf_t* den = Sqrt(sqDen);
	mpf_t* nval = Divide(num, den);
	mpf_t* res = Arctan(nval);
	ReleaseValue(n1);
	ReleaseValue(squ);
	ReleaseValue(sqDen);
	ReleaseValue(den);
	ReleaseValue(nval);
	return res;
}

mpf_t* Arccos(mpf_t* num){
	mpf_t* pi = GetPi();
	mpf_t* n2 = ToMpft(2.0);
	mpf_t* pi_d2 = Divide(pi, n2);
	mpf_t* a_sin = Arcsin(num);
	mpf_t* res = Subtract(pi_d2, a_sin);
	ReleaseValue(pi);
	ReleaseValue(n2);
	ReleaseValue(pi_d2);
	ReleaseValue(a_sin);
	return res;
}

mpf_t* Tan(mpf_t* num){
	mpf_t* nume = Sin(num);
	mpf_t* deno = Cos(num);
	// division by zero produces undefined value
	mpf_t* res = Divide(nume, deno);
	mpftoperations::ReleaseValue(nume);
	mpftoperations::ReleaseValue(deno);
	return res;
}

mpf_t* Cos(mpf_t* num){
	if(IsUndefined(num))return GetUndefined();
	if(IsInfinite(num))return GetUndefined();
	// application of formula:
	// cos(alpha) = sin(alpha + pi / 2)
	mpf_t* pi = GetPi();
	mpf_t* pi_half = Multiply2Exp(pi, -1);
	mpf_t* nnum = Add(num, pi_half); // DO PRECISION CHECK HERE!!
	mpf_t* res = Sin(nnum);
	ReleaseValue(nnum);
	ReleaseValue(pi_half);
	ReleaseValue(pi);
	return res;
}

mpf_t* Sin(mpf_t* num){
	if(IsUndefined(num))return GetUndefined();
	if(IsInfinite(num))return GetUndefined();
	num = Clone(num);
	mpf_t* pi = GetPi();
	mpf_t* minus_pi = Negate(pi);
	mpf_t* n1 = ToMpft(1.0);
	mpf_t* pi_half = Multiply2Exp(pi, -1);
	mpf_t* pi_d4 = Multiply2Exp(pi_half, -1);
	mpf_t* n1d2 = Multiply2Exp(n1, -1);
	mpf_t* n2_pi = Multiply2Exp(pi, 1);
	mpf_t* absnum = Abs(num);

	// move in interval -pi..pi
	if(mpf_cmp(*absnum, *pi) > 0){
		// use formula
		// sin(alpha) = sin(alpha + m * 2 * pi)
		// for all integers m
		mpf_t* num_d_pi = Divide(num, n2_pi);
		mpf_t* num_d_pi_phalf = add_NO_CHECK(num_d_pi, n1d2);
		mpf_t* num_d_pi_round = GetResultPointer();
		mpf_floor(*num_d_pi_round, *num_d_pi_phalf);
		mpf_t* move = Multiply(num_d_pi_round, n2_pi);
		mpf_t* nnum = Subtract(num, move); // DO PRECISION CHECK HERE!!
		ReleaseValue(num);
		ReleaseValue(num_d_pi);
		ReleaseValue(num_d_pi_phalf);
		ReleaseValue(num_d_pi_round);
		ReleaseValue(move);
		num = nnum;
		ReleaseValue(absnum);
		absnum = Abs(num);
	}

	if(mpf_cmp(*absnum, *pi) > 0){
		// happens only if initial num is very large
		mpf_t* res = Sin(num);
		ReleaseValue(num);
		ReleaseValue(pi);
		ReleaseValue(minus_pi);
		ReleaseValue(n1);
		ReleaseValue(pi_half);
		ReleaseValue(pi_d4);
		ReleaseValue(n1d2);
		ReleaseValue(n2_pi);
		ReleaseValue(absnum);
		return res;
	}

	// move in interval 0 .. pi
	bool doNegate = false;
	if(Compare(num, 0.0) < 0){
		// application of formula:
		// sin(alpha) = -sin(-alpha)
		doNegate = true;
		mpf_t* nnum = Negate(num);
		ReleaseValue(num);
		num = nnum;
	}
	// move in interval 0 .. pi/2
	if(mpf_cmp(*num, *pi_half) > 0){
		// application of formula:
		// sin(alpha) = sin(pi - alpha)
		mpf_t* nnum = Subtract(pi, num); // DO PRECISION CHECK HERE!!
		ReleaseValue(num);
		num = nnum;
	}
	// move in interval 0 .. pi/4
	bool doCosinus = false;
	if(mpf_cmp(*num, *pi_d4) > 0){
		// application of formula:
		// sin(alpha) = cos(alpha - pi / 2)
		doCosinus = true;
		mpf_t* nnum = Subtract(pi_half, num); // DO PRECISION CHECK HERE!!
		ReleaseValue(num);
		num = nnum;
	}

	ReleaseValue(pi);
	ReleaseValue(minus_pi);
	ReleaseValue(n1);
	ReleaseValue(pi_half);
	ReleaseValue(pi_d4);
	ReleaseValue(n1d2);
	ReleaseValue(n2_pi);
	ReleaseValue(absnum);

	mpf_t* res = NULL;

#if ARBITRARY_PRECISION_CALCULATION_OPERATIONS_CPP_USE_TAYLOR_APPROXIMATION_FOR_TRIGONOMETRC_FUNCTIONS_ // use taylor approximation
	if(doCosinus)res = cos_taylor(num);
	else res = sin_taylor(num);
#else // use CORDIC-algorithm
	if(doCosinus)res = cos_CORDIC(num);
	else res = sin_CORDIC(num);
#endif

	ReleaseValue(num);
	if(doNegate){
		mpf_t* tmp = Negate(res);
		ReleaseValue(res);
		res = tmp;
	}
	return res;
}

int Compare(const mpf_t* a, const double b){
	AssertCondition(!IsUndefined(a) && !std::isnan(b), "Comparisons with undefined values are not possible. Please check whether your functions or statistics can produce undefined values.");
	if(std::isfinite(b)){
		if(IsPlusInfinity(a)){
			return 1;
		} else if(IsMinusInfinity(a)){
			return -1;
		} else {
			return mpf_cmp_d(*a, b);
		}
	} else {
		mpf_t* tmp_b = ToMpft(b);
		int res = Compare(a,tmp_b);
		ReleaseValue(tmp_b);
		return res;
	}
}

int Compare(const double a, const mpf_t* b){
	return -Compare(b, a);
}

int Compare(const mpf_t* a, const mpf_t* b){
	AssertCondition(!IsUndefined(a) && !IsUndefined(b),
			"Comparisons with undefined values are not possible. Please check whether your functions or statistics can produce undefined values.");
	if ( IsPlusInfinity(a) ) {
		if ( IsPlusInfinity(b) ) {
			return 0;
		} else {
			return 1;
		}
	} else if ( IsPlusInfinity(b) ) {
		return -1;
	}
	if ( IsMinusInfinity(a) ) {
		if ( IsMinusInfinity(b) ) {
			return 0;
		} else {
			return -1;
		}
	} else if ( IsMinusInfinity(b) ) {
		return 1;
	}

	if(mpf_cmp(*a, *b) == 0)return 0;

	mpf_t* a1 = Multiply2Exp(a, -1);
	mpf_t* a2 = Multiply2Exp(a, 1);
	if(mpf_cmp(*a2, *a1) < 0)std::swap(a1,a2);
	bool inside = ((mpf_cmp(*a1, *b) < 0) && (mpf_cmp(*b, *a2) < 0));
	mpftoperations::ReleaseValue(a1);
	mpftoperations::ReleaseValue(a2);
	if(!inside){
		return mpf_cmp(*a, *b);
	}
	// if values are close difference has to be calculated
	// to ensure that precision is checked
	mpf_t* diff = mpftoperations::Subtract(a, b);
	int res = mpf_cmp_d(*diff, 0.0);
	mpftoperations::ReleaseValue(diff);
	return res;
}

mpf_t* mpft_plus_infinity_cached_ = NULL;
mpf_t* GetPlusInfinity(){
	if(mpft_plus_infinity_cached_ == NULL){
		mpft_plus_infinity_cached_ = GetResultPointer();
		ChangeNumberOfMpftValuesCached(1);
	}
	return mpft_plus_infinity_cached_;
}

mpf_t* mpft_minus_infinity_cached_ = NULL;
mpf_t* GetMinusInfinity(){
	if(mpft_minus_infinity_cached_ == NULL) {
		mpft_minus_infinity_cached_ = GetResultPointer();
		ChangeNumberOfMpftValuesCached(1);
	}
	return mpft_minus_infinity_cached_;
}

bool IsPlusInfinity(const mpf_t* v){
	return v == GetPlusInfinity();
}
bool IsMinusInfinity(const mpf_t* v){
	return v == GetMinusInfinity();
}
bool IsInfinite(const mpf_t* v){
	return IsPlusInfinity(v) || IsMinusInfinity(v);
}

mpf_t* mpft_undefined_ = NULL;
mpf_t* GetUndefined(){
	if(mpft_undefined_ == NULL){
		mpft_undefined_ = GetResultPointer();
		ChangeNumberOfMpftValuesCached(1);
	}
	return mpft_undefined_;
}

bool IsUndefined(const mpf_t* v){
	return v == GetUndefined();
}

mpf_t* pi_cached_ = NULL;
unsigned int pi_cached_precision_ = 1;
mpf_t* GetPi(){
	if(pi_cached_ == NULL || mpf_get_default_prec() != pi_cached_precision_){
		if(pi_cached_ == NULL)number_of_mpf_t_values_cached_++;
		// calculate Pi with Bailey-Borwein-Plouffe formula
		ReleaseValue(pi_cached_);
		pi_cached_ = NULL;
		pi_cached_precision_ = mpf_get_default_prec();
		mpf_t* n16 = ToMpft(16.0);
		mpf_t* n8 = ToMpft(8.0);
		mpf_t* n6 = ToMpft(6.0);
		mpf_t* n5 = ToMpft(5.0);
		mpf_t* n4 = ToMpft(4.0);
		mpf_t* n2 = ToMpft(2.0);
		mpf_t* n1 = ToMpft(1.0);
		mpf_t* n0 = ToMpft(0.0);
		mpf_t* n1d16 = Divide(n1, n16);
		mpf_t* mul = Clone(n1);
		mpf_t* k8 = Clone(n0);
		mpf_t* res = Clone(n0);
		bool finish = false;
		while(!finish){
			mpf_t* d1 = add_NO_CHECK(k8, n1);
			mpf_t* t1 = Divide(n4, d1);
			mpf_t* d2 = add_NO_CHECK(k8, n4);
			mpf_t* t2 = Divide(n2, d2);
			mpf_t* d3 = add_NO_CHECK(k8, n5);
			mpf_t* t3 = Divide(n1, d3);
			mpf_t* d4 = add_NO_CHECK(k8, n6);
			mpf_t* t4 = Divide(n1, d4);
			mpf_t* t12 = subtract_NO_CHECK(t1, t2);
			mpf_t* t123 = subtract_NO_CHECK(t12, t3);
			mpf_t* t1234 = subtract_NO_CHECK(t123, t4);
			mpf_t* curAdd = Multiply(t1234, mul);
			mpf_t* nres = add_NO_CHECK(res, curAdd);
			finish = (mpf_cmp(*res, *nres) == 0);
			ReleaseValue(res);
			res = nres;
			mpf_t* nk8 = add_NO_CHECK(k8, n8);
			ReleaseValue(k8);
			k8 = nk8;
			mpf_t* nmul = Multiply(mul, n1d16);
			ReleaseValue(mul);
			mul = nmul;
			ReleaseValue(d1);
			ReleaseValue(t1);
			ReleaseValue(d2);
			ReleaseValue(t2);
			ReleaseValue(d3);
			ReleaseValue(t3);
			ReleaseValue(d4);
			ReleaseValue(t4);
			ReleaseValue(t12);
			ReleaseValue(t123);
			ReleaseValue(t1234);
			ReleaseValue(curAdd);
		}
		ReleaseValue(n16);
		ReleaseValue(n8);
		ReleaseValue(n6);
		ReleaseValue(n5);
		ReleaseValue(n4);
		ReleaseValue(n2);
		ReleaseValue(n1);
		ReleaseValue(n0);
		ReleaseValue(n1d16);
		ReleaseValue(mul);
		ReleaseValue(k8);
		pi_cached_ = res;
	}
	return Clone(pi_cached_);
}

mpf_t* e_cached = NULL;
unsigned int e_cached_precision = 1;
mpf_t* GetE(){
	if(e_cached == NULL || mpf_get_default_prec() != e_cached_precision){
		if(e_cached == NULL)number_of_mpf_t_values_cached_++;
		ReleaseValue(e_cached);
		e_cached = NULL;
		e_cached_precision = mpf_get_default_prec();
		mpf_t* n1 = ToMpft(1.0);
		e_cached = Exp(n1);
		ReleaseValue(n1);
	}
	return Clone(e_cached);
}

mpf_t* exp_taylor(mpf_t* v){
	mpf_t* n1 = ToMpft(1.0);
	mpf_t* res = Clone(n1);
	mpf_t* n = Clone(n1);
	mpf_t* factor = Clone(n1);
	while(true){
		mpf_t* tmp = Divide(factor, n);
		std::swap(tmp, factor);
		ReleaseValue(tmp);
		tmp = Multiply(factor, v);
		std::swap(tmp, factor);
		ReleaseValue(tmp);
		tmp = Add(n, n1);
		std::swap(n, tmp);
		ReleaseValue(tmp);
		mpf_t* nres = add_NO_CHECK(res, factor);
		bool same = (mpf_cmp(*nres, *res) == 0);
		ReleaseValue(res);
		res = nres;
		if(same)break;
	}
	ReleaseValue(n1);
	ReleaseValue(n);
	ReleaseValue(factor);
	return res;
}

mpf_t* Exp(mpf_t* v){
	if(IsUndefined(v))return GetUndefined();
	if(IsPlusInfinity(v))return GetPlusInfinity();
	if(IsMinusInfinity(v))return ToMpft(0.0);
	if(Compare(v, -1.0) < 0 || Compare(v, 1.0) > 0){
		mpf_t* v_half = Multiply2Exp(v, -1);
		mpf_t* res_half = Exp(v_half);
		mpf_t* res = Multiply(res_half, res_half);
		ReleaseValue(v_half);
		ReleaseValue(res_half);
		return res;
	} else {
		return exp_taylor(v);
	}
}

mpf_t* logE_taylor(mpf_t* v){
	mpf_t* n1 = ToMpft(1.0);
	mpf_t* x = Subtract(n1, v);
	mpf_t* res = Negate(x);
	mpf_t* n = Clone(n1);
	mpf_t* factor = Negate(x);
	while(true){
		mpf_t* tmp = Multiply(factor, x);
		std::swap(tmp, factor);
		ReleaseValue(tmp);
		tmp = Add(n, n1);
		std::swap(n, tmp);
		ReleaseValue(tmp);
		mpf_t* part = Divide(factor, n);
		mpf_t* nres = add_NO_CHECK(res, part);
		ReleaseValue(part);
		bool same = (mpf_cmp(*nres, *res) == 0);
		ReleaseValue(res);
		res = nres;
		if(same)break;
	}
	ReleaseValue(n1);
	ReleaseValue(x);
	ReleaseValue(n);
	ReleaseValue(factor);
	return res;
}

mpf_t* LogE(mpf_t* v){
	if(IsUndefined(v))return GetUndefined();
	if(IsPlusInfinity(v))return GetPlusInfinity();
	if(IsMinusInfinity(v))return GetUndefined();
	if(Compare(v, 0.0) < 0)return GetUndefined();
	if(Compare(v, 0.0) ==  0)return GetMinusInfinity();
	if(Compare(v, 0.99) < 0 || Compare(v, 1.01) > 0){
		mpf_t* sqv = Sqrt(v);
		mpf_t* n2 = ToMpft(2);
		mpf_t* res_sq = LogE(sqv);
		mpf_t* res = Multiply(n2, res_sq);
		ReleaseValue(res_sq);
		ReleaseValue(sqv);
		ReleaseValue(n2);
		return res;
	} else {
		return logE_taylor(v);
	}
}

double Log2Double(mpf_t* v) {
	if(IsUndefined(v))return NAN;
	if(IsPlusInfinity(v))return INFINITY;
	if(IsMinusInfinity(v))return NAN;
	if(Compare(v, 0.0) < 0)return NAN;
	if(Compare(v, 0.0) ==  0)return -INFINITY;
	AssertCondition(Compare(v, 0.0) > 0, "");
	if(Compare(v, 1e-300) < 0 || Compare(v, 1e300) > 0){
		mpf_t* sq = Sqrt(v);
		double res = Log2Double(sq);
		mpftoperations::ReleaseValue(sq);
		return 2.0 * res;
	}
	return std::log(mpftoperations::MpftToDouble(v)) / std::log(2.0);
}

std::string MpftToString(mpf_t* v){
	if(IsUndefined(v))return "nan";
	if(IsPlusInfinity(v))return "inf";
	if(IsMinusInfinity(v))return "-inf";
	static char* buf = NULL;
	static int buf_size = 0;
	if(buf == NULL){
		AssertCondition(buf_size == 0, "Initialization failed.");
		buf_size = 10;
		buf = (char*) malloc(buf_size * sizeof(char));
	}
	AssertCondition(buf_size != 0, "Initialization failed.");
	int string_size;
	int output_precision = Configuration::getOutputPrecision();
	if(output_precision == -1){
		output_precision = mpf_get_default_prec() / log(10.0) * log(2.0);
	}
	while (true) {
		string_size = gmp_snprintf(buf, buf_size, "%.*Fe", output_precision, *v);
		if( string_size < buf_size ){
			break;
		}
		free (buf);
		buf_size *= 2;
		buf = (char*) malloc(buf_size * sizeof(char));
	}
	AssertCondition(string_size == (int)strlen(buf), "");
	if(Configuration::getOutputPrecision() == -1){
		int e_pos = string_size - 1;
		while(e_pos > 0 && buf[e_pos] != 'e')--e_pos;
		if((buf[1] == '.' || buf[2] == '.') && e_pos > 1 && buf[e_pos - 1] == '0'){
			int zeros = 1;
			while(buf[e_pos - 1 - zeros] == '0')++zeros;
			if(buf[e_pos - 1 - zeros] == '.')--zeros;
			for(int copy_pos = e_pos; copy_pos <= string_size; copy_pos++){
				buf[copy_pos - zeros] = buf[copy_pos];
			}
		}
	}
	std::string res(buf);
	return res;
}

int StringToInt(std::string s){
	std::istringstream is(s);
	int res;
	is >> res;
	return res;
}

void StoreMpft(mpf_t* a, std::ofstream * file){
	if ( NULL == a ) {
		(*file) << "NULL" << std::endl;
	} else if ( IsPlusInfinity(a) ) {
		(*file) << "+inf" << std::endl;
	} else if ( IsMinusInfinity(a) ) {
		(*file) << "-inf" << std::endl;
	} else if ( IsUndefined(a) ) {
		(*file) << "nan" << std::endl;
	} else {
		(*file) << (*a)->_mp_size << " "
			<< (*a)->_mp_prec << " "
			<< (*a)->_mp_exp;
		for(int i = 0; i < std::abs((*a)->_mp_size); i++){
			(*file) << " " << (*a)->_mp_d[i];
		}
		(*file) << std::endl;
	}
}

mpf_t* LoadMpft(std::ifstream* file){
	std::string part;
	(*file) >> part;
	if ( part == "NULL" ) {
		return NULL;
	}
	if ( part == "+inf" ) {
		return GetPlusInfinity();
	}
	if ( part == "-inf" ) {
		return GetMinusInfinity();
	}
	if ( part == "nan" ) {
		return GetUndefined();
	}
	mpf_t* a = GetResultPointer();
	std::istringstream is_part(part);
	is_part >> (*a)->_mp_size;
	(*file) >> (*a)->_mp_prec >> (*a)->_mp_exp;
	AssertCondition(std::abs((*a)->_mp_size) <= (*a)->_mp_prec + 1, "Load of mpf_t failed.");
	(*a)->_mp_d = (mp_limb_t*) malloc(((*a)->_mp_prec + 1) * sizeof(mp_limb_t));
	for(int i = 0; i < std::abs((*a)->_mp_size); i++){
		(*file) >> (*a)->_mp_d[i];
	}
	return a;
}

} // namespace mpftoperations
} // namespace arbitraryprecisioncalculation
