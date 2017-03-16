/**
* @file   specific_statistical_evaluations.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file contain classes, which can evaluate matrices depending on the current state of the particle swarm, and some helping classes to generate analyzable statistics out of them.
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

#ifndef HIGH_PRECISION_PSO_STATISTICS_SPECIFIC_STATISTICAL_EVALUATIONS_H_
#define HIGH_PRECISION_PSO_STATISTICS_SPECIFIC_STATISTICAL_EVALUATIONS_H_

#include <gmp.h>
#include <string>
#include <vector>

#include "general/general_objects.h"

namespace highprecisionpso {

/**
* @brief This class is an abstract class. It supplies the interface for intermediate statistical evaluations.
*/
class SpecificStatisticalEvaluation {
public:
	/**
	* The destructor.
	*/
	virtual ~SpecificStatisticalEvaluation(){}
	/**
	* @brief Performs some statistical evaluations.
	*
	* @return The result of the statistical evaluations.
	*/
	virtual std::vector<std::vector<mpf_t*> > Evaluate() = 0;
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
};

/**
* @brief This class implements a specific statistical evaluation, which modifies the values of another specific statistical evaluation.
*/
class ModifiedSpecificStatisticalEvaluation : public SpecificStatisticalEvaluation {
public:
	/**
	* @brief The constructor, where the modification and the specific statistical evaluation can be specified.
	*
	* @param modification The modification operation, which will be performed on the specific statistical evaluation.
	* @param specific_evaluation The specific statistical evaluation, which will be modified.
	*/
	ModifiedSpecificStatisticalEvaluation(Modification* modification, SpecificStatisticalEvaluation* specific_evaluation);
	/**
	* @brief Evaluates the specified specific statistical evaluation and modifies its values by the specified modification.
	*
	* @return The result of the statistical evaluations.
	*/
	std::vector<std::vector<mpf_t*> > Evaluate();
	std::string GetName();

private:
	Modification* modification_;
	SpecificStatisticalEvaluation* specific_evaluation_;
};


/**
* @brief This class implements a specific statistical evaluation, which combines two specific statistical evaluations by a combination operation.
*/
class CombineSpecificStatisticalEvaluation : public SpecificStatisticalEvaluation {
public:
	/**
	* @brief The constructor, where the combination operation and the specific statistical evaluations, which will be combined, can be specified.
	*
	* @param operation The operation, which determines how the specific statistical evaluations will be combined.
	* @param operator1 The first specific statistical evaluation, which will be combined.
	* @param operator2 The second specific statistical evaluation, which will be combined.
	*/
	CombineSpecificStatisticalEvaluation(PairCombinationOperation* operation, SpecificStatisticalEvaluation* operator1, SpecificStatisticalEvaluation* operator2);
	/**
	* @brief Evaluates the two specified specific statistical evaluations and combines them.
	*
	* @return The result of the statistical evaluations.
	*/
	std::vector<std::vector<mpf_t*> > Evaluate();
	std::string GetName();

private:
	PairCombinationOperation* operation_;
	SpecificStatisticalEvaluation* operator1_;
	SpecificStatisticalEvaluation* operator2_;
};

/**
* @brief This class implements a specific statistical evaluation, which supplies the positions of the particles.
*/
class PositionEvaluation : public SpecificStatisticalEvaluation {
public:
	/**
	* @brief Returns the current positions of the particles.
	*
	* @return The positions of the particles.
	*/
	std::vector<std::vector<mpf_t*> > Evaluate();
	std::string GetName();
};

/**
* @brief This class implements a specific statistical evaluation, which supplies the velocities of the particles.
*/
class VelocityEvaluation : public SpecificStatisticalEvaluation {
public:
	/**
	* @brief Returns the current velocities of the particles.
	*
	* @return The velocities of the particles.
	*/
	std::vector<std::vector<mpf_t*> > Evaluate();
	std::string GetName();
};

/**
* @brief This class implements a specific statistical evaluation, which supplies the local attractor positions of the particles.
*/
class LocalAttractorEvaluation : public SpecificStatisticalEvaluation {
public:
	/**
	* @brief Returns the current local attractor positions of the particles.
	*
	* @return The local attractor positions of the particles.
	*/
	std::vector<std::vector<mpf_t*> > Evaluate();
	std::string GetName();
};

/**
* @brief This class implements a specific statistical evaluation, which supplies the global attractor positions of the particles.
*/
class GlobalAttractorEvaluation : public SpecificStatisticalEvaluation {
public:
	/**
	* @brief Returns the current global attractor positions of the particles.
	*
	* @return The global attractor positions of the particles.
	*/
	std::vector<std::vector<mpf_t*> > Evaluate();
	std::string GetName();
};

