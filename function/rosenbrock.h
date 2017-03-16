/**
* @file   rosenbrock.h 
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2015
* @brief  This file contains the description of the rosenbrock function and a moved version.
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_ROSENBROCK_H_
#define HIGH_PRECISION_PSO_FUNCTION_ROSENBROCK_H_

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This class represents the well known test function rosenbrock.
*/
class Rosenbrock: public Function {
public:
	/**
	* @brief The constructor.
	*/
	Rosenbrock();
	/**
	* @brief Returns the evaluation of the rosenbrock function evaluated at the given position.
	*
	* The standard bounds of the search space are [-30, 30] in each dimension.
	*
	* @param pos The position vector.
	*
	* @return \f$10\cdot D+\sum_{d=0}^{D-2}(100(pos[d+1] - pos[d]^2)^2+(1-pos[d])^2)\f$.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
};

/**
* @brief This class represents a moved version of the well known test function rosenbrock, where the optimal value is moved to the origin.
*/
class MovedRosenbrock: public Function {
public:
	/**
	* @brief The constructor.
	*/
	MovedRosenbrock();
	/**
	* @brief Returns the evaluation of the moved rosenbrock function evaluated at the given position.
	*
	* To all entries of the position vector 1 is added.
	* With this moved position the real rosenbrock function is evaluated.
	* The standard bounds of the search space are [-31, 29] in each dimension.
	*
	* @param pos The position vector.
	*
	* @return The evaluation of the moved rosenbrock function.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_ROSENBROCK_H_ */
