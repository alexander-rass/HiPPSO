/**
* @file   single_different_direction.h 
* @author Alexander Raß (alexander.rass@fau.de)
* @date   August, 2015
* @brief  This file contains the description of function which performs different in a single direction.
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_SINGLE_DIFFERENT_DIRECTION_H_
#define HIGH_PRECISION_PSO_FUNCTION_SINGLE_DIFFERENT_DIRECTION_H_

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This enumeration determines the specific behavior of the single different direction function.
*/
enum SingleDifferentDirectionMode { 
	/**
	* @brief In this mode the special direction is the direction of the coordinate axis belonging to the first dimension.
	* The special direction vector is then (1,0,...,0)
	*/
	SINGLE_DIFFERENT_DIRECTION_MODE_FIRST,
	/**
	* @brief In this mode the special direction is diagonal according to all dimensions. All entries of the direction vector are 1.
	* The special direction vector is then (1,1,...,1)
	*/
	SINGLE_DIFFERENT_DIRECTION_MODE_DIAGONAL, 
	/**
	* @brief In this mode the special direction is randomly chosen according to the random number generator for this function.
	* The special direction vector is then (r<sub>0</sub>,r<sub>1</sub>,...,r<sub>D-1</sub>), where each r<sub>i</sub> is randomly distributed similar to a gaussian distribution with expectation 0 and variance 1.
	*/
	SINGLE_DIFFERENT_DIRECTION_MODE_RANDOM };

/**
* @brief This class implements a function with a special direction.
*/
class SingleDifferentDirection: public Function {
public:
	/**
	* @brief The constructor.
	*
	* @param single_dimension_exponent The exponent for the length in the special direction.
	* @param remaining_dimension_exponent The exponent for the length in the remaining direction.
	* @param mode The specification, which direction performs special.
	* @param rng_description The description of the random number generator. If the mode is not random then this vector can be empty.
	*/
	SingleDifferentDirection(double single_dimension_exponent = 2.0,
			double remaining_dimension_exponent = 2.0,
			SingleDifferentDirectionMode mode = SINGLE_DIFFERENT_DIRECTION_MODE_FIRST,
			std::vector<std::string> rng_description = std::vector<std::string>(0));
	/**
	* @brief The destructor.
	*/
	~SingleDifferentDirection();
	/**
	* @brief Returns the evaluation of the SingleDifferentDirection at the given position.
	*
	* Let sd be the special direction vector.
	* See the enumeration about the single different direction mode for details on this vector.
	* Let *orth* be the orthogonal projection of the position vector to the special direction vector:<br>
    * orth:=( ( pos . sd ) / ( sd . sd ) ) &middot; sd.<br>
	* Let *rem* be the remaining part:<br>
    * rem:=pos-orth.<br>
	* Then the result is<br>
    * ( orth . orth )<sup>single_dimension_exponent</sup> + ( rem . rem )<sup>remaining_dimension_exponent</sup>.<br>
	* Here ( a . b ) represents the dot product (scalar product) of the two vectors a and b.
	*
	* @param pos The position.
	*
	* @return The result of the evaluation of the single different direction function.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();

private:
	void InitSpecialDirection(const std::vector<mpf_t*> & vec);
	const double single_dimension_exponent_;
	const double remaining_dimension_exponent_;
	const SingleDifferentDirectionMode direction_mode_;
	const std::vector<std::string> rng_description_;
	std::vector<mpf_t*> special_direction_cached_;
	unsigned int special_direction_cached_precision_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_SINGLE_DIFFERENT_DIRECTION_H_ */
