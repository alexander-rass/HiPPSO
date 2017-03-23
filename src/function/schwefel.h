/**
* @file   function/schwefel.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   January, 2014
* @brief  This file contains two different versions of the schwefel test function.
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_SCHWEFEL_H_
#define HIGH_PRECISION_PSO_FUNCTION_SCHWEFEL_H_

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This class represents the well known test function schwefel.
*
* There are two different common test functions with the name schwefel function.
* These two versions are implemented in the classes Schwefel1 and Schwefel2.
*/
class Schwefel1: public Function {
public:
	/**
	* @brief Returns the evaluation of the first version of a schwefel function evaluated at the given position.
	*
	* @param pos The position.
	*
	* @return &sum;<sub>d=0..D-1</sub>(&sum;<sub>i=0..d</sub> pos[i])<sup>2</sup>.
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
};

/**
* @brief This class represents the well known test function schwefel.
*
* There are two different common test functions with the name schwefel function.
* These two versions are implemented in the classes Schwefel1 and Schwefel2.
*/
class Schwefel2: public Function {
public:
	/**
	* @brief The constructor.
	*/
	Schwefel2();
	/**
	* @brief Returns the evaluation of the second version of a schwefel function evaluated at the given position.
	*
	* The standard bounds of the search space are [-500,500] in each dimension.
	*
	* @param pos The position.
	*
	* @return -&sum;<sub>d=0..D-1</sub>( pos[d] &middot; sin( sqrt( pos[d] ) ) ).
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_SCHWEFEL_H_ */
