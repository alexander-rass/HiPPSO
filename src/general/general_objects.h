/**
* @file   general/general_objects.h
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

#ifndef HIGH_PRECISION_PSO_GENERAL_GENERAL_OBJECTS_H_
#define HIGH_PRECISION_PSO_GENERAL_GENERAL_OBJECTS_H_

#include <gmp.h>
#include <string>
#include <vector>

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This class stores a program version.
*/
class ProgramVersion{
public:
	/**
	* @brief The major program version.
	*/
	int version;
	/**
	* @brief The minor program version.
	*/
	int version_minor;
	/**
	* @brief The patch level.
	*/
	int version_patch_level;

	/**
	* @brief A constructor.
	*
	* @param version The major program version.
	* @param version_minor The minor program version.
	* @param version_patch_level The patch level.
	*/
	ProgramVersion(int version, int version_minor, int version_patch_level);
	/**
	* @brief A constructor.
	*
	* @param version_string This version string should have the format "<version>.<version_minor>.<version_patch_level>".
	*                       Then the correct ProgramVersion is generated.
	*/
	ProgramVersion(std::string version_string);
	/**
	* @brief Returns a string, which represents the stored program version in the format "<version>.<version_minor>.<version_patch_level>" 
	*
	* @return The program version as a string.
	*/
	std::string GetCompleteVersion() const;
};

/**
* @brief Comparison operator "greater" between program versions.
*
* @param a The first program version which should be compared.
* @param b The second program version which should be compared.
*
* @return The result of the comparison.
*
* Most important is version, second important is version_minor and least important is version_patch_level.
*/
bool operator>(const ProgramVersion & a, const ProgramVersion & b);
/**
* @brief Comparison operator "less or equal" between program versions.
*
* @param a The first program version which should be compared.
* @param b The second program version which should be compared.
*
* @return The result of the comparison.
*
* Most important is version, second important is version_minor and least important is version_patch_level.
*/
bool operator<=(const ProgramVersion & a, const ProgramVersion & b);
/**
* @brief Comparison operator "greater or equal" between program versions.
*
* @param a The first program version which should be compared.
* @param b The second program version which should be compared.
*
* @return The result of the comparison.
*
* Most important is version, second important is version_minor and least important is version_patch_level.
*/
bool operator>=(const ProgramVersion & a, const ProgramVersion & b);
/**
* @brief Comparison operator "equal" between program versions.
*
* @param a The first program version which should be compared.
* @param b The second program version which should be compared.
*
* @return The result of the comparison.
*
* Most important is version, second important is version_minor and least important is version_patch_level.
*/
bool operator==(const ProgramVersion & a, const ProgramVersion & b);
/**
* @brief Comparison operator "less" between program versions.
*
* @param a The first program version which should be compared.
* @param b The second program version which should be compared.
*
* @return The result of the comparison.
*
* Most important is version, second important is version_minor and least important is version_patch_level.
*/
bool operator<(const ProgramVersion & a, const ProgramVersion & b);

/************** VECTOR REDUCE OPERATIONS ***********************/
/**
* @brief This an abstract class. It supplies the interface
* for reducing a vector of mpf_t values to a single value.
*/
class VectorReduceOperation {
public:
	/**
	 * @brief The destructor.
	 */
	virtual ~VectorReduceOperation(){}
	/**
	* @brief Reduces the entries of the vector to a single value.
	*
	* @param vec The vector which should be reduced.
	*
	* @return The reduced value.
	*/
	virtual mpf_t* Evaluate(const std::vector<mpf_t*> & vec) = 0;

	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
};

/************************* VECTOR REDUCE OPERATIONS ******************************/
/**
* @brief This vector reduce operation reduces a vector by ignoring all values of the vector except the specified one.
*/
class SpecificIdReduceOperation : public VectorReduceOperation {
public:
	/**
	* @brief The constructor, where the remaining index can be specified.
	*
	* @param id The remaining index.
	*/
	SpecificIdReduceOperation(int id);
	/**
	* @brief Reduces the vector by ignoring all entries except the specified one.
	*
	* @param vec The vector which should be reduced.
	*
	* @return The value of the entry at the specified position.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();

private:
	/**
	* @brief The index of the value of the vector which should remain.
	*/
	const int id_;
};

