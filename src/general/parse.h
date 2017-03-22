/**
* @file   parse.h 
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

#ifndef HIGH_PRECISION_PSO_GENERAL_PARSE_H_
#define HIGH_PRECISION_PSO_GENERAL_PARSE_H_

#include <string>
#include <vector>

#include "function/function.h"
#include "general/general_objects.h"
#include "random_number_generator.h"
#include "statistics/direct_statistics.h"
#include "statistics/specific_statistical_evaluations.h"

namespace highprecisionpso {

/**
* @brief Contains a large set of functions for parsing the configuration file.
*/
namespace parse {

/**
 * @brief Parses the given vector for an operation starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of an operation.
 * If nothing can be parsed as an operation then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If an operation can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed operation if the parsing process was successful and NULL otherwise.
 */
Operation* ParseOperation(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
/**
 * @brief Parses the given vector for a pair combination operation starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a pair combination operation.
 * If nothing can be parsed as a pair combination operation then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a pair combination operation can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed pair combination operation if the parsing process was successful and NULL otherwise.
 */
PairReduceOperation* ParsePairCombinationOperation(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
/**
 * @brief Parses the given vector for a constant evaluation starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a constant evaluation.
 * If nothing can be parsed as a constant evaluation then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a constant evaluation can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed constant evaluation if the parsing process was successful and NULL otherwise.
 */
ConstantEvaluation* ParseConstantEvaluation(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
/**
 * @brief Parses the given vector for a vector reduce operation starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a vector reduce operation.
 * If nothing can be parsed as a vector reduce operation then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a vector reduce operation can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed vector reduce operation if the parsing process was successful and NULL otherwise.
 */
VectorReduceOperation* ParseVectorReduceOperation(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);

/**
 * @brief Parses the given vector for a specific statistical evaluation starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a specific statistical evaluation.
 * If nothing can be parsed as a specific statistical evaluation then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a specific statistical evaluation can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed specific statistical evaluation if the parsing process was successful and NULL otherwise.
 */
SpecificStatisticalEvaluation* ParseSpecificStatistic(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
/**
 * @brief Parses the given vector for a combined specific statistical evaluation starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a combined specific statistical evaluation.
 * If nothing can be parsed as a combined specific statistical evaluation then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a combined specific statistical evaluation can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed combined specific statistical evaluation if the parsing process was successful and NULL otherwise.
 */
SpecificStatisticalEvaluation* ParseCombineSpecificStatistic(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
/**
 * @brief Parses the given vector for a reduce operator starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a reduce operator.
 * If nothing can be parsed as a reduce operator then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a reduce operator can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed reduce operator if the parsing process was successful and NULL otherwise.
 */
Statistic* ParseReduceOperator(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
/**
 * @brief Parses the given vector for a statistic starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a statistic.
 * If nothing can be parsed as a statistic then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a statistic can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed statistic if the parsing process was successful and NULL otherwise.
 */
Statistic* ParseStatistic(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
/**
 * @brief Parses the given vector for a combined statistic starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a combined statistic.
 * If nothing can be parsed as a combined statistic then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a combined statistic can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed combined statistic if the parsing process was successful and NULL otherwise.
 */
Statistic* ParseCombineStatistic(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);

/**
 * @brief Parses the given vector for a specific function starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a specific function.
 * If nothing can be parsed as a specific function then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a specific function can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed specific function if the parsing process was successful and NULL otherwise.
 */
SpecificFunction* ParseSpecificFunction(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
/**
 * @brief Parses the given vector for a combined specific function starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a combined specific function.
 * If nothing can be parsed as a combined specific function then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a combined specific function can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed combined specific function if the parsing process was successful and NULL otherwise.
 */
SpecificFunction* ParseCombineSpecificFunction(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
/**
 * @brief Parses the given vector for a function reduce operator starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a function reduce operator.
 * If nothing can be parsed as a function reduce operator then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a function reduce operator can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed function reduce operator if the parsing process was successful and NULL otherwise.
 */
Function* ParseFunctionReduceOperator(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
/**
 * @brief Parses the given vector for a standard function starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a standard function.
 * If nothing can be parsed as a standard function then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a standard function can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed standard function if the parsing process was successful and NULL otherwise.
 */
Function* ParseStandardFunction(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
/**
 * @brief Parses the given vector for a function starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a function.
 * If nothing can be parsed as a function then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a function can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed function if the parsing process was successful and NULL otherwise.
 */
Function* ParseFunction(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
/**
 * @brief Parses the given vector for a combined function starting at the given index.
 *
 * The parameter parsed_parameters will store the number of parsed parameters after successful parsing of a combined function.
 * If nothing can be parsed as a combined function then the parameter parsed_parameters will store the initial number of parsed parameters at the time when the function is called.
 * If a combined Function can be parsed partially but without success then the parameter parsed_parameters will store the number of elements of the specification vector.
 *
 * @param parameters The vector containing the specification.
 * @param parsed_parameters The number of already parsed parameters by previous functions.
 *
 * @return A pointer to the parsed combined Function if the parsing process was successful and NULL otherwise.
 */
Function* ParseCombineFunction(const std::vector<std::string> & parameters, unsigned int & parsed_parameters);
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

/**
 * @brief Prints the supplied parameters to stderr and let the program terminate with exit code 1.
 *
 * This function is called if a command in the configuration file can not be parsed.
 * The supplied command will be printed to stderr with an error message.
 * Finally the function let the program terminate with exit code 1.
 *
 * @param parameters The command of the configuration file which can not be parsed.
 */
void SignalInvalidCommand(const std::vector<std::string> & parameters);

} // namespace parse
} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_GENERAL_PARSE_H_ */
