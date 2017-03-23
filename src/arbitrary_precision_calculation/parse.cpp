/**
* @file   arbitrary_precision_calculation/parse.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   October, 2015
* @brief  This file contains functions for parsing the configuration file.
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

#include "arbitrary_precision_calculation/parse.h"

#include <iostream>
#include <sstream>

#include "arbitrary_precision_calculation/random_number_generator.h"

namespace arbitraryprecisioncalculation {
namespace parse {

RandomNumberGenerator* ParseRandomNumberGenerator(const std::vector<std::string> & parameters, unsigned int & parsed_parameters) {
	if(parsed_parameters == parameters.size()) {
		return NULL;
	}
	unsigned int mem_parsed_parameters = parsed_parameters;
	std::string parameter = parameters[parsed_parameters++];
	if(parameter == "linearCongruenceRNG") {
		if(parsed_parameters + 2 > parameters.size()) {
			parsed_parameters = mem_parsed_parameters;
			return NULL;
		}
		// parse seed:
		unsigned long long seed;
		{
			std::istringstream is(parameters[parsed_parameters++]);
			if(!(is >> seed)) {
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
		}
		// parse <lcrng>
		std::string subparameter1 = parameters[parsed_parameters++];
		if(subparameter1 == "standard") {
			if(parsed_parameters + 1 > parameters.size()) {
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
			std::string subparameter2 = parameters[parsed_parameters++];
			if(subparameter2 == "fast") {
				return new FastM2P63LinearCongruenceRandomNumberGenerator(1571204578482947281ULL, 12345678901234567ULL, seed);
			} else if(subparameter2 == "intense") {
				if(parsed_parameters + 1 > parameters.size()) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
				int used_bits;
				{
					std::istringstream is(parameters[parsed_parameters++]);
					if(!(is >> used_bits)) {
						parsed_parameters = mem_parsed_parameters;
						return NULL;
					}
				}
				return new IntenseM2P63LinearCongruenceRandomNumberGenerator(1571204578482947281ULL, 12345678901234567ULL, seed, used_bits);
			} else {
				// invalid option;
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
		} else if(subparameter1 == "mod2p63") {
			if(parsed_parameters + 3 > parameters.size()) {
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
			unsigned long long multiplier, adder;
			{
				std::istringstream is(parameters[parsed_parameters++]);
				if(!(is >> multiplier)) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
			}
			{
				std::istringstream is(parameters[parsed_parameters++]);
				if(!(is >> adder)) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
			}
			std::string subparameter2 = parameters[parsed_parameters++];
			if(subparameter2 == "fast") {
				return new FastM2P63LinearCongruenceRandomNumberGenerator(multiplier, adder, seed);
			} else if(subparameter2 == "intense") {
				if(parsed_parameters + 1 > parameters.size()) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
				int used_bits;
				{
					std::istringstream is(parameters[parsed_parameters++]);
					if(!(is >> used_bits)) {
						parsed_parameters = mem_parsed_parameters;
						return NULL;
					}
				}
				return new IntenseM2P63LinearCongruenceRandomNumberGenerator(multiplier, adder, seed, used_bits);
			} else {
				// invalid option;
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
		} else if(subparameter1 == "specific") {
			if(parsed_parameters + 4 > parameters.size()) {
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
			unsigned long long multiplier, adder, modulus;
			{
				std::istringstream is(parameters[parsed_parameters++]);
				if(!(is >> multiplier)) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
			}
			{
				std::istringstream is(parameters[parsed_parameters++]);
				if(!(is >> adder)) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
			}
			{
				std::istringstream is(parameters[parsed_parameters++]);
				if(!(is >> modulus)) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
			}
			std::string subparameter2 = parameters[parsed_parameters++];
			if(subparameter2 == "fast") {
				return new FastLinearCongruenceRandomNumberGenerator(multiplier, adder, modulus, seed);
			} else if(subparameter2 == "intense") {
				return new IntenseLinearCongruenceRandomNumberGenerator(multiplier, adder, modulus, seed);
			} else {
				// invalid option;
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
		} else {
			// invalid option;
			parsed_parameters = mem_parsed_parameters;
			return NULL;
		}
	} else { // try to parse seed
		unsigned long long seed;
		std::istringstream is(parameter);
		if(!(is >> seed)){
			parsed_parameters = mem_parsed_parameters;
			return NULL;
		}
		return new FastM2P63LinearCongruenceRandomNumberGenerator(1571204578482947281ULL, 12345678901234567ULL, seed);
	}
}

void SignalInvalidCommand(const std::vector<std::string> & command){
	std::cerr << ("One of the specified commands in the configuration file can not be interpreted:\n\"");
	for(unsigned int i = 0; i < command.size(); i++){
		if(i != 0)std::cerr << " ";
		std::cerr << command[i];
	}
	std::cerr << ("\"\n");
	exit(1);
}

} // namespace parse
} // namespace arbitraryprecisioncalculation
