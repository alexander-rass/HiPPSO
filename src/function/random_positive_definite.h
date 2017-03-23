/**
* @file   function/random_positive_definite.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains the description of a function with random positive definite second derivative matrix.
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_RANDOM_POSITIVE_DEFINITE_H_
#define HIGH_PRECISION_PSO_FUNCTION_RANDOM_POSITIVE_DEFINITE_H_

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This class represents a function with constant random positive (semi) definite derivative.
*/
class RandomPositiveDefiniteSecondDerivative: public Function {
public:
	/**
	* @brief The constructor, where the description of the random number generator can be specified.
	*
	* @param rng_description The description of the random number generator.
	*/
	RandomPositiveDefiniteSecondDerivative(std::vector<std::string> rng_description);
	/**
	* @brief The destructor.
	*/
	~RandomPositiveDefiniteSecondDerivative();
	/**
	* @brief Returns the product transpose(pos) * A * pos, where A is a random matrix.
	*
	* The random matrix A is generated according to the specified random number generator.
	* Let D be the number of dimensions.
	* Then a D times D matrix of random long long values is generated.
	* This matrix is multiplied by its transposed matrix to generate the positive (semi) definite matrix A.
	* If the random number generator is not bad then the probability,
	* that the matrix A is positive definite and not only positive semi definite, is very large.
	*
	* @param pos The position vector.
	*
	* @return Transpose(pos) * A * pos.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	/**
	* @brief Calculates for a position and a dimension the next local optimum if only the value of the position in the specified dimension is varied and returns the distance to it.
	*
	* For a given position all values (beside the value in the specified dimension) are regarded as constants.
	* By the evaluation of a quadratic equation,
	* where the value of the position in the specified dimension is varied,
	* a local optimum is calculated.
	* The result is the distance of the evaluated position and the found local optimum.
	*
	* @param pos The position.
	* @param d The index of the observed dimension.
	*
	* @return The distance to the next local optimum in a single dimension setting.
	*/
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);

private:
	void InitMatrixCached(const std::vector<mpf_t*> & vec);
	const std::vector<std::string> rng_description_;
	std::vector<std::vector<mpf_t*> > matrix_cached_;
	unsigned int matrix_cached_precision_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_RANDOM_POSITIVE_DEFINITE_H_ */
