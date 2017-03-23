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
namespace configuration {

int g_initial_precision = 32;
bool g_initial_precision_already_set = false;
int g_precision_safety_margin = 32;

enum CheckPrecisionMode g_check_precision_mode = CHECK_PRECISION_ALWAYS_EXCEPT_STATISTICS;
double g_check_precision_probability = 1.00;
// <= 0 -> surely not check
// >= 1 -> surely check
// > 0 && < 1 -> randomly check
bool g_increase_precision = false;

int g_output_precision = 5;

RandomNumberGenerator* g_standard_random_number_generator = new FastM2P63LinearCongruenceRandomNumberGenerator(1571204578482947281ULL, 12345678901234567ULL, 0);

void Init(){

	g_initial_precision = 32;
	g_initial_precision_already_set = false;
	g_precision_safety_margin = 32;

	g_check_precision_mode = CHECK_PRECISION_ALWAYS_EXCEPT_STATISTICS;
	g_check_precision_probability = 1.00;
	g_increase_precision = false;

	g_output_precision = 5;

	g_standard_random_number_generator = new FastM2P63LinearCongruenceRandomNumberGenerator(1571204578482947281ULL, 12345678901234567ULL, 0);
}

} // namespace configuration
} // namespace arbitraryprecisioncalculation
