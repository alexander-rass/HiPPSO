/**
* @file   general_objects.h
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

/************** VECTOR MERGE OPERATIONS ***********************/
/**
* @brief This an abstract class. It supplies the interface
* for merging a vector of mpf_t values to a single value.
*/
class VectorMergeOperation {
public:
	/**
	 * @brief The destructor.
	 */
	virtual ~VectorMergeOperation(){}
	/**
	* @brief Merges the entries of the vector to a single value.
	*
	* @param vec The vector which should be merged.
	*
	* @return The merged value.
	*/
	virtual mpf_t* Evaluate(const std::vector<mpf_t*> & vec) = 0;

	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
};

/************************* VECTOR MERGE OPERATIONS ******************************/
/**
* @brief This vector merge operation merges a vector by ignoring all values of the vector except the specified one.
*/
class SpecificIdMergeOperation : public VectorMergeOperation {
public:
	/**
	* @brief The constructor, where the remaining index can be specified.
	*
	* @param id The remaining index.
	*/
	SpecificIdMergeOperation(int id);
	/**
	* @brief Merges the vector by ignoring all entries except the specified one.
	*
	* @param vec The vector which should be merged.
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
* @brief This vector merge operation merges a vector by sorting the vector
* and returning the value at the respective position of the sorted vector.
*/
class IncreasingOrderNthObjectMergeOperation : public VectorMergeOperation {
public:
	/**
	* @brief The constructor, where the remaining index can be specified.
	*
	* @param order_id The remaining index.
	*/
	IncreasingOrderNthObjectMergeOperation (int order_id);
	/**
	* @brief Merges the vector by sorting the vector and then ignoring all entries except the specified one.
	*
	* @param vec The vector which should be merged
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
* @brief This vector merge operation merges a vector by calculation of its arithmetic average.
*/
class ArithmeticAverageMergeOperation : public VectorMergeOperation {
public:
	/**
	* @brief The constructor.
	*/
	ArithmeticAverageMergeOperation();
	/**
	* @brief Merges the vector by calculation of its arithmetic average.
	*
	* @param vec The vector which should be merged.
	*
	* @return The arithmetic average of the vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/**
* @brief This vector merge operation merges a vector by calculation of its geometric average.
*/
class GeometricAverageMergeOperation : public VectorMergeOperation {
public:
	/**
	* @brief The constructor.
	*/
	GeometricAverageMergeOperation();
	/**
	* @brief Merges the vector by calculation of its geometric average.
	*
	* @param vec The vector which should be merged.
	*
	* @return The geometric average of the vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/**
* @brief This vector merge operation merges a vector by calculation of its sum.
*/
class SumMergeOperation : public VectorMergeOperation {
public:
	/**
	* @brief The constructor.
	*/
	SumMergeOperation();
	/**
	* @brief Merges the vector by calculation of its sum.
	*
	* @param vec The vector which should be merged.
	*
	* @return The sum of the vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/**
* @brief This vector merge operation merges a vector by calculation of its product.
*/
class ProductMergeOperation : public VectorMergeOperation {
public:
	/**
	* @brief The constructor.
	*/
	ProductMergeOperation();
	/**
	* @brief Merges the vector by calculation of its product.
	*
	* @param vec The vector which should be merged.
	*
	* @return The product of the vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/**
* @brief This vector merge operation merges a vector by calculation of its maximal entry.
*/
class MaximalValueMergeOperation : public VectorMergeOperation {
public:
	/**
	* @brief The constructor.
	*/
	MaximalValueMergeOperation();
	/**
	* @brief Merges the vector by calculation of its maximal entry.
	*
	* @param vec The vector which should be merged.
	*
	* @return The maximal entry of the vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/**
* @brief This vector merge operation merges a vector by calculation of its minimal entry.
*/
class MinimalValueMergeOperation : public VectorMergeOperation {
public:
	/**
	* @brief The constructor.
	*/
	MinimalValueMergeOperation();
	/**
	* @brief Merges the vector by calculation of its minimal entry.
	*
	* @param vec The vector which should be merged.
	*
	* @return The minimal entry of the vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/**
* @brief This vector merge operation merges a vector by evaluation of the given function.
*/
class FunctionEvaluationMergeOperation : public VectorMergeOperation {
public:
	/**
	* @brief The constructor, where the merging function can be specified.
	*
	* @param function The function, which should be evaluated to merge a vector.
	*/
	FunctionEvaluationMergeOperation(Function* function);
	/**
	* @brief Merges the vector by evaluation of the specified function.
	*
	* @param vec The vector which should be merged.
	*
	* @return The result of the evaluation of the specified function with input vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();

private:
	Function* function_;
};

/**
* @brief This vector merge operation merges a vector by evaluation of the specified objective function.
*/
class ObjectiveFunctionEvaluationMergeOperation : public VectorMergeOperation {
public:
	/**
	* @brief The constructor.
	*/
	ObjectiveFunctionEvaluationMergeOperation();
	/**
	* @brief Merges the vector by evaluation of the specified objective function in the configuration section.
	*
	* @param vec The vector which should be merged.
	*
	* @return The result of the evaluation of the specified objective function with input vector vec.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & vec);
	std::string GetName();
};

/************** PAIR COMBINATION OPERATIONS ********************/
/**
* @brief This class is an abstract class. It supplies the interface for merging two mpf_t values to a single value.
*/
class PairCombinationOperation {
public:
	/**
	 * @brief The destructor.
	 */
	virtual ~PairCombinationOperation(){}
	/**
	* @brief Merges two values to a single value.
	*
	* @param value1 The first value which should be merged.
	* @param value2 The second value which should be merged.
	*
	* @return The merged value.
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
* @brief This pair combination operation merges two values by addition.
*/
class AddOperation : public PairCombinationOperation {
public:
	/**
	* @brief Merges the two values by adding them.
	*
	* @param value1 The first value which should be merged.
	* @param value2 The second value which should be merged.
	*
	* @return The sum of value1 and value2
	*/
	mpf_t* Evaluate(mpf_t* value1, mpf_t* value2);
	std::string GetName();
};

/**
* @brief This pair combination operation merges two values by subtraction.
*/
class SubtractOperation : public PairCombinationOperation {
public:
	/**
	* @brief Merges the two values by subtraction.
	*
	* @param value1 The first value which should be merged.
	* @param value2 The second value which should be merged.
	*
	* @return value1 - value2
	*/
	mpf_t* Evaluate(mpf_t* value1, mpf_t* value2);
	std::string GetName();
};

/**
* @brief This pair combination operation merges two values by multiplication.
*/
class MultiplyOperation : public PairCombinationOperation {
public:
	/**
	* @brief Merges the two values by multiplication.
	*
	* @param value1 The first value which should be merged.
	* @param value2 The second value which should be merged.
	*
	* @return The product of value1 and value2
	*/
	mpf_t* Evaluate(mpf_t* value1, mpf_t* value2);
	std::string GetName();
};

/**
* @brief This pair combination operation merges two values by division.
*/
class DivideOperation : public PairCombinationOperation {
public:
	/**
	* @brief Merges the two values by division.
	*
	* @param value1 The first value which should be merged.
	* @param value2 The second value which should be merged.
	*
	* @return value1 / value2
	*/
	mpf_t* Evaluate(mpf_t* value1, mpf_t* value2);
	std::string GetName();
};

/**
* @brief This pair combination operation merges two values by calculation of the minimum.
*/
class MinOperation : public PairCombinationOperation {
public:
	/**
	* @brief Merges the two values by calculation of the minimum.
	*
	* @param value1 The first value which should be merged.
	* @param value2 The second value which should be merged.
	*
	* @return min ( value1 , value2 )
	*/
	mpf_t* Evaluate(mpf_t* value1, mpf_t* value2);
	std::string GetName();
};

/**
* @brief This pair combination operation merges two values by calculation of the maximum.
*/
class MaxOperation : public PairCombinationOperation {
public:
	/**
	* @brief Merges the two values by calculation of the maximum.
	*
	* @param value1 The first value which should be merged.
	* @param value2 The second value which should be merged.
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

/************** VECTOR MODIFICATIONS *********************/
/**
* @brief This class is an abstract class. It supplies the interface to modify vectors of mpf_t values.
*/
class VectorModification {
public:
	/**
	 * @brief The destructor.
	 */
	virtual ~VectorModification(){}
	/**
	* @brief Calculates a modification of the supplied vector.
	*
	* @param vec The vector which should be modified.
	*
	* @return The modified vector.
	*/
	virtual std::vector<mpf_t*> Modify(std::vector<mpf_t*> vec) = 0;
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
};

/**
* @brief This vector modification calculates for each dimension the distance to the nearest 1-dimensional local optimum.
*/
class DistTo1DOptimumModification : public VectorModification {
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
	std::vector<mpf_t*> Modify(std::vector<mpf_t*> vec);
	std::string GetName();
};

/************** MODIFICATIONS ****************************/
/**
* @brief This class is an abstract class. It supplies the interface to modify single mpf_t values.
*/
class Modification {
public:
	/**
	 * @brief The destructor.
	 */
	virtual ~Modification(){}
	/**
	* @brief Calculates a modification of the supplied mpf_t value.
	*
	* @param value The value which should be modified.
	*
	* @return The modified value.
	*/
	virtual mpf_t* Modify(mpf_t* value) = 0;
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
};


/**
* @brief This modification calculates the square root of the supplied value.
*/
class SqrtModification : public Modification {
public:
	/**
	* @brief Calculates the square root of the value which should be modified.
	*
	* @param value The value which should be modified.
	*
	* @return The square root of the value which should be modified.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();
};

/**
* @brief This modification calculates the power of the supplied value to a given exponent.
*/
class PowModification : public Modification {
public:
	/**
	* @brief The constructor, where the exponent of the power calculation can be specified.
	*
	* @param exponent The exponent of the power calculation.
	*/
	PowModification(double exponent);

