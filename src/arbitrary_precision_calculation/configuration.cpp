/**
* @file   arbitrary_precision_calculation/configuration.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   June, 2013
* @brief  This file contains general information about the configuration for running the particle swarm optimization algorithm.
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

#include "arbitrary_precision_calculation/configuration.h"

#include "arbitrary_precision_calculation/random_number_generator.h"

namespace arbitraryprecisioncalculation {

int Configuration::initial_precision_ = 32;
bool Configuration::initial_precision_already_set_ = false;
int Configuration::precision_safety_margin_ = 32;

arbitraryprecisioncalculation::configuration::CheckPrecisionMode Configuration::check_precision_mode_ = arbitraryprecisioncalculation::configuration::CHECK_PRECISION_ALWAYS_EXCEPT_STATISTICS;
double Configuration::check_precision_probability_ = 1.00;
// <= 0 -> surely not check
// >= 1 -> surely check
// > 0 && < 1 -> randomly check
bool Configuration::increase_precision_recommended_ = false;

int Configuration::output_precision_ = 5;

RandomNumberGenerator* Configuration::standard_random_number_generator_ = new FastM2P63LinearCongruenceRandomNumberGenerator(1571204578482947281ULL, 12345678901234567ULL, 0);

void Configuration::Init(){

	initial_precision_ = 32;
	initial_precision_already_set_ = false;
	precision_safety_margin_ = 32;

	check_precision_mode_ = arbitraryprecisioncalculation::configuration::CHECK_PRECISION_ALWAYS_EXCEPT_STATISTICS;
	check_precision_probability_ = 1.00;
	increase_precision_recommended_ = false;

	output_precision_ = 5;

	standard_random_number_generator_ = new FastM2P63LinearCongruenceRandomNumberGenerator(1571204578482947281ULL, 12345678901234567ULL, 0);
}


arbitraryprecisioncalculation::configuration::CheckPrecisionMode Configuration::getCheckPrecisionMode() {
	return check_precision_mode_;
}

void Configuration::setCheckPrecisionMode(
		arbitraryprecisioncalculation::configuration::CheckPrecisionMode checkPrecisionMode) {
	check_precision_mode_ = checkPrecisionMode;
}

double Configuration::getCheckPrecisionProbability() {
	return check_precision_probability_;
}

void Configuration::setCheckPrecisionProbability(double checkPrecisionProbability) {
	check_precision_probability_ = checkPrecisionProbability;
}

bool Configuration::isIncreasePrecisionRecommended() {
	return increase_precision_recommended_;
}

void Configuration::ResetIncreasePrecisionRecommended() {
	increase_precision_recommended_ = false;
}

void Configuration::RecommendIncreasePrecision() {
	increase_precision_recommended_ = true;
}

int Configuration::getInitialPrecision() {
	return initial_precision_;
}

void Configuration::setInitialPrecision(int initialPrecision) {
	initial_precision_ = initialPrecision;
	mpf_set_default_prec(initialPrecision);
	if(!initial_precision_already_set_){
		initial_precision_already_set_ = true;
		setPrecisionSafetyMargin(initialPrecision);
	}
}

int Configuration::getOutputPrecision() {
	return output_precision_;
}

void Configuration::setOutputPrecision(int outputPrecision) {
	output_precision_ = outputPrecision;
}

int Configuration::getPrecisionSafetyMargin() {
	return precision_safety_margin_;
}

void Configuration::setPrecisionSafetyMargin(int precisionSafetyMargin) {
	precision_safety_margin_ = precisionSafetyMargin;
	if(!initial_precision_already_set_){
		initial_precision_already_set_ = true;
		setInitialPrecision(precisionSafetyMargin);
	}
}

RandomNumberGenerator* Configuration::getStandardRandomNumberGenerator() {
	return standard_random_number_generator_;
}


void Configuration::setStandardRandomNumberGenerator(
		RandomNumberGenerator* standardRandomNumberGenerator) {
	standard_random_number_generator_ = standardRandomNumberGenerator;
}

} // namespace arbitraryprecisioncalculation
