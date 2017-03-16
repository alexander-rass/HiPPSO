/**
* @file   diagonal_function.h 
* @author Alexander Raß (alexander.rass@fau.de)
* @date   January, 2014
* @brief  This file contains the description of a function with heavy diagonal eigenvector in the second derivative matrix.
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_DIAGONAL_FUNCTION_H_
#define HIGH_PRECISION_PSO_FUNCTION_DIAGONAL_FUNCTION_H_

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This class implements the diagonal function, which represents a function with optimum at the origin and constant second derivative matrix.
*
* The constant second derivative matrix is the matrix for the sphere function plus a some diagonal part.
*/
class DiagonalFunction: public Function {
public:
	/**
	* @brief The constructor, which specifies the weight of the diagonal part of the function.
	*
	* @param scale The weight of the diagonal part.
	*/
	DiagonalFunction(double scale);
	/**
	* @brief Evaluates the function at the specified position.
	*
	* @param pos The position.
	*
	* @return (sum (pos[i] * pos[i])) + scale * (sum (pos[i])) * (sum (pos[i]))
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);

private:
	const double scale_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_DIAGONAL_FUNCTION_H_ */
