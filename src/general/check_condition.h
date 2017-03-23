/**
* @file   general/check_condition.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file includes a possibility to check conditions which should be true. (similar to assertions).
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

#ifndef HIGH_PRECISION_PSO_GENERAL_CHECK_CONDITION_H_
#define HIGH_PRECISION_PSO_GENERAL_CHECK_CONDITION_H_

#include <string>

namespace highprecisionpso {

/**
* @brief If EX is evaluated to false then it prints the given information to stderr and let the program terminate with exit code 1.
*
* @param EX The expression which should be checked.
* @param MSG A special message which will be displayed if EX is not true.
*/
#define AssertCondition(EX, MSG) (void)((EX) || (__AssertCondition (#EX, __FILE__, __func__, __LINE__, MSG),0))

/**
* @brief Prints the given information to stderr and let the program terminate with exit code 1.
*
* @param argument The argument of the checked condition.
* @param file The file where the error occurs.
* @param function_name The function name where the error occurs.
* @param line The line where the error occurs
* @param message A further information message, which gives some additional information why the error occurs.
*/
void __AssertCondition(const char *argument, const char * file, const char * function_name, int line, std::string message);

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_GENERAL_CHECK_CONDITION_H_ */