/**
* @brief This vector reduce operation reduces a vector by sorting the vector
* and returning the value at the respective position of the sorted vector.
*/
class IncreasingOrderNthObjectReduceOperation : public VectorReduceOperation {
public:
	/**
	* @brief The constructor, where the remaining index can be specified.
	*
	* @param order_id The remaining index.
	*/
	IncreasingOrderNthObjectReduceOperation (int order_id);
	/**
	* @brief Reduces the vector by sorting the vector and then ignoring all entries except the specified one.
	*
	* @param vec The vector which should be reduced.
	*
	* @return The value of the entry at the specified position of the sorted vector.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();

private:
	/**
	* @brief The index of the value of the sorted vector which should remain.
	*/
	const int order_id_;
};

/**
* @brief This vector reduce operation reduces a vector by calculation of its arithmetic average.
*/
class ArithmeticAverageReduceOperation : public VectorReduceOperation {
public:
	/**
	* @brief The constructor.
	*/
	ArithmeticAverageReduceOperation();
	/**
	* @brief Reduces the vector by calculation of its arithmetic average.
	*
	* @param vec The vector which should be reduced.
	*
	* @return The arithmetic average of the vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/**
* @brief This vector reduce operation reduces a vector by calculation of its geometric average.
*/
class GeometricAverageReduceOperation : public VectorReduceOperation {
public:
	/**
	* @brief The constructor.
	*/
	GeometricAverageReduceOperation();
	/**
	* @brief Reduces the vector by calculation of its geometric average.
	*
	* @param vec The vector which should be reduced.
	*
	* @return The geometric average of the vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/**
* @brief This vector reduce operation reduces a vector by calculation of its sum.
*/
class SumReduceOperation : public VectorReduceOperation {
public:
	/**
	* @brief The constructor.
	*/
	SumReduceOperation();
	/**
	* @brief Reduces the vector by calculation of its sum.
	*
	* @param vec The vector which should be reduced.
	*
	* @return The sum of the vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/**
* @brief This vector reduce operation reduces a vector by calculation of its product.
*/
class ProductReduceOperation : public VectorReduceOperation {
public:
	/**
	* @brief The constructor.
	*/
	ProductReduceOperation();
	/**
	* @brief Reduces the vector by calculation of its product.
	*
	* @param vec The vector which should be reduced.
	*
	* @return The product of the vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/**
* @brief This vector reduce operation reduces a vector by calculation of its maximal entry.
*/
class MaximalValueReduceOperation : public VectorReduceOperation {
public:
	/**
	* @brief The constructor.
	*/
	MaximalValueReduceOperation();
	/**
	* @brief Reduces the vector by calculation of its maximal entry.
	*
	* @param vec The vector which should be reduced.
	*
	* @return The maximal entry of the vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/**
* @brief This vector reduce operation reduces a vector by calculation of its minimal entry.
*/
class MinimalValueReduceOperation : public VectorReduceOperation {
public:
	/**
	* @brief The constructor.
	*/
	MinimalValueReduceOperation();
	/**
	* @brief Reduces the vector by calculation of its minimal entry.
	*
	* @param vec The vector which should be reduced.
	*
	* @return The minimal entry of the vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/**
* @brief This vector reduce operation reduces a vector by evaluation of the given function.
*/
class FunctionEvaluationReduceOperation : public VectorReduceOperation {
public:
	/**
	* @brief The constructor, where the reducing function can be specified.
	*
	* @param function The function, which should be evaluated to reduce a vector.
	*/
	FunctionEvaluationReduceOperation(Function* function);
	/**
	* @brief Reduces the vector by evaluation of the specified function.
	*
	* @param vec The vector which should be reduced.
	*
	* @return The result of the evaluation of the specified function with input vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();

private:
	Function* function_;
};

/**
* @brief This vector reduce operation reduces a vector by evaluation of the specified objective function.
*/
class ObjectiveFunctionEvaluationReduceOperation : public VectorReduceOperation {
public:
	/**
	* @brief The constructor.
	*/
	ObjectiveFunctionEvaluationReduceOperation();
	/**
	* @brief Reduces the vector by evaluation of the specified objective function in the configuration section.
	*
	* @param vec The vector which should be reduced.
	*
	* @return The result of the evaluation of the specified objective function with input vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/************** PAIR COMBINATION OPERATIONS ********************/
/**
* @brief This class is an abstract class. It supplies the interface for reducing two mpf_t values to a single value.
*/
class PairReduceOperation {
public:
	/**
	 * @brief The destructor.
	 */
	virtual ~PairReduceOperation(){}
	/**
	* @brief Reduces two values to a single value.
	*
	* @param value1 The first value which should be reduced.
	* @param value2 The second value which should be reduced.
	*
	* @return The reduced value.
	*/
	virtual mpf_t* Evaluate(mpf_t* value1, mpf_t* value2) = 0;
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
};

/**
* @brief This pair combination operation reduces two values by addition.
*/
class AddOperation : public PairReduceOperation {
public:
	/**
	* @brief Reduces the two values by adding them.
	*
	* @param value1 The first value which should be reduced.
	* @param value2 The second value which should be reduced.
	*
	* @return The sum of value1 and value2
	*/
	mpf_t* Evaluate(mpf_t* value1, mpf_t* value2);
	std::string GetName();
};

/**
* @brief This pair combination operation reduces two values by subtraction.
*/
class SubtractOperation : public PairReduceOperation {
public:
	/**
	* @brief Reduces the two values by subtraction.
	*
	* @param value1 The first value which should be reduced.
	* @param value2 The second value which should be reduced.
	*
	* @return value1 - value2
	*/
	mpf_t* Evaluate(mpf_t* value1, mpf_t* value2);
	std::string GetName();
};

/**
* @brief This pair combination operation reduces two values by multiplication.
*/
class MultiplyOperation : public PairReduceOperation {
public:
	/**
	* @brief Reduces the two values by multiplication.
	*
	* @param value1 The first value which should be reduced.
	* @param value2 The second value which should be reduced.
	*
	* @return The product of value1 and value2
	*/
	mpf_t* Evaluate(mpf_t* value1, mpf_t* value2);
	std::string GetName();
};

/**
* @brief This pair combination operation reduces two values by division.
*/
class DivideOperation : public PairReduceOperation {
public:
	/**
	* @brief Reduces the two values by division.
	*
	* @param value1 The first value which should be reduced.
	* @param value2 The second value which should be reduced.
	*
	* @return value1 / value2
	*/
	mpf_t* Evaluate(mpf_t* value1, mpf_t* value2);
	std::string GetName();
};

/**
* @brief This pair combination operation reduces two values by calculation of the minimum.
*/
class MinOperation : public PairReduceOperation {
public:
	/**
	* @brief Reduces the two values by calculation of the minimum.
	*
	* @param value1 The first value which should be reduced.
	* @param value2 The second value which should be reduced.
	*
	* @return min ( value1 , value2 )
	*/
	mpf_t* Evaluate(mpf_t* value1, mpf_t* value2);
	std::string GetName();
};

/**
* @brief This pair combination operation reduces two values by calculation of the maximum.
*/
class MaxOperation : public PairReduceOperation {
public:
	/**
	* @brief Reduces the two values by calculation of the maximum.
	*
	* @param value1 The first value which should be reduced.
	* @param value2 The second value which should be reduced.
	*
	* @return max ( value1 , value2 )
	*/
	mpf_t* Evaluate(mpf_t* value1, mpf_t* value2);
	std::string GetName();
};

/************** CONSTANT EVALUATIONS ********************/
/**
* @brief This class is an abstract class. It supplies the interface for generating a constant value.
*/
class ConstantEvaluation {
public:
	/**
	 * @brief The destructor.
	 */
	virtual ~ConstantEvaluation(){}
	/**
	* @brief Returns the predefined constant value specified by this constant evaluation.
	*
	* @return The constant value.
	*/
	virtual mpf_t* Evaluate() = 0;
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
};

/**
* @brief This constant evaluation can calculate a predefined mpf_t value from a double value.
*/
class DoubleConstantEvaluation : public ConstantEvaluation {
public:
	/**
	* @brief The constructor, where the mpf_t value, which should be generated, can be specified by a double value.
	*
	* @param value The value which will be transformed to an mpf_t.
	*/
	DoubleConstantEvaluation(double value);
	/**
	* @brief Returns the converted double value.
	*
	* @return The constant value.
	*/
	mpf_t* Evaluate();
	std::string GetName();

private:
	const double value_;
};

/**
* @brief This constant evaluation calculates the value Pi.
*/
class PiConstantEvaluation : public ConstantEvaluation {
public:
	/**
	* @brief Returns the value of Pi.
	*
	* @return The constant value Pi.
	*/
	mpf_t* Evaluate();
	std::string GetName();
};

/**
* @brief This constant evaluation calculates the value E.
*/
class EConstantEvaluation : public ConstantEvaluation {
public:
	/**
	* @brief Returns the value of E.
	*
	* @return The constant value E.
	*/
	mpf_t* Evaluate();
	std::string GetName();
};

/**
* @brief This constant evaluation calculates the value +infinity.
*/
class PlusInfinityConstantEvaluation : public ConstantEvaluation {
public:
	/**
	* @brief Returns the value of +infinity.
	*
	* @return The constant value +infinity.
	*/
	mpf_t* Evaluate();
	std::string GetName();
};

/**
* @brief This constant evaluation calculates the value -infinity.
*/
class MinusInfinityConstantEvaluation : public ConstantEvaluation {
public:
	/**
	* @brief Returns the value of -infinity.
	*
	* @return The constant value -infinity.
	*/
	mpf_t* Evaluate();
	std::string GetName();
};

/************** VECTOR OPERATIONS *********************/
/**
* @brief This class is an abstract class. It supplies the interface to operate on vectors of mpf_t values.
*/
class VectorOperation {
public:
	/**
	 * @brief The destructor.
	 */
	virtual ~VectorOperation(){}
	/**
	* @brief Calculates a operation of the supplied vector.
	*
	* @param vec The vector which should be the base of operation.
	*
	* @return The result of the operation on the given vector.
	*/
	virtual std::vector<mpf_t*> Operate(std::vector<mpf_t*> vec) = 0;
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
};

/**
* @brief This vector operation calculates for each dimension the distance to the nearest 1-dimensional local optimum.
*/
class DistTo1DOptimumOperation : public VectorOperation {
public:
	/**
	* @brief Calculates for each dimension the distance to the nearest 1-dimensional local optimum.
	*
	* For each dimension the given position is observed.
	* The position entry in the current dimension can be varied and all other position entries are regarded as constants.
	* In this setting the nearest local optimum in respect to the specified objective function is calculated by varying the value of the current dimension.
	* (In general this is done by some kind of ternary search.)
	* Then the distance to the found local optimum is the value for the current position.
	*
	* @param vec The vector which should be observed.
	*
	* @return The distances to the local optima.
	*/
	std::vector<mpf_t*> Operate(std::vector<mpf_t*> vec);
	std::string GetName();
};

/************** OPERATIONS ****************************/
/**
* @brief This class is an abstract class. It supplies the interface to operate on single mpf_t values.
*/
class Operation {
public:
	/**
	 * @brief The destructor.
	 */
	virtual ~Operation(){}
	/**
	* @brief Calculates a operation of the supplied mpf_t value.
	*
	* @param value The value which should be operated on.
	*
	* @return The result of the operation.
	*/
	virtual mpf_t* Operate(mpf_t* value) = 0;
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
};


/**
* @brief This operation calculates the square root of the supplied value.
*/
class SqrtOperation : public Operation {
public:
	/**
	* @brief Calculates the square root of the supplied value.
	*
	* @param value The value which should be operated on.
	*
	* @return The square root of the supplied value.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();
};

/**
* @brief This operation calculates the power of the supplied value to a given exponent.
*/
class PowOperation : public Operation {
public:
	/**
	* @brief The constructor, where the exponent of the power calculation can be specified.
	*
	* @param exponent The exponent of the power calculation.
	*/
	PowOperation(double exponent);

