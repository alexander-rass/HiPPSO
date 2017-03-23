/**
* @file   arbitrary_precision_calculation/configuration.h
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

#ifndef ARBITRARY_PRECISION_CALCULATION_CONFIGURATION_H_
#define ARBITRARY_PRECISION_CALCULATION_CONFIGURATION_H_

#include <string>
#include <vector>

#include "arbitrary_precision_calculation/random_number_generator.h"

/**
 * @brief base namespace for arbitrary precision calculation.
 */
namespace arbitraryprecisioncalculation {

/**
* @brief Contains the variables which store information about the configuration for the arbitrary precision calculation.
*/
namespace configuration {

/**
* @brief Specifies the initial precision of the mpf_t data type.
*/
extern int g_initial_precision;
/**
* @brief Remembers whether the initial precision is already set manually.
* If this is not the case then a specification of the precision also overwrites the initial precision.
*/
extern bool g_initial_precision_already_set;
/**
* @brief Specifies the additional precision. If precision checks are activated then the check whether the precision should be increased uses this number to increase the calculated needed precision additionally.
*/
extern int g_precision_safety_margin;

/**
* @brief Specification possibilities in which cases precision checks occur.
*/
enum CheckPrecisionMode {
	/**
	* @brief If there are statistical calculations then no precision is checked. Beside that the precision is checked on each addition and subtraction.
	*/
	CHECK_PRECISION_ALWAYS_EXCEPT_STATISTICS,
	/**
	* @brief The precision is checked on each addition and subtraction.
	*/
	CHECK_PRECISION_ALWAYS,
	/**
	* @brief The precision is never checked.
	*/
	CHECK_PRECISION_NEVER
};
/**
* @brief Stores the information in which cases precision checks occur.
*/
extern CheckPrecisionMode g_check_precision_mode;
/**
* @brief Specifies the probability whether a precision check is actually done.
*/
extern double g_check_precision_probability;
/**
* @brief Specifies whether the precision will be increased shortly.
*/
extern bool g_increase_precision;

/**
* @brief Specifies the number of digits for the output of mpf_t values while using printing functions.
* The value -1 specifies that the output precision should match the current precision of the mpf_t data type.
*/
extern int g_output_precision;

/**
* @brief Specifies the random number generator for the particle swarm optimization algorithm.
*/
extern RandomNumberGenerator* g_standard_random_number_generator;

/**
* @brief Resets all variables in the configuration section to the default values.
*/
void Init();

} // namespace configuration
} // namespace arbitraryprecisioncalculation

#endif /* ARBITRARY_PRECISION_CALCULATION_CONFIGURATION_H_ */
