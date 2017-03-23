/**
* @file   function/rastrigin.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2015
* @brief  This file contains the description of the rastrigin function.
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_RASTRIGIN_H_
#define HIGH_PRECISION_PSO_FUNCTION_RASTRIGIN_H_

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This class represents the well known test function rastrigin.
*/
class Rastrigin: public Function {
public:
	/**
	* @brief The constructor.
	*/
	Rastrigin();
	/**
	* @brief Returns the evaluation of the rastrigin function evaluated at the given position.
	*
	* The standard bounds of the search space are [-5.12,5.12] in each dimension.
	*
	* @param pos The position vector.
	*
	* @return 10 &middot; D + &sum;<sub>d=1..D-1</sub>( pos[d]<sup>2</sup> - 10 cos( 2&pi; &middot; pos[d] ) ).
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_RASTRIGIN_H_ */
