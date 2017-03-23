/**
* @file   function/two_cups_function.h
* @author Manuel Schmitt
* @date   July, 2014
* @brief  This file contains the description of the two cups function.
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_TWO_CUPS_FUNCTION_H_
#define HIGH_PRECISION_PSO_FUNCTION_TWO_CUPS_FUNCTION_H_

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This class represents a test function with 2<sup>D</sup> many optimal positions.
*/
class TwoCupsFunction: public Function {
public:
	/**
	* @brief Calculates the sum over a single dimensional function evaluated on each position entry.
	*
	* The single dimensional function performs in two different ways.
	* If the position entry is not positive then the distance to -1 is calculated and the square of this distance is returned.
	* If the position entry is positive then the distance to 1 is calculated and the fourth power of this distance is returned.
	*
	* @param pos The position.
	*
	* @return &sum;<sub>d=0..D-1</sub>tc( pos[d] ), where tc( x )= ( x + 1 )<sup>2</sup> if x is not positive and tc( x )= ( x -1 )<sup>4</sup> if x is positive.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	/**
	* @brief Calculates for a position and a dimension the next local optimum if only the value of the position in the specified dimension is varied and returns the distance to it.
	*
	* @param pos The position.
	* @param d The index of the observed dimension.
	*
	* @return min( abs( pos[d]-1 ), abs( pos[d]+1 ) ).
	*/
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_TWO_CUPS_FUNCTION_H_ */
