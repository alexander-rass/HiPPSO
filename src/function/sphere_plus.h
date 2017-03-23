/**
* @file   function/sphere_plus.h
* @author Manuel Schmitt
* @date   July, 2014
* @brief  This file contains the description of the sphere plus function.
*
* @copyright
* This project is released under the MIT License (MIT).
*
* @copyright
* The MIT License (MIT)
*
* @copyright
* Copyright (c) 2016 by Friedrich-Alexander-Universität Erlangen-Nürnberg and
* Alexander Raß and Manuel Schmitt
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_SPHERE_PLUS_H_
#define HIGH_PRECISION_PSO_FUNCTION_SPHERE_PLUS_H_

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This class represents the well known test function sphere plus.
*/
class SpherePlus: public Function {
public:
	/**
	* @brief Performs like the sphere function if all position entries are positive and is plus infinity otherwise.
	*
	* @param pos The position.
	*
	* @return &sum;<sub>d=0..D-1</sub>pos[d]<sup>2</sup> if all position entries are positive and plus infinity otherwise.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	/**
	* @brief Calculates for a position and a dimension the next local optimum if only the value of the position in the specified dimension is varied and returns the distance to it.
	*
	* @param pos The position.
	* @param d The index of the observed dimension.
	*
	* @return The absolute value of the position entry in dimension d.
	*/
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_SPHERE_PLUS_H_ */
