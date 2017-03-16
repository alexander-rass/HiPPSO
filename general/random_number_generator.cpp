/**
* @file   random_number_generator.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains the available random number generators.
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

#include <general/random_number_generator.h>
#include <cmath>
#include <iostream>
#include <sstream>

#include "general/check_condition.h"
#include "general/operations.h"

namespace highprecisionpso {

RandomNumberGenerator::~RandomNumberGenerator(){}

FastM2P63LinearCongruenceRandomNumberGenerator::FastM2P63LinearCongruenceRandomNumberGenerator(
		unsigned long long multiplier,
		unsigned long long adder,
		unsigned long long seed) :
				multiplier_(multiplier), adder_(adder), initial_seed_(seed), seed_(seed) {}


mpf_t* FastM2P63LinearCongruenceRandomNumberGenerator::RandomMpft() {
	mpf_t* rand_long_long = mpftoperations::ToMpft(RandomLongLong());
	mpf_t* res = mpftoperations::Multiply2Exp(rand_long_long, -63);
	mpftoperations::ReleaseValue(rand_long_long);
	return res;
}

long long FastM2P63LinearCongruenceRandomNumberGenerator::RandomLongLong(){
	// Overflows are fine because the result is needed modulo 2^63
	seed_ = (multiplier_ * seed_ + adder_);
	seed_ ^= seed_ & (1ULL<<63);
	return (long long) seed_;
}

void FastM2P63LinearCongruenceRandomNumberGenerator::LoadData(std::ifstream* inputstream, ProgramVersion* version_of_stored_data){
	(*inputstream) >> seed_;
}
void FastM2P63LinearCongruenceRandomNumberGenerator::StoreData(std::ofstream* outputstream){
	(*outputstream) << seed_ << std::endl;
}

std::string FastM2P63LinearCongruenceRandomNumberGenerator::GetName(){
	std::ostringstream os;
	os << "FLCRNG_2P63_Seed";
	int minimalseedfigures = 4;
	unsigned long long mul = 10;
	for(int i = 1; i < minimalseedfigures; i++){
		if(initial_seed_ < mul)os << "0";
		mul *= 10;
	}
	os << initial_seed_;
	return os.str();
}


IntenseM2P63LinearCongruenceRandomNumberGenerator::IntenseM2P63LinearCongruenceRandomNumberGenerator(
		unsigned long long multiplier,
		unsigned long long adder,
		unsigned long long seed,
		int accept_bits) :
				multiplier_(multiplier), adder_(adder), initial_seed_(seed), seed_(seed), accept_bits_(accept_bits){}

mpf_t* IntenseM2P63LinearCongruenceRandomNumberGenerator::RandomMpft() {
	if(accept_bits_ <= 0) {
		std::cerr << "The number of accepted bits for the random number generator is reseted to 1 because " << accept_bits_ << " is invalid.\n";
		accept_bits_ = 1;
	} else if(accept_bits_ > 63) {
		std::cerr << "The number of accepted bits for the random number generator is reseted to 63 because " << accept_bits_ << " is invalid.\n";
		accept_bits_ = 63;
	}
	mpf_t* res = mpftoperations::ToMpft(0.0);
	for(int remaining_bits = mpf_get_default_prec(); remaining_bits > 0; remaining_bits -= accept_bits_){
		long long next_rand = RandomLongLong();
		next_rand = (next_rand >> (63 - accept_bits_));
		mpf_t* next_part = mpftoperations::ToMpft(next_rand);
		mpf_t* tmp_res = mpftoperations::Add(next_part, res);
		mpftoperations::ReleaseValue(next_part);
		mpftoperations::ReleaseValue(res);
		res = mpftoperations::Multiply2Exp(tmp_res, -accept_bits_);
		mpftoperations::ReleaseValue(tmp_res);
	}
	return res;
}

long long IntenseM2P63LinearCongruenceRandomNumberGenerator::RandomLongLong(){
	// Overflows are fine because the result is needed modulo 2^63
	seed_ = (multiplier_ * seed_ + adder_);
	seed_ ^= seed_ & (1ULL<<63);
	return (long long) seed_;
}

void IntenseM2P63LinearCongruenceRandomNumberGenerator::LoadData(std::ifstream* inputstream, ProgramVersion* version_of_stored_data){
	(*inputstream) >> seed_;
}
void IntenseM2P63LinearCongruenceRandomNumberGenerator::StoreData(std::ofstream* outputstream){
	(*outputstream) << seed_ << std::endl;
}

std::string IntenseM2P63LinearCongruenceRandomNumberGenerator::GetName(){
	std::ostringstream os;
	os << "ILCRNG_2P63_Seed";
	int minimalseedfigures = 4;
	unsigned long long mul = 10;
	for(int i = 1; i < minimalseedfigures; i++){
		if(initial_seed_ < mul)os << "0";
		mul *= 10;
	}
	os << initial_seed_;
	return os.str();
}

FastLinearCongruenceRandomNumberGenerator::FastLinearCongruenceRandomNumberGenerator(
		unsigned long long multiplier,
		unsigned long long adder,
		unsigned long long modulus,
		unsigned long long seed) :
				multiplier_(multiplier), adder_(adder), modulus_(modulus), initial_seed_(seed), seed_(seed){}

mpf_t* FastLinearCongruenceRandomNumberGenerator::RandomMpft() {
	mpf_t* rand_long_long = mpftoperations::ToMpft(RandomLongLong());
	mpf_t* modulus = mpftoperations::ToMpft(modulus_);
	mpf_t* res = mpftoperations::Divide(rand_long_long, modulus);
	mpftoperations::ReleaseValue(rand_long_long);
	mpftoperations::ReleaseValue(modulus);
	return res;
}

long long FastLinearCongruenceRandomNumberGenerator::RandomLongLong(){
	if(modulus_ > (1ULL << 63)){
		std::cerr << "The modulus is to large. It is reseted to 2^63=" << (1ULL << 63) << ".\n";
		modulus_ = (1ULL << 63);
	}

	if(adder_ >= modulus_) adder_ %= modulus_;
	if(multiplier_ >= modulus_) multiplier_ %= modulus_;
	if(seed_ >= modulus_) seed_ %= modulus_;

	unsigned long long next_seed = adder_;
	for(unsigned long long remaining_multiplier = multiplier_; remaining_multiplier > 0; remaining_multiplier /= 2) {
		if(remaining_multiplier % 2 == 1){
			next_seed += seed_;
			if(next_seed >= modulus_)next_seed -= modulus_;
			AssertCondition(next_seed < modulus_, "Modulo calculations of random number generator failed.");
		}
		seed_ = (seed_ + seed_);
		if(seed_ >= modulus_) seed_ -= modulus_;
		AssertCondition(seed_ < modulus_, "Modulo calculations of random number generator failed.");
	}
	seed_ = next_seed;
	return (long long) seed_;
}

void FastLinearCongruenceRandomNumberGenerator::LoadData(std::ifstream* inputstream, ProgramVersion* version_of_stored_data){
	(*inputstream) >> seed_;
}
void FastLinearCongruenceRandomNumberGenerator::StoreData(std::ofstream* outputstream){
	(*outputstream) << seed_ << std::endl;
}

std::string FastLinearCongruenceRandomNumberGenerator::GetName(){
	std::ostringstream os;
	os << "FLCRNG_Seed";
	int minimalseedfigures = 4;
	unsigned long long mul = 10;
	for(int i = 1; i < minimalseedfigures; i++){
		if(initial_seed_ < mul)os << "0";
		mul *= 10;
	}
	os << initial_seed_ ;
	return os.str();
}

IntenseLinearCongruenceRandomNumberGenerator::IntenseLinearCongruenceRandomNumberGenerator(
		unsigned long long multiplier,
		unsigned long long adder,
		unsigned long long modulus,
		unsigned long long seed) :
				multiplier_(multiplier), adder_(adder), modulus_(modulus), initial_seed_(seed), seed_(seed){}

mpf_t* IntenseLinearCongruenceRandomNumberGenerator::RandomMpft() {
	mpf_t* res = mpftoperations::ToMpft(0.0);
	mpf_t* divisor = mpftoperations::ToMpft(modulus_);
	int bits_per_iteration = 1;
	while(bits_per_iteration < 63 && (modulus_ >> (bits_per_iteration + 1)) > 0){
		++bits_per_iteration;
	}
	int iterations = (mpf_get_default_prec() / bits_per_iteration) + 1;
	for(int i = 0; i < iterations; i++){
		long long next_rand = RandomLongLong();
		mpf_t* next_part = mpftoperations::ToMpft(next_rand);
		mpf_t* tmp_res = mpftoperations::Add(next_part, res);
		mpftoperations::ReleaseValue(next_part);
		mpftoperations::ReleaseValue(res);
		res = mpftoperations::Divide(tmp_res, divisor);
		mpftoperations::ReleaseValue(tmp_res);
	}
	mpftoperations::ReleaseValue(divisor);
	return res;
}

long long IntenseLinearCongruenceRandomNumberGenerator::RandomLongLong(){
	if(modulus_ > (1ULL << 63)){
		std::cerr << "The modulus is to large. It is reseted to 2^63=" << (1ULL << 63) << ".\n";
		modulus_ = (1ULL << 63);
	}

	if(adder_ >= modulus_) adder_ %= modulus_;
	if(multiplier_ >= modulus_) multiplier_ %= modulus_;
	if(seed_ >= modulus_) seed_ %= modulus_;

	unsigned long long next_seed = adder_;
	for(unsigned long long remaining_multiplier = multiplier_; remaining_multiplier > 0; remaining_multiplier /= 2) {
		if(remaining_multiplier % 2 == 1){
			next_seed += seed_;
			if(next_seed >= modulus_)next_seed -= modulus_;
		}
		seed_ = (seed_ + seed_);
		if(seed_ >= modulus_) seed_ -= modulus_;
	}
	seed_ = next_seed;
	return (long long) seed_;
}

void IntenseLinearCongruenceRandomNumberGenerator::LoadData(std::ifstream* inputstream, ProgramVersion* version_of_stored_data){
	(*inputstream) >> seed_;
}
void IntenseLinearCongruenceRandomNumberGenerator::StoreData(std::ofstream* outputstream){
	(*outputstream) << seed_ << std::endl;
}

std::string IntenseLinearCongruenceRandomNumberGenerator::GetName(){
	std::ostringstream os;
	os << "ILCRNG_Seed";
	int minimalseedfigures = 4;
	unsigned long long mul = 10;
	for(int i = 1; i < minimalseedfigures; i++){
		if(initial_seed_ < mul)os << "0";
		mul *= 10;
	}
	os << initial_seed_;
	return os.str();
}

} // namespace highprecisionpso
