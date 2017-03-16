/**
* @file   manual_function.h
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_MANUAL_FUNCTION_H_
#define HIGH_PRECISION_PSO_FUNCTION_MANUAL_FUNCTION_H_

#include <gmp.h>
#include <string>
#include <vector>

#include "function/function.h"
#include "general/general_objects.h"

namespace highprecisionpso {

/**
* @brief This class is a function, which merges a specific function to a function with a vector merge operation.
*/
class FunctionMergeOperator : public Function {
public:
	/**
	* @brief The constructor, where the specific function, which should be merged, and the vector merge operation can be specified.
	*
	* @param vector_merge_operation The merge operation.
	* @param specific_function The specific function, which should be merged.
	*/
	FunctionMergeOperator(VectorMergeOperation* vector_merge_operation, SpecificFunction* specific_function);
	/**
	* @brief Merges the result of the stored specific function by the stored vector merge operation.
	*
	* @param pos The position for evaluation of the specific function.
	*
	* @return The merged result.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();

private:
	VectorMergeOperation* vector_merge_operation_;
	SpecificFunction* specific_function_;
};

/**
* @brief This class is a function, which applies some modification to the result of another function.
*/
class ModifiedFunction : public Function {
public:
	/**
	* @brief The constructor, where the modification, which should be applied, and the other function can be specified.
	*
	* @param modification The modification, which should be applied.
	* @param function The function.
	*/
	ModifiedFunction(Modification* modification, Function* function);
	/**
	* @brief Evaluates the stored function on the given position and applies then the stored modification.
	*
	* @param pos The position
	*
	* @return The modified function evaluation.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();

private:
	Modification* modification_;
	Function* function_;
};

/**
* @brief This class implements a function which combines two functions with an operation.
*/
class CombineFunction : public Function {
public:
	/**
	* @brief The constructor, which specifies the combination operation and the two functions, which should be combined.
	*
	* @param operation The combine operation.
	* @param operator1 The first parameter of the operation.
	* @param operator2 The second parameter of the operation.
	*/
	CombineFunction(PairCombinationOperation* operation, Function* operator1, Function* operator2);
	/**
	* @brief Returns the evaluation of the combination of the two functions.
	*
	* @param pos The position.
	*
	* @return The evaluation of the position.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();

private:
	PairCombinationOperation* operation_;
	Function* operator1_;
	Function* operator2_;
};

/**
* @brief This class implements a specific function with an evaluation function that always evaluates to a vector of a predefined constant.
*/
class ConstantFunction : public Function {
public:
	/**
	* @brief The constructor, where the constant can be specified.
	*
	* @param constant_evaluation The constant evaluation, which does the actual evaluation.
	*/
	ConstantFunction(ConstantEvaluation* constant_evaluation);
	/**
	* @brief Returns the evaluation of the current constant function.
	*
	* @param pos The position.
	*
	* @return The constant value.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();

private:
	ConstantEvaluation* constant_evaluation_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_MANUAL_FUNCTION_H_ */

