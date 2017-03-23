/**
* @file   function/inclined_plane.h
* @author Manuel Schmitt
* @date   July, 2014
* @brief  This file contains the description of the inclined plane functions.
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_INCLINED_PLANE_H_
#define HIGH_PRECISION_PSO_FUNCTION_INCLINED_PLANE_H_

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This class implements the inclined plane function, which represents a function with constant first derivative.
*/
class InclinedPlane: public Function {
public:
	/**
	* @brief The constructor.
	*/
	InclinedPlane();
	/**
	* @brief Returns the sum of the vector entries multiplied by -1.
	*
	* @param pos The position.
	*
	* @return The negative vector sum.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	/**
	* @brief Returns always +infinity, because the optimum can not be reached.
	*
	* @param pos The position.
	* @param d The dimension index.
	*
	* @return +infinity.
	*/
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);
};

/**
* @brief This class implements the asymmetric inclined plane function, which represents a function with constant first derivative, but different constant entries for each dimension.
*/
class InclinedPlaneAsym: public Function {
public:
	/**
	* @brief The constructor.
	*/
	InclinedPlaneAsym();
	/**
	* @brief Returns a weighted sum of the vector entries.
	*
	* Calculates the following sum: -&sum;<sub>d=0..D-1</sub> ( d + 1 ) &middot; pos[d].
	*
	* @param pos The position.
	*
	* @return The weighted sum.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	/**
	* @brief Returns always +infinity, because the optimum can not be reached.
	*
	* @param pos The position.
	* @param d The dimension index.
	*
	* @return +infinity.
	*/
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_INCLINED_PLANE_H_ */