	/**
	* @brief Calculates the power of the supplied value to the specified exponent.
	*
	* @param value The value which should be operated on.
	*
	* @return The power of the supplied value to the specified exponent.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();

private:
	const double exponent_;
};

/**
* @brief This operation calculates the logarithm with base two of the supplied value.
*/
class Log2Operation : public Operation {
public:
	/**
	* @brief Calculates the logarithm with base two of the supplied value.
	*
	* @param value The value which should be operated on.
	*
	* @return The logarithm with base two of the supplied value.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();
};

/**
* @brief This operation calculates logarithm with base two of the supplied value.
* @warning Results of operation may have very bad precision.
*/
class Log2DoubleOperation : public Operation {
public:
	/**
	* @brief Calculates the logarithm with base two of the supplied value.
	*
	* @warning The calculation uses double calculations. Therefore the result may have a very bad precision.
	* For exact results pleas use the Log2Operation and not Log2DoubleOperation!
	*
	* @param value The value which should be operated on.
	*
	* @return The logarithm with base two of the supplied value.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();
};

/**
* @brief This operation calculates the absolute value of the supplied value.
*/
class AbsOperation : public Operation {
public:
	/**
	* @brief Calculates the absolute value of the supplied value.
	*
	* @param value The value which should be operated on.
	*
	* @return The absolute value of the supplied value.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();
};

/**
* @brief This operation calculates the exponential function of the supplied value.
*/
class ExpOperation : public Operation {
public:
	/**
	* @brief Calculates the exponential function of the supplied value.
	*
	* @param value The value which should be operated on.
	*
	* @return The exponential function of the supplied value.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();
};

/**
* @brief This operation calculates the sine function of the supplied value.
*/
class SinOperation : public Operation {
public:
	/**
	* @brief Calculates the sine function of the supplied value.
	*
	* @param value The value which should be operated on.
	*
	* @return The sine function of the supplied value.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();
};

/**
* @brief This operation calculates the cosine function of the supplied value.
*/
class CosOperation : public Operation {
public:
	/**
	* @brief Calculates the cosine function of the supplied value.
	*
	* @param value The value which should be operated on.
	*
	* @return The cosine function of the supplied value.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();
};

/**
* @brief This operation calculates the tangent function of the supplied value.
*/
class TanOperation : public Operation {
public:
	/**
	* @brief Calculates the tangent function of the supplied value.
	*
	* @param value The value which should be operated on.
	*
	* @return The tangent function of the supplied value.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();
};

/**
* @brief This operation calculates the arcsine function of the supplied value.
*/
class ArcsinOperation : public Operation {
public:
	/**
	* @brief Calculates the arcsine function of the supplied value.
	*
	* @param value The value which should be operated on.
	*
	* @return The arcsine function of the supplied value.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();
};

/**
* @brief This operation calculates the arccosine function of the supplied value.
*/
class ArccosOperation : public Operation {
public:
	/**
	* @brief Calculates the arccosine function of the supplied value.
	*
	* @param value The value which should be operated on.
	*
	* @return The arccosine function of the supplied value.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();
};

/**
* @brief This operation calculates the arctangent function of the supplied value.
*/
class ArctanOperation : public Operation {
public:
	/**
	* @brief Calculates the arctangent function of the supplied value.
	*
	* @param value The value which should be operated on.
	*
	* @return The arctangent function of the supplied value.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();
};

/**
* @brief This operation calculates the logarithm with base E of the supplied value.
*/
class LogEOperation : public Operation {
public:
	/**
	* @brief Calculates the logarithm with base E of the supplied value.
	*
	* @param value The value which should be operated on.
	*
	* @return The logarithm with base E of the supplied value.
	*/
	mpf_t* Operate(mpf_t* value);
	std::string GetName();
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_GENERAL_GENERAL_OBJECTS_H_ */
