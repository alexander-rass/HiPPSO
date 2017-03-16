/**
* @file   direct_statistics.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file contain classes, which evaluate to vectors depending on the current state of the particle swarm.
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

#ifndef HIGH_PRECISION_PSO_STATISTICS_DIRECT_STATISTICS_H_
#define HIGH_PRECISION_PSO_STATISTICS_DIRECT_STATISTICS_H_

#include <gmp.h>
#include <vector>

#include "general/general_objects.h"
#include "statistics/specific_statistical_evaluations.h"

namespace highprecisionpso {

/**
* @brief This class is an abstract class. It supplies the interface for statistical evaluations.
*/
class Statistic {
public:
	/**
	* The destructor.
	*/
	virtual ~Statistic(){}
	/**
	* @brief Calls the normal evaluation function and transforms the result to a string.
	*
	* @return String representation of current evaluation values.
	*/
	std::string EvaluationToString();

	/**
	* @brief Performs some statistical evaluations.
	*
	* @return The result of the statistical evaluations.
	*/
	virtual std::vector<mpf_t*> Evaluate() = 0;
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
};

/**
* @brief This class implements a statistic, which combines two statistics by a combination operation.
*/
class CombineStatistic : public Statistic {
public:
	/**
	* @brief The constructor, where the combination operation and the statistics, which will be combined, can be specified.
	*
	* @param operation The operation, which determines how the statistics will be combined.
	* @param operator1 The first statistic, which will be combined.
	* @param operator2 The second statistic, which will be combined.
	*/
	CombineStatistic(PairCombinationOperation* operation, Statistic* operator1, Statistic* operator2);
	/**
	* @brief Evaluates the two specified statistics and combines them.
	*
	* @return The result of the statistical evaluations.
	*/
	std::vector<mpf_t*> Evaluate();
	std::string GetName();

private:
	PairCombinationOperation* operation_;
	Statistic* operator1_;
	Statistic* operator2_;
};

/**
* @brief This class implements a statistic, which merges a specific statistical evaluation to an analyzable statistic.
*/
class StatisticMergeOperator: public Statistic {
public:
	/**
	* @brief The constructor, where the merge operation and the specific statistical evaluation can be specified.
	*
	* @param merge_operation The merge operation, which specifies how the specific statistical evaluation should be merged.
	* @param specific_evaluation The specific statistical evaluation.
	*/
	StatisticMergeOperator(StatisticMergeOperation* merge_operation, SpecificStatisticalEvaluation* specific_evaluation);
	/**
	* @brief Evaluates the specific statistical evaluation and merges the result to an analyzable statistic.
	*
	* @return The result of the statistical evaluations.
	*/
	std::vector<mpf_t*> Evaluate();
	std::string GetName();

private:
	StatisticMergeOperation* merge_operation_;
	SpecificStatisticalEvaluation* specific_evaluation_;
};

/**
* @brief This class implements a statistic, which modifies the values of another statistic.
*/
class ModifiedStatistic: public Statistic {
public:
	/**
	* @brief The constructor, where the modification and the statistic can be specified.
	*
	* @param modification The modification operation, which will be performed on the statistic.
	* @param statistic The statistic, which will be modified.
	*/
	ModifiedStatistic(Modification* modification, Statistic* statistic);
	/**
	* @brief Evaluates the specified statistic and modifies its values by the specified modification.
	*
	* @return The result of the statistical evaluations.
	*/
	std::vector<mpf_t*> Evaluate();
	std::string GetName();

private:
	Modification* modification_;
	Statistic* statistic_;
};

/**
* @brief This class implements a statistic, where the name is set manually.
*/
class NamedStatistic: public Statistic {
public:
	/**
	* @brief The constructor, where the name and the actual statistic can be specified.
	*
	* @param name The name of this statistic.
	* @param statistic The actual statistic, which will be evaluated.
	*/
	NamedStatistic(std::string name, Statistic* statistic);
	std::vector<mpf_t*> Evaluate();
	std::string GetName();

private:
	std::string name_;
	Statistic* statistic_;
};

/**
* @brief This class implements a statistic, which displays the position entries of the overall best position found so far.
*/
class GlobalBestPositionStatistic : public Statistic{
public:
	/**
	* @brief Returns the best found position.
	*
	* @return The vector with the position entries of the overall best position found so far.
	*/
	std::vector<mpf_t*> Evaluate();
	std::string GetName();
};

/**
* @brief This class implements a statistic, which evaluates the distance of the global best found position to the nearest local optimum if only a single dimension is variable.
*/
class GlobalBestPositionDistTo1DOptimumStatistic : public Statistic{
public:
	/**
	* @brief Returns the distances to the nearest local optima according to the global best found position.
	*
	* @return The vector with the distances to the nearest local optimum if only a single dimension is variable.
	*/
	std::vector<mpf_t*> Evaluate();
	std::string GetName();
};

/**
* @brief This class implements a statistic, which evaluates the objective function at the global best position found so far.
*/
class GlobalBestPositionFunctionEvaluationStatistic : public Statistic {
public:
	/**
	* @brief Returns the best found objective function value.
	*
	* @return The evaluation of the objective function at the best position found so far.
	*/
	std::vector<mpf_t*> Evaluate();
	std::string GetName();
};

/**
* @brief This class implements a statistic, which evaluates how often each particle updates its local attractor.
*/
class LocalAttractorUpdatesStatistic : public Statistic {
public:
	/**
	* @brief Returns for each particle the number of updates of its local attractor.
	*
	* @return The number of local attractor updates for each particle.
	*/
	std::vector<mpf_t*> Evaluate();
	std::string GetName();
};

/**
* @brief This class implements a statistic, which evaluates how often each particle updates a global attractor.
*/
class GlobalAttractorUpdatesStatistic : public Statistic {
public:
	/**
	* @brief Returns for each particle the number of updates of a global attractor enforced by this particle.
	*
	* @return The number of global attractor updates enforced by each particle.
	*/
	std::vector<mpf_t*> Evaluate();
	std::string GetName();
};

/**
* @brief This class implements a statistic, which evaluates how the precision of the evaluations evolve.
*/
class PrecisionStatistic : public Statistic {
public:
	/**
	* @brief Returns the current precision in bits of the mantissa.
	*
	* @return The number of bits in the mantissa of all floating point evaluations.
	*/
	std::vector<mpf_t*> Evaluate();
	std::string GetName();
};

/**
* @brief This class implements a statistic, which always evaluates to the same value.
*
* This statistic can be combined with other statistics to evaluate for example the distance to some constant value.
*/
class ConstantStatistic : public Statistic {
public:
	/**
	* @brief The constructor, where the number of dimensions for this statistic and the generation of the constant value can be specified.
	*
	* @param dimensions The number of dimensions for this statistic.
	* @param constant_evaluation The generation method for the constant value
	*/
	ConstantStatistic(int dimensions, ConstantEvaluation* constant_evaluation);
	/**
	* @brief Returns the vector with specified number of entries and specified content.
	*
	* @return The constant vector.
	*/
	std::vector<mpf_t*> Evaluate();
	std::string GetName();

private:
	int dimensions_;
	ConstantEvaluation* constant_evaluation_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_STATISTICS_DIRECT_STATISTICS_H_ */
