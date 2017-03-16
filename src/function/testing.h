/**
* @file   testing.h 
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2015
* @brief  This file contains the description of an experimental testing function.
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_TESTING_H_
#define HIGH_PRECISION_PSO_FUNCTION_TESTING_H_

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This class is generated for testing purpose.
*
* The specific evaluation formula will not be specified.
* Evaluations may change during the optimization process and the evaluations might differ after software updates.
*
* @warning Usage of this class is not recommended!
*
*/
class TestingFunction: public Function {
public:
	/**
	* @brief The constructor.
	*
	* @warning Usage of this class is not recommended!
	*
	*/
	TestingFunction();
	/**
	* @brief Calculates some function value.
	*
	* The specific evaluation formula will not be specified.
	* Evaluations may change during the optimization process and the evaluations might differ after software updates.
	*
	* @warning Usage of this function is not recommended!
	*
	* @param pos The position.
	*
	* @return Some value.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_TESTING_H_ */
