/**
* @file   arbitrary_precision_calculation/parse.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   October, 2015
* @brief  This file contains functions for parsing the configuration file.
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

#ifndef ARBITRARY_PRECISION_CALCULATION_PARSE_H_
#define ARBITRARY_PRECISION_CALCULATION_PARSE_H_

#include <string>
#include <vector>

#include "arbitrary_precision_calculation/random_number_generator.h"

namespace arbitraryprecisioncalculation {

/**
* @brief Contains a large set of functions for parsing the configuration file.
*/
namespace parse {

/**
 * @brief Parses the given vector for a random number generator starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a random number generator.
 * If nothing can be parsed as a random number generator then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a random number generator can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed random number generator if the parsing process was successful and NULL otherwise.
 */
RandomNumberGenerator* ParseRandomNumberGenerator(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);

} // namespace parse
} // namespace arbitraryprecisioncalculation

#endif /* ARBITRARY_PRECISION_CALCULATION_PARSE_H_ */