/**
* @brief This class implements a specific statistical evaluation, which always evaluates to the same value.
*
* This statistic can be combined with other  statistics to evaluate for example the distance to some constant value.
*/
class ConstantSpecificStatisticalEvaluation : public SpecificStatisticalEvaluation {
public:
	/**
	* @brief The constructor, where the generation of the constant value can be specified.
	*
	* @param constant_evaluation The generation method for the constant value.
	*/
	ConstantSpecificStatisticalEvaluation(ConstantEvaluation* constant_evaluation);
	/**
	* @brief Returns the matrix with constant content as specified.
	*
	* The result contains as many vectors as the number of particles.
	* Each of these vectors contains as many values as the number of dimensions of the search space.
	*
	* @return The constant matrix.
	*/
	std::vector<std::vector<mpf_t*> > Evaluate();
	std::string GetName();

private:
	ConstantEvaluation* constant_evaluation_;
};

/************************ statistical merge operations *****************************/

/**
* @brief This class is an abstract class. It supplies the interface to merge specific statistical evaluations to analyzable statistics.
*/
class StatisticMergeOperation {
public:
	/**
	* The destructor.
	*/
	virtual ~StatisticMergeOperation(){}
	/**
	* @brief Merges the entries of the given data matrix to a vector.
	*
	* @param data The data matrix.
	*
	* @return The merged vector.
	*/
	virtual std::vector<mpf_t*> Evaluate(const std::vector<std::vector<mpf_t*> > & data) = 0;
	/**
	* @brief Returns a string representation of this object.
	*
	* @return  The name of this object.
	*/
	virtual std::string GetName() = 0;
};

/**
* @brief This class is an abstract class.
* Additionally to the statistic merge operation it stores a vector merge operation,
* which determines how specific statistical evaluations should be merged. 
*/
class ComposedStatisticMergeOperation : public StatisticMergeOperation {
public:
	/**
	* @brief The constructor, where the merge operation can be specified.
	*
	* @param vector_merge_operation The merge operation.
	*/
	ComposedStatisticMergeOperation(VectorMergeOperation* vector_merge_operation);
	std::vector<mpf_t*> Evaluate(const std::vector<std::vector<mpf_t*> > & vec) = 0;
	std::string GetName() = 0;
protected:
	/**
	* @brief The stored vector merge operation.
	*/
	VectorMergeOperation*  vector_merge_operation;
};

/**
* @brief This class implements a merge operation, which merges the values of all particles to a single value.
*/
class ComposedParticleMergeOperation : public ComposedStatisticMergeOperation {
public:
	/**
	* @brief The constructor, where the merge operation can be specified.
	*
	* @param vector_merge_operation The merge operation.
	*/
	ComposedParticleMergeOperation(VectorMergeOperation* vector_merge_operation);
	/**
	* @brief Merges the entries of the given data matrix to a vector.
	*
	* The data matrix contains a vector for each particle and each of those
	* vectors has an entry for each dimension of the search space.
	* This merge operation merges the particles together and therefore produces
	* a value for each dimension of the search space.
	* For example if the vector merge operation calculates an average and the data
	* matrix contains the positions of the particles, then an average position is calculated.
	*
	* @param data The data matrix.
	*
	* @return The merged vector.
	*/
	std::vector<mpf_t*> Evaluate(const std::vector<std::vector<mpf_t*> > & data);
	std::string GetName();
};

/**
* @brief This class implements a merge operation, which merges the values of all dimensions to a single value.
*/
class ComposedDimensionMergeOperation : public ComposedStatisticMergeOperation {
public:
	/**
	* @brief The constructor, where the merge operation can be specified.
	*
	* @param vector_merge_operation The merge operation.
	*/
	ComposedDimensionMergeOperation(VectorMergeOperation* vector_merge_operation);
	/**
	* @brief Merges the entries of the given data matrix to a vector.
	*
	* The data matrix contains a vector for each particle and each of those
	* vectors has an entry for each dimension of the search space.
	* This merge operation merges the dimensions of the search space together
	* and therefore produces a value for each particle.
	* For example if the vector merge operation calculates the sum and the data
	* matrix contains the squared positions of the particles,
	* then for each particle the squared euclidean distance to the origin is calculated.
	*
	* @param data The data matrix.
	*
	* @return The merged vector.
	*/
	std::vector<mpf_t*> Evaluate(const std::vector<std::vector<mpf_t*> > & data);
	std::string GetName();
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_STATISTICS_SPECIFIC_STATISTICAL_EVALUATIONS_H_ */
