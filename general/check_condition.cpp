/**
* @file   check_condition.cpp
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

#include <iostream>

#include "check_condition.h"

namespace highprecisionpso {

void __AssertCondition(const char * argument, const char * file, const char * function_name,  int line, std::string message){
	std::cerr << "Unsatisfied condition \"" << std::string(argument) << "\" in file \"" << std::string(file) << "\" in function \"" 
			<< std::string(function_name) << "\" at line " << line << ".\n";
	if(message.size() > 0)std::cerr << "Message: " << message << std::endl;
	std::cerr << "If you are sure that you did not accidentally cause the error yourself "
			<< "(reason may be explained in the displayed message), "
			<< "please contact the main program author Alexander Raß per mail at \"Alexander.Rass@fau.de\". "
			<< "Please supply complete error message and available files which were used for or created by "
			<< "the current program execution (like configuration file, log file, backup file, etc.). "
			<< "Please supply also runtime circumstances (operating system, parallel program executions, etc.).\n";
	exit (1);
}

} // namespace highprecisionpso