	/**
	* @brief Calculates the power of the value which should be modified to the specified exponent.
	*
	* @param value The value which should be modified.
	*
	* @return The power of the value which should be modified to the specified exponent.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();

private:
	const double exponent_;
};

/**
* @brief This modification calculates the logarithm with base two of the supplied value.
*/
class Log2Modification : public Modification {
public:
	/**
	* @brief Calculates the logarithm with base two of the value which should be modified.
	*
	* @param value The value which should be modified.
	*
	* @return The logarithm with base two of the value which should be modified.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();
};

/**
* @brief This modification calculates logarithm with base two of the supplied value.
* @warning Results of modification may have very bad precision.
*/
class Log2DoubleModification : public Modification {
public:
	/**
	* @brief Calculates the logarithm with base two of the value which should be modified.
	*
	* @warning The calculation uses double calculations. Therefore the result may have a very bad precision.
	* For exact results pleas use the Log2Modification and not Log2DoubleModification!
	*
	* @param value The value which should be modified.
	*
	* @return The logarithm with base two of the value which should be modified.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();
};

/**
* @brief This modification calculates the absolute value of the supplied value.
*/
class AbsModification : public Modification {
public:
	/**
	* @brief Calculates the absolute value of the value which should be modified.
	*
	* @param value The value which should be modified.
	*
	* @return The absolute value of the value which should be modified.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();
};

/**
* @brief This modification calculates the exponential function of the supplied value.
*/
class ExpModification : public Modification {
public:
	/**
	* @brief Calculates the exponential function of the value which should be modified.
	*
	* @param value The value which should be modified.
	*
	* @return The exponential function of the value which should be modified.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();
};

/**
* @brief This modification calculates the sine function of the supplied value.
*/
class SinModification : public Modification {
public:
	/**
	* @brief Calculates the sine function of the value which should be modified.
	*
	* @param value The value which should be modified.
	*
	* @return The sine function of the value which should be modified.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();
};

/**
* @brief This modification calculates the cosine function of the supplied value.
*/
class CosModification : public Modification {
public:
	/**
	* @brief Calculates the cosine function of the value which should be modified.
	*
	* @param value The value which should be modified.
	*
	* @return The cosine function of the value which should be modified.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();
};

/**
* @brief This modification calculates the tangent function of the supplied value.
*/
class TanModification : public Modification {
public:
	/**
	* @brief Calculates the tangent function of the value which should be modified.
	*
	* @param value The value which should be modified.
	*
	* @return The tangent function of the value which should be modified.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();
};

/**
* @brief This modification calculates the arcsine function of the supplied value.
*/
class ArcsinModification : public Modification {
public:
	/**
	* @brief Calculates the arcsine function of the value which should be modified.
	*
	* @param value The value which should be modified.
	*
	* @return The arcsine function of the value which should be modified.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();
};

/**
* @brief This modification calculates the arccosine function of the supplied value.
*/
class ArccosModification : public Modification {
public:
	/**
	* @brief Calculates the arccosine function of the value which should be modified.
	*
	* @param value The value which should be modified.
	*
	* @return The arccosine function of the value which should be modified.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();
};

/**
* @brief This modification calculates the arctangent function of the supplied value.
*/
class ArctanModification : public Modification {
public:
	/**
	* @brief Calculates the arctangent function of the value which should be modified.
	*
	* @param value The value which should be modified.
	*
	* @return The arctangent function of the value which should be modified.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();
};

/**
* @brief This modification calculates the logarithm with base E of the supplied value.
*/
class LogEModification : public Modification {
public:
	/**
	* @brief Calculates the logarithm with base E of the value which should be modified.
	*
	* @param value The value which should be modified.
	*
	* @return The logarithm with base E of the value which should be modified.
	*/
	mpf_t* Modify(mpf_t* value);
	std::string GetName();
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_GENERAL_GENERAL_OBJECTS_H_ */
