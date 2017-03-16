/**
* @file   function.cpp
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

#include "function/function.h"

#include <sstream>

#include "general/check_condition.h"
#include "general/configuration.h"
#include "general/operations.h"

namespace highprecisionpso {

bool FUNCTION_EVALUATE_ALREADY_CALLED = false;

// lowerBound and upperBound are correctly set at the end of
// configuration::readConfigFile(..)
// it fills up the vectors if not all dimensions are set with
// values according to the previous dimensions
// and it removes dimensions if there are too much.
Function::Function(){
}

Function::Function(double lower_Bound, double upper_Bound)
	: search_space_lower_bound_ (std::vector<double>(1, lower_Bound))
	, search_space_upper_bound_ (std::vector<double>(1, upper_Bound)) {}

Function::Function(std::vector<double> lower_Bound, std::vector<double> upper_Bound)
	: search_space_lower_bound_ (lower_Bound)
	, search_space_upper_bound_ (upper_Bound) {}

mpf_t* Function::Evaluate(const std::vector<mpf_t*> & pos){
	AssertCondition(!FUNCTION_EVALUATE_ALREADY_CALLED, "Recursive function evaluation. This is not allowed.");
	if(configuration::g_function_behavior_outside_of_bounds_mode == configuration::FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_INFINITY){
		AssertCondition(((int)pos.size()) == configuration::g_dimensions, "Dimension mismatch: function is called with a position which has different number of dimensions as specified.");
		bool infinity = false;
		std::vector<mpf_t*> lower = GetLowerSearchSpaceBound();
		std::vector<mpf_t*> upper = GetUpperSearchSpaceBound();
		for(int i = 0; i < configuration::g_dimensions; i++){
			if(mpftoperations::Compare((lower[i]), (pos[i])) > 0 ||
				mpftoperations::Compare((pos[i]), (upper[i])) > 0) {
				infinity = true;
				break;
			}
		}
		vectoroperations::ReleaseValues(lower);
		vectoroperations::ReleaseValues(upper);
		if(infinity){
			return mpftoperations::GetPlusInfinity();
		}
	} else if(configuration::g_function_behavior_outside_of_bounds_mode == configuration::FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_PERIODIC){
		AssertCondition(((int)pos.size()) == configuration::g_dimensions, "Dimension mismatch: function is called with a position which has different number of dimensions as specified.");
		std::vector<mpf_t*> eval_pos;
		std::vector<mpf_t*> lower = GetLowerSearchSpaceBound();
		std::vector<mpf_t*> upper = GetUpperSearchSpaceBound();
		for(int i = 0; i < configuration::g_dimensions; i++){
			if(mpftoperations::Compare((lower[i]), (pos[i])) > 0 ||
				mpftoperations::Compare((pos[i]), (upper[i])) > 0) {

				mpf_t* mod_num = mpftoperations::Subtract(upper[i], lower[i]);
				mpf_t* distance_away = mpftoperations::Subtract(pos[i], lower[i]);
				mpf_t* intervals_away = mpftoperations::Divide(distance_away, mod_num);
				mpf_t* interval_move = mpftoperations::Floor(intervals_away);
				mpf_t* distance_move = mpftoperations::Multiply(interval_move, mod_num);
				mpf_t* actual_pos = mpftoperations::Subtract(pos[i], distance_move);
				while(mpftoperations::Compare(lower[i], actual_pos) > 0) {
					mpf_t* tmp = mpftoperations::Add(actual_pos, mod_num);
					std::swap(tmp, actual_pos);
					mpftoperations::ReleaseValue(tmp);
				}
				while(mpftoperations::Compare(actual_pos, upper[i]) > 0) {
					mpf_t* tmp = mpftoperations::Subtract(actual_pos, mod_num);
					std::swap(tmp, actual_pos);
					mpftoperations::ReleaseValue(tmp);
				}
				eval_pos.push_back(actual_pos);
				mpftoperations::ReleaseValue(mod_num);
				mpftoperations::ReleaseValue(distance_away);
				mpftoperations::ReleaseValue(intervals_away);
				mpftoperations::ReleaseValue(interval_move);
				mpftoperations::ReleaseValue(distance_move);
			} else {
				eval_pos.push_back(mpftoperations::Clone(pos[i]));
			}
		}
		vectoroperations::ReleaseValues(lower);
		vectoroperations::ReleaseValues(upper);
		FUNCTION_EVALUATE_ALREADY_CALLED = true;
		mpf_t* result = Eval(eval_pos);
		FUNCTION_EVALUATE_ALREADY_CALLED = false;
		vectoroperations::ReleaseValues(eval_pos);
		return result;
	}
	FUNCTION_EVALUATE_ALREADY_CALLED = true;
	mpf_t* result = Eval(pos);
	FUNCTION_EVALUATE_ALREADY_CALLED = false;
	return result;
}

std::vector<mpf_t*> Function::GetLowerSearchSpaceBound() {
	if(search_space_lower_bound_.size() < (unsigned int)configuration::g_dimensions)InitSearchSpaceBounds();
	AssertCondition(search_space_lower_bound_.size() >= (unsigned int)configuration::g_dimensions, "Function::InitSearchSpaceBounds does not work.");
	if(search_space_lower_bound_.size() > (unsigned int)configuration::g_dimensions){
		auto copy_search_space_lower_bound = search_space_lower_bound_;
		copy_search_space_lower_bound.resize(configuration::g_dimensions);
		return vectoroperations::GetVector(copy_search_space_lower_bound);
	} else {
		return vectoroperations::GetVector(search_space_lower_bound_);
	}
}

std::vector<mpf_t*> Function::GetUpperSearchSpaceBound() {
	if(search_space_upper_bound_.size() < (unsigned int)configuration::g_dimensions)InitSearchSpaceBounds();
	AssertCondition(search_space_upper_bound_.size() >= (unsigned int)configuration::g_dimensions, "Function::InitSearchSpaceBounds does not work.");
	if(search_space_upper_bound_.size() > (unsigned int)configuration::g_dimensions){
		auto copy_search_space_upper_bound = search_space_upper_bound_;
		copy_search_space_upper_bound.resize(configuration::g_dimensions);
		return vectoroperations::GetVector(copy_search_space_upper_bound);
	} else {
		return vectoroperations::GetVector(search_space_upper_bound_);
	}
}

mpf_t* Function::DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d){
	mpf_t* start_distance = mpftoperations::ToMpft(0.001);
	mpf_t* res = DistanceTo1DLocalOptimumTernarySearch(pos, d, start_distance);
	mpftoperations::ReleaseValue(start_distance);
	return res;
}

mpf_t* Function::DistanceTo1DLocalOptimumTernarySearch(const std::vector<mpf_t*> & pos, int d, mpf_t* start_distance){
	mpf_t* min_pos;
	mpf_t* max_pos;
	{
		std::vector<mpf_t*> low_bound = GetLowerSearchSpaceBound();
		std::vector<mpf_t*> high_bound = GetUpperSearchSpaceBound();
		min_pos = low_bound[d];
		max_pos = high_bound[d];
		low_bound[d] = NULL;
		high_bound[d] = NULL;
		vectoroperations::ReleaseValues(low_bound);
		vectoroperations::ReleaseValues(high_bound);
	}
	if( (mpftoperations::Compare(min_pos, pos[d]) > 0) ||
			(mpftoperations::Compare(max_pos, pos[d]) < 0) ) {
		// If the position is outside of the search space
		// then the distance to the search space is returned.
		mpf_t* res;
		if(mpftoperations::Compare(min_pos, pos[d]) > 0){
			res = mpftoperations::Subtract(min_pos, pos[d]);
		} else {
			res = mpftoperations::Subtract(pos[d], max_pos);
		}
		mpftoperations::ReleaseValue(min_pos);
		mpftoperations::ReleaseValue(max_pos);
		return res;
	}

	AssertCondition(mpftoperations::Compare(start_distance, 0.0) > 0, "distance_to_1D_local_optimum_ternary_search has to be called with a positive value.");
	mpf_t* low_position = NULL;
	mpf_t* high_position = NULL;
	std::vector<mpf_t*> current_position = vectoroperations::Clone(pos);
	mpf_t* mid_position = current_position[d];
	current_position[d] = NULL;
	mpf_t* diff = mpftoperations::Clone(start_distance);
	mpf_t* value = Evaluate(pos);

	{
		mpf_t* next_position = mpftoperations::Add(mid_position, diff);
		if( mpftoperations::Compare(next_position, max_pos) > 0){
			high_position = mpftoperations::Clone(max_pos);
			mpftoperations::ReleaseValue(next_position);
		} else {
			current_position[d] = next_position;
			mpf_t* next_value = Evaluate(current_position);
			if(mpf_cmp(*next_value, *value) < 0 ){
				low_position = mid_position;
				mid_position = next_position;
				mpftoperations::ReleaseValue(value);
				value = next_value;
			} else {
				high_position = next_position;
				mpftoperations::ReleaseValue(next_value);
			}
		}
		AssertCondition(high_position != NULL || low_position != NULL, "Bounds are not available.");
		current_position[d] = NULL;
		while(high_position == NULL || low_position == NULL){

			bool high_position_unset = (high_position == NULL);
			if(high_position_unset){
				next_position = mpftoperations::Add(mid_position, diff);
				if( mpftoperations::Compare(next_position, max_pos) > 0){
					high_position = mpftoperations::Clone(max_pos);
					mpftoperations::ReleaseValue(next_position);
					continue;
				}
			} else { // low_position unset
				next_position = mpftoperations::Subtract(mid_position, diff);
				if( mpftoperations::Compare(next_position, min_pos) < 0){
					low_position = mpftoperations::Clone(min_pos);
					mpftoperations::ReleaseValue(next_position);
					continue;
				}
			}
			current_position[d] = next_position;
			mpf_t* next_value = Evaluate(current_position);
			if(mpf_cmp(*next_value, *value) < 0){
				// next_value is better than current value:
				//  -> move search space
				//  -> update function value of mid_position: "value"
				//  -> increase value of diff
				if(high_position_unset){
					mpftoperations::ReleaseValue(low_position);
					low_position = mid_position;
					mid_position = next_position;
				} else {
					mpftoperations::ReleaseValue(high_position);
					high_position = mid_position;
					mid_position = next_position;
				}
				{
					mpftoperations::ReleaseValue(value);
					value = next_value;
				}
				{
					mpf_t* tmp = mpftoperations::Multiply(diff, 2.0);
					std::swap(tmp, diff);
					mpftoperations::ReleaseValue(tmp);
				}
			} else {
				// current value is better than next_value:
				//  -> set the unset bound
				//  -> free the useless function value
				if(high_position_unset){
					high_position = next_position;
				} else {
					low_position = next_position;
				}
				mpftoperations::ReleaseValue(next_value);
			}
		}
		current_position[d] = NULL;
	}
	mpftoperations::ReleaseValue(diff);
	mpftoperations::ReleaseValue(min_pos);
	mpftoperations::ReleaseValue(max_pos);


	int current_precision = mpf_get_default_prec();
	int equal_evaluation_counter = 0;
	for(int iteration_counter = 0; iteration_counter < current_precision * 3 && equal_evaluation_counter < 10; iteration_counter++){
		diff = mpftoperations::Subtract(high_position, low_position);
		mpf_t* diff_half = mpftoperations::Multiply(diff, 0.5);
		mpf_t* interval_mid = mpftoperations::Add(low_position, diff_half);
		mpf_t* actual_diff;
		if(mpf_cmp(*interval_mid, *mid_position) < 0) {
			actual_diff = mpftoperations::Multiply(diff, 0.4);
		} else {
			actual_diff = mpftoperations::Multiply(diff, 0.6);
		}
		mpf_t* next_position = mpftoperations::Add(actual_diff, low_position);
		mpftoperations::ReleaseValue(diff);
		mpftoperations::ReleaseValue(diff_half);
		mpftoperations::ReleaseValue(actual_diff);
		mpftoperations::ReleaseValue(interval_mid);
		current_position[d] = next_position;
		mpf_t* next_value = Evaluate(current_position);
		current_position[d] = NULL;
		int cmp_result = mpf_cmp(*next_value, *value);
		if(cmp_result == 0){
			++equal_evaluation_counter;
		} else {
			equal_evaluation_counter = 0;
		}
		if(cmp_result <= 0){
			std::swap(value, next_value);
			std::swap(mid_position, next_position);
		}
		mpftoperations::ReleaseValue(next_value);
		if(mpf_cmp(*next_position, *mid_position) < 0){
			std::swap(low_position, next_position);
		} else {
			std::swap(high_position, next_position);
		}
		mpftoperations::ReleaseValue(next_position);
	}
	mpf_t* best_position = mid_position;
	mpf_t* res_signed = mpftoperations::Subtract(pos[d], best_position);
	mpf_t* res = mpftoperations::Abs(res_signed);
	mpftoperations::ReleaseValue(low_position);
	mpftoperations::ReleaseValue(high_position);
	mpftoperations::ReleaseValue(best_position);
	mpftoperations::ReleaseValue(res_signed);
	mpftoperations::ReleaseValue(value);
	vectoroperations::ReleaseValues(current_position);
	return res;
}

void Function::SetFunctionBound(SearchSpaceBound* search_space_bound){
	InitSearchSpaceBounds();
	for(int d = std::max(0, search_space_bound->dimension_from); d <= std::min(configuration::g_dimensions - 1, search_space_bound->dimension_to); d++) {
		search_space_lower_bound_[d] = search_space_bound->lower_search_space_bound;
		search_space_upper_bound_[d] = search_space_bound->upper_search_space_bound;
	}
}

void Function::InitSearchSpaceBounds(){
	if(search_space_lower_bound_.size() == 0){
		search_space_lower_bound_.resize(configuration::g_dimensions, -100.0);
	}
	if(search_space_upper_bound_.size() == 0){
		search_space_upper_bound_.resize(configuration::g_dimensions, 100.0);
	}
	if(search_space_lower_bound_.size() < (unsigned int) configuration::g_dimensions){
		search_space_lower_bound_.resize(configuration::g_dimensions, search_space_lower_bound_[search_space_lower_bound_.size() - 1]);
	}
	if(search_space_upper_bound_.size() < (unsigned int) configuration::g_dimensions){
		search_space_upper_bound_.resize(configuration::g_dimensions, search_space_upper_bound_[search_space_upper_bound_.size() - 1]);
	}
}

} // namespace highprecisionpso
