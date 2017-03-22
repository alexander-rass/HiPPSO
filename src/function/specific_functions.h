/**
* @file   specific_functions.h
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_SPECIFIC_FUNCTIONS_H_
#define HIGH_PRECISION_PSO_FUNCTION_SPECIFIC_FUNCTIONS_H_

#include "function/function.h"
#include "general/general_objects.h"

namespace highprecisionpso {

/**
* @brief This class implements a specific function with an evaluation function that always evaluates to a vector of a predefined constant.
*/
class ConstantSpecificFunction : public SpecificFunction {
public:
	/**
	* @brief The constructor, where the constant can be specified.
	*
	* @param constant_evaluation The constant evaluation, which does the actual evaluation.
	*/
	ConstantSpecificFunction(ConstantEvaluation* constant_evaluation);
	/**
	* @brief Returns the evaluation of the current constant specific function.
	*
	* @param pos The position.
	*
	* @return The constant vector.
	*/
	std::vector<mpf_t*> Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();

private:
	ConstantEvaluation* constant_evaluation_;
};

/**
* @brief This class implements a specific function with an evaluation function that clones the input.
*/
class IdentitySpecificFunction : public SpecificFunction{
public:
	/**
	* @brief Returns the evaluation of the cloned position vector.
	*
	* @param pos The position.
	*
	* @return The cloned vector.
	*/
	std::vector<mpf_t*> Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
};

/**
* @brief This class implements a specific function, which combines an operation with a specific function.
*/
class OperatedSpecificFunction : public SpecificFunction {
public:
	/**
	* @brief The constructor, which specifies the operation and the specific function, which should be combined.
	*
	* @param operation The operation.
	* @param specific_function The specific function.
	*/
	OperatedSpecificFunction(Operation* operation, SpecificFunction* specific_function);
	/**
	* @brief Returns the evaluation of the specified operation on the specified specific function.
	*
	* @param pos The position.
	*
	* @return The evaluation of the position.
	*/
	std::vector<mpf_t*> Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();

private:
	Operation* operation_;
	SpecificFunction* specific_function_;
};

/**
* @brief This class implements a specific function, which combines two specific functions with an operation.
*/
class CombineSpecificFunction : public SpecificFunction {
public:
	/**
	* @brief The constructor, which specifies the combination operation and the two specific functions, which should be combined.
	*
	* @param operation The combine operation.
	* @param operator1 The first parameter of the operation.
	* @param operator2 The second parameter of the operation.
	*/
	CombineSpecificFunction(PairReduceOperation* operation, SpecificFunction* operator1, SpecificFunction* operator2);
	/**
	* @brief Returns the evaluation of the combination of the two specific functions.
	*
	* @param pos The position.
	*
	* @return The evaluation of the position.
	*/
	std::vector<mpf_t*> Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();

private:
	PairReduceOperation* operation_;
	SpecificFunction* operator1_;
	SpecificFunction* operator2_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_SPECIFIC_FUNCTIONS_H_ */
