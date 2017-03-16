/**
* @file   potential_calculator.h 
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2014
* @brief  This file contains classes for statistical calculations.
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

#ifndef HIGH_PRECISION_PSO_STATISTICS_POTENTIAL_CALCULATOR_H_
#define HIGH_PRECISION_PSO_STATISTICS_POTENTIAL_CALCULATOR_H_

#include <gmp.h>
#include <string>
#include <vector>

#include "general/particle.h"
#include "statistics/specific_statistical_evaluations.h"

namespace highprecisionpso {

/**
* @brief This class is an abstract class. It supplies the interface for calculation of some statistical data.
*/
class PotentialCalculator : public SpecificStatisticalEvaluation {
public:
	/**
	* @brief Calculates for a specific particle and a specific dimension a statistical value.
	*
	* @param p The particle.
	* @param dim The dimension index.
	*
	* @return The statistical value.
	*/
	virtual mpf_t* Evaluate(Particle* p, int dim) = 0;
	/**
	* @brief Calculates for all particles and dimensions the statistical values.
	*
	* @return A matrix containing statistical data.
	*/
	std::vector<std::vector<mpf_t*> > Evaluate();
};

/**
* @brief This potential calculator can calculate the function difference statistic.
*/
class PotentialFunctionDifference : public PotentialCalculator {
public:
	/**
	* @brief Calculates for a specific particle and a specific dimension the function difference statistic.
	*
	* Let x be the position of the particle.<br>
	* Let v be the velocity of the particle.<br>
	* Let f be the objective function.<br>
	* Let x' be a position such that x'[dim] = x[dim] + v[dim] and x'[d] = x[d] for all dimensions not equal to dim.<br>
	* Then the result of the function difference statistic is abs( f ( x ) - f ( x' ) ).
	*
	* @param p The particle.
	* @param dim The dimension index.
	*
	* @return The statistical value.
	*/
	mpf_t* Evaluate(Particle* p, int dim);
	std::string GetName();
};

/**
* @brief This potential calculator can calculate the scaled absolute value of the velocity plus the distance to the global attractor.
*/
class PotentialAbsVelocityPlusDistToGlobalAttractor : public PotentialCalculator {
public:
	/**
	* @brief The constructor, which specifies the scale for the absolute velocity.
	*
	* @param scale The scale.
	*/
	PotentialAbsVelocityPlusDistToGlobalAttractor(double scale);
	/**
	* @brief Calculates the scaled absolute value of the velocity plus the distance to the global attractor.
	*
	* Let x be the position of the particle,<br>
	* let v be the velocity of the particle,<br>
	* let g be the global attractor of the particle<br>
	* and let s be the scale of this potential calculator.<br>
	* Then the result of this potential calculation is (s &middot; abs( v[dim] ) + abs( x[dim] - g[dim] )).
	*
	* @param p The particle.
	* @param dim The dimension index.
	*
	* @return The statistical value.
	*/
	mpf_t* Evaluate(Particle* p, int dim);
	std::string GetName();

private:
	const double scale_;
};

/**
* @brief This potential calculator can calculate the scaled square root of the absolute velocity plus the square root of the distance to the global attractor.
*/
class PotentialSqrtAbsVelocityPlusSqrtDistToGlobalAttractor : public PotentialCalculator {
public:
	/**
	* @brief The constructor, which specifies the scale for the absolute velocity.
	*
	* @param scale The scale.
	*/
	PotentialSqrtAbsVelocityPlusSqrtDistToGlobalAttractor(double scale);
	/**
	* @brief Calculates the scaled square root of the absolute value of the velocity plus the square root of the distance to the global attractor.
	*
	* Let x be the position of the particle,<br>
	* let v be the velocity of the particle,<br>
	* let g be the global attractor of the particle<br>
	* and let s be the scale of this PotentialCalculator.<br>
	* Then the result of this potential calculation is (s &middot; sqrt( abs( v[dim] ) ) + sqrt( abs( x[dim] - g[dim] ) ).
	*
	* @param p The particle.
	* @param dim The dimension index.
	*
	* @return The statistical value.
	*/
	mpf_t* Evaluate(Particle* p, int dim);
	std::string GetName();

private:
	const double scale_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_STATISTICS_POTENTIAL_CALCULATOR_H_ */
