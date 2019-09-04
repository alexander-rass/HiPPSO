/**
* @file   arbitrary_precision_calculation/operations.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains a large set of operations for calculations with the mpf_t data type.
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

#ifndef ARBITRARY_PRECISION_CALCULATION_OPERATIONS_H_
#define ARBITRARY_PRECISION_CALCULATION_OPERATIONS_H_

#include <gmp.h>
#include <string>
#include <vector>

#include "arbitrary_precision_calculation/random_number_generator.h"

namespace arbitraryprecisioncalculation {

/**
* @brief Contains a large set of operations for calculations with vectors of the mpf_t data type.
*/
namespace vectoroperations {
/**
* @brief Performs element wise addition of the two vectors.
*
* @param a The first vector.
* @param b The second vector.
*
* @return The result vector of the element wise addition.
*/
std::vector<mpf_t*> Add(const std::vector<mpf_t*> & a, const std::vector<mpf_t*> & b);
/**
* @brief Performs element wise subtraction of the two vectors.
*
* @param a The first vector.
* @param b The second vector.
*
* @return The result vector of the element wise subtraction.
*/
std::vector<mpf_t*> Subtract(const std::vector<mpf_t*> & a, const std::vector<mpf_t*> & b);
/**
* @brief Performs element wise division of the two vectors.
*
* @param a The first vector.
* @param b The second vector.
*
* @return The result vector of the element wise division.
*/
std::vector<mpf_t*> Divide(const std::vector<mpf_t*> & a, const std::vector<mpf_t*> & b);
/**
* @brief Performs element wise multiplication of the two vectors.
*
* @param a The first vector.
* @param b The second vector.
*
* @return The result vector of the element wise multiplication.
*/
std::vector<mpf_t*> Multiply(const std::vector<mpf_t*> & a, const std::vector<mpf_t*> & b);
/**
* @brief Performs element wise multiplication of the two vectors.
*
* @param a The first vector.
* @param b The second vector.
*
* @return The result vector of the element wise multiplication.
*/
std::vector<mpf_t*> Multiply(const std::vector<mpf_t*> & a, const std::vector<double> & b);
/**
* @brief Performs for each element in the input vector a multiplication with the specified factor.
*
* @param a The input vector.
* @param b The specified factor.
*
* @return The result vector of multiplications.
*/
std::vector<mpf_t*> Multiply(const std::vector<mpf_t*> & a, const double & b);
/**
* @brief Performs for each element in the input vector a multiplication with the specified factor.
*
* @param a The input vector.
* @param b The specified factor.
*
* @return The result vector of multiplications.
*/
std::vector<mpf_t*> Multiply(const std::vector<mpf_t*> & a, mpf_t* b);
/**
* @brief Calculates the sum of the vector entries.
*
* @param a The input vector.
*
* @return The sum.
*/
mpf_t* Add(const std::vector<mpf_t*> &a);
/**
* @brief Creates a vector with the specified number of entries. Each entry represents the supplied value.
*
* @param dimension The specified number of entries.
* @param value The supplied value.
*
* @return The created vector.
*/
std::vector<mpf_t*> GetConstantVector(int dimension, const double & value);
/**
* @brief Creates a vector with the specified number of entries. Each entry represents the supplied value.
*
* @param dimension The specified number of entries.
* @param value The supplied value.
*
* @return The created vector.
*/
std::vector<mpf_t*> GetConstantVector(int dimension, const mpf_t* value);
/**
* @brief Transforms the vector with double entries to a vector with mpf_t* values.
*
* @param a The vector with double entries.
*
* @return The transformed vector.
*/
std::vector<mpf_t*> GetVector(const std::vector<double> & a);
/**
* @brief Release the reserved memory for the entries of the supplied vector.
*
* The supplied mpf_t objects will be reused soon.
*
* @param a The supplied vector.
*/
void ReleaseValues(const std::vector<mpf_t*> & a);
/**
* @brief Creates a copy of the supplied vector where each entry is cloned.
*
* The result vector can be stored for later use.
*
* @param a The supplied vector.
*
* @return The copy.
*/
std::vector<mpf_t*> Clone(const std::vector<mpf_t*> & a);
/**
* @brief Randomizes the supplied vector. Each entry will be multiplied by a random mpf_t value in the interval [0,1].
*
* @param a The supplied vector.
*
* @return The randomized vector.
*/
std::vector<mpf_t*> Randomize(const std::vector<mpf_t*> & a);
/**
* @brief Creates a random mpf_t* vector with the specified number of entries, which contains random values in the interval [0,1].
*
* @param dimensions The number of entries.
*
* @return The random vector.
*/
std::vector<mpf_t*> GetRandomVector(int dimensions);
/**
* @brief Converts the values of a vector with mpf_t* entries to a vector with double entries.
*
* @param a The vector with mpf_t* entries.
*
* @return The converted vector.
*/
std::vector<double> MpftToDouble(const std::vector<mpf_t*> & a);
/**
* @brief Checks whether two mpf_t* vectors are equal.
*
* @param a The first vector.
* @param b The second vector.
*
* @return true if the two vectors contain equal values.
*/
bool Equals(const std::vector<mpf_t*> & a, const std::vector<mpf_t*> & b);
/**
* @brief Calculates the orthogonal projection of the vector vec on the projection vector.
*
* The following formula is used:<br>
* ( ( vec . projection_vector ) / ( projection_vector . projection_vector ) ) &middot; projection_vector,<br>
* where ( a . b ) is dot product or scalar product.
*
* @param vec The vector which should be projected.
* @param projection_vector The projection vector.
*
* @return The projection of the vector vec on the vector projection_vector.
*/
std::vector<mpf_t*> OrthogonalProjection(const std::vector<mpf_t*> & vec, const std::vector<mpf_t*> & projection_vector);
/**
* @brief Calculates the squared euclidean length of the supplied vector.
*
* @param vec The vector.
*
* @return The squared euclidean length.
*/
mpf_t* SquaredEuclideanLength(const std::vector<mpf_t*> & vec);
/**
* @brief Sorts the elements of the supplied vector in increasing order.
*
* @param vec The vector.
*/
void Sort(std::vector<mpf_t*>* vec);

} // namespace vectoroperations

/**
* @brief Contains a large set of operations for calculations with the mpf_t data type.
*/
namespace mpftoperations {

/**
* @brief Getter function for the number of mpf_t values which are currently in use.
*
* @return The number of mpf_t values in use.
*/
int GetNumberOfMpftValuesInUse();
/**
* @brief Getter function for the number of mpf_t values which are currently cached.
*
* @return The number of mpf_t values in cache.
*/
int GetNumberOfMpftValuesCached();
/**
* @brief Changes the number of mpf_t values currently in cache by the specified number.
*
* If any class stores values for later usage then this needs to be announced here.
* Otherwise an error may occur.
* If the parameter is negative the number of mpf_t in cache decreases.
* If the parameter is positive the number of mpf_t in cache increases.
*
* @param change The change of the number of mpf_t in cache.
*/
void ChangeNumberOfMpftValuesCached(int change);
/**
* @brief Signals the start of statistical calculations.
*
* If currently statistical calculations are running then (if specified) the check whether the precision needs to be increased is not executed.
*/
void StatisticalCalculationsStart();
/**
* @brief Signals the end of statistical calculations.
*
* If currently statistical calculations are running then (if specified) the check whether the precision needs to be increased is not executed.
*/
void StatisticalCalculationsEnd();
/**
* @brief Increases the precision instantly to a higher value.
*/
void IncreasePrecision();
/**
* @brief Prepares an mpf_t value for using it.
*
* @return The pointer to the prepared mpf_t.
*/
mpf_t* GetResultPointer();
/**
* @brief Performs an addition of the two values.
*
* @param a The first value.
* @param b The second value.
*
* @return a + b
*/
mpf_t* Add(const mpf_t* a, const mpf_t* b);
/**
* @brief Performs a subtraction of the two values.
*
* @param a The first value.
* @param b The second value.
*
* @return a - b
*/
mpf_t* Subtract(const mpf_t* a, const mpf_t* b);
/**
* @brief Multiplies the value of parameter a by a factor of 2<sup>p</sup>.
*
* @param a The factor.
* @param p The exponent.
*
* @return a &middot; 2<sup>p</sup>.
*/
mpf_t* Multiply2Exp(const mpf_t* a, int p);
/**
* @brief Performs a multiplication of the two values.
*
* @param a The first value.
* @param b The second value.
*
* @return a &middot; b.
*/
mpf_t* Multiply(const mpf_t* a, const mpf_t* b);
/**
* @brief Performs a multiplication of the two values.
*
* @param a The first value.
* @param b The second value.
*
* @return a &middot; b.
*/
mpf_t* Multiply(const mpf_t* a, const double b);
/**
* @brief Performs a division of the two values.
*
* @param a The first value.
* @param b The second value.
*
* @return a / b
*/
mpf_t* Divide(const mpf_t* a, const mpf_t* b);
/**
* @brief Releases the reserved memory for the supplied mpf_t.
*
* The supplied mpf_t object will be reused soon.
*
* @param a The supplied mpf_t.
*/
void ReleaseValue(mpf_t* a);
/**
* @brief Calculates the value of Pi.
*
* @return Pi.
*/
mpf_t* GetPi();
/**
* @brief Calculates the value of E.
*
* @return E.
*/
mpf_t* GetE();
/**
* @brief Calculates the exponential function of the input.
*
* @param a The input.
*
* @return exp( a )
*/
mpf_t* Exp(mpf_t* a);
/**
* @brief Compares the two parameters.
*
* @param a The first parameter.
* @param b The second parameter.
*
* @retval "< 0" if the first parameter is less than the second parameter.
* @retval  0 if the two parameters are equal.
* @retval "> 0" if the first parameter is greater than the second parameter.
*/
int Compare(const mpf_t* a, const mpf_t* b);
/**
* @brief Compares the two parameters.
*
* @param a The first parameter.
* @param b The second parameter.
*
* @retval "< 0" if the first parameter is less than the second parameter.
* @retval  0 if the two parameters are equal.
* @retval "> 0" if the first parameter is greater than the second parameter.
*/
int Compare(const double a, const mpf_t* b);
/**
* @brief Compares the two parameters.
*
* @param a The first parameter.
* @param b The second parameter.
*
* @retval "< 0" if the first parameter is less than the second parameter.
* @retval  0 if the two parameters are equal.
* @retval "> 0" if the first parameter is greater than the second parameter.
*/
int Compare(const mpf_t* a, double b);
/**
* @brief Calculates the representation of +infinity.
*
* @return +infinity.
*/
mpf_t* GetPlusInfinity();
/**
* @brief Calculates the representation of -infinity.
*
* @return -infinity.
*/
mpf_t* GetMinusInfinity();
/**
* @brief Calculates the representation of an undefined value.
*
* @return The representation of an undefined value.
*/
mpf_t* GetUndefined();
/**
* @brief Checks whether the supplied value is the representation of +infinity or -infinity.
*
* @param value The supplied value.
*
* @retval true If the supplied value is a representation of +infinity or -infinity.
* @retval false otherwise.
*/
bool IsInfinite(const mpf_t* value);
/**
* @brief Checks whether the supplied value is the representation of +infinity.
*
* @param value The supplied value.
*
* @retval true If the supplied value is a representation of +infinity.
* @retval false otherwise.
*/
bool IsPlusInfinity(const mpf_t* value);
/**
* @brief Checks whether the supplied value is the representation of -infinity.
*
* @param value The supplied value.
*
* @retval true If the supplied value is a representation of -infinity.
* @retval false otherwise.
*/
bool IsMinusInfinity(const mpf_t* value);
/**
* @brief Checks whether the supplied value is the representation of an undefined value.
*
* @param value The supplied value.
*
* @retval true If the supplied value is a representation of undefined value.
* @retval false otherwise.
*/
bool IsUndefined(const mpf_t* value);
/**
* @brief Calculates the sine function of the supplied value.
*
* @param x The supplied value.
*
* @return sin( x )
*/
mpf_t* Sin(mpf_t* x);
/**
* @brief Calculates the cosine function of the supplied value.
*
* @param x The supplied value.
*
* @return cos( x )
*/
mpf_t* Cos(mpf_t* x);
/**
* @brief Calculates the tangent function of the supplied value.
*
* @param x The supplied value.
*
* @return tan( x )
*/
mpf_t* Tan(mpf_t* x);
/**
* @brief Calculates the arcsine function of the supplied value.
*
* @param x The supplied value.
*
* @return arcsin( x )
*/
mpf_t* Arcsin(mpf_t* x);
/**
* @brief Calculates the arccosine function of the supplied value.
*
* @param x The supplied value.
*
* @return arccos( x )
*/
mpf_t* Arccos(mpf_t* x);
/**
* @brief Calculates the arctangent function of the supplied value.
*
* @param x The supplied value.
*
* @return arctan( x )
*/
mpf_t* Arctan(mpf_t* x);
/**
* @brief Converts the supplied double value to its mpf_t representation.
*
* @param value The supplied value.
*
* @return The mpf_t representation of the supplied value.
*/
mpf_t* ToMpft(double value);
/**
* @brief Converts the supplied long long value to its mpf_t representation.
*
* @param value The supplied value.
*
* @return The mpf_t representation of the supplied value.
*/
mpf_t* ToMpft(long long value);
/**
* @brief Converts the supplied unsigned long long value to its mpf_t representation.
*
* @param value The supplied value.
*
* @return The mpf_t representation of the supplied value.
*/
mpf_t* ToMpft(unsigned long long value);
/**
* @brief Converts the supplied int value to its mpf_t representation.
*
* @param value The supplied value.
*
* @return The mpf_t representation of the supplied value.
*/
mpf_t* ToMpft(int value);
/**
* @brief Converts the supplied unsigned int value to its mpf_t representation.
*
* @param value The supplied value.
*
* @return The mpf_t representation of the supplied value.
*/
mpf_t* ToMpft(unsigned int value);
/**
* @brief Calculates the absolute value of the supplied value.
*
* @param x The supplied value.
*
* @return abs( x )
*/
mpf_t* Abs(const mpf_t* x);
/**
* @brief Calculates the negation of the supplied value.
*
* @param x The supplied value.
*
* @return -x
*/
mpf_t* Negate(const mpf_t* x);
/**
* @brief Creates a copy of the supplied value.
*
* The result value can be stored for later use.
*
* @param value The supplied value.
*
* @return A clone of the supplied value.
*/
mpf_t* Clone(const mpf_t* value);
/**
* @brief Calculates the maximum of the two values.
*
* @param a The first value.
* @param b The second value.
*
* @return max ( a , b )
*/
mpf_t* Max(mpf_t* a, mpf_t* b);
/**
* @brief Calculates the minimum of the two values.
*
* @param a The first value.
* @param b The second value.
*
* @return min ( a , b )
*/
mpf_t* Min(mpf_t* a, mpf_t* b);
/**
* @brief Calculates the largest integral value less or equal than the supplied value.
*
* The supplied value will be rounded down to the next integral value.
*
* @param value The supplied value.
*
* @return The rounded value.
*/
mpf_t* Floor(mpf_t* value);
/**
* @brief Calculates the smallest integral value greater or equal than the supplied value.
*
* The supplied value will be rounded up to the next integral value.
*
* @param value The supplied value.
*
* @return The rounded value.
*/
mpf_t* Ceil(mpf_t* value);
/**
* @brief Calculates a random mpf_t value in the range [0,1] with the supplied random number generator.
*
* @param random The random number generator.
*
* @return The calculated random number.
*/
mpf_t* GetRandomMpft(RandomNumberGenerator* random);
/**
* @brief Calculates a random mpf_t value in the range [0,1] with the specified standard random number generator for this execution.
*
* @return The calculated random number.
*/
mpf_t* GetRandomMpft();
/**
* @brief Calculates a random value with normal distribution (gaussian distribution).
*
* @param mu The expectation of the normal distribution.
* @param sigma The standard deviation of the normal distribution (this is the square root of the variance).
* @param random The random number generator.
*
* @return The calculated random number.
*/
mpf_t* GetGaussianRandomMpft(double mu, double sigma, RandomNumberGenerator* random);
/**
* @brief Calculates a random value with normal distribution (gaussian distribution).
*
* @param mu The expectation of the normal distribution.
* @param sigma The standard deviation of the normal distribution (this is the square root of the variance).
*
* @return The calculated random number.
*/
mpf_t* GetGaussianRandomMpft(double mu, double sigma);
/**
* @brief Multiplies the supplied value by a random value in the interval [0,1].
*
* @param value The supplied value.
*
* @return The result of the multiplication.
*/
mpf_t* Randomize(mpf_t* value);
/**
* @brief Multiplies the supplied value by a random value in the interval [0,1].
*
* @param value The supplied value.
* @param random The used random number generator.
*
* @return The result of the multiplication.
*/
mpf_t* Randomize(mpf_t* value, RandomNumberGenerator* random);
/**
* @brief Converts an mpf_t value to a double value.
*
* @param value The mpf_t value.
*
* @return The converted value.
*/
double MpftToDouble(mpf_t* value);
/**
* @brief Calculates the value to the power of the specified exponent.
*
* @param value The value.
* @param exponent The exponent.
*
* @return value to the power of exponent.
*/
mpf_t* Pow(mpf_t* value, int exponent);
/**
* @brief Calculates the value to the power of the specified exponent.
*
* @param value The value.
* @param exponent The exponent.
*
* @return value to the power of exponent.
*/
mpf_t* Pow(mpf_t* value, double exponent);
/**
* @brief Calculates the value to the power of the specified exponent.
*
* @param value The value.
* @param exponent The exponent.
*
* @return value to the power of exponent.
*/
mpf_t* Pow(mpf_t* value, mpf_t* exponent);
/**
* @brief Calculates the square root of the specified value.
*
* @param value The value
*
* @return The square root of the specified value.
*/
mpf_t* Sqrt(mpf_t* value);
/**
* @brief Calculates the base E logarithm of the specified value.
*
* @param value The value.
*
* @return The base E logarithm of the specified value.
*/
mpf_t* LogE(mpf_t* value);
/**
* @brief Calculates the base 2 logarithm of the specified value.
*
* @param value The value.
*
* @return The base 2 logarithm of the specified value.
*/
double Log2Double(mpf_t* value);

/**
* @brief Converts an mpf_t value to a string representation of it.
*
* @param value The value which should be converted.
*
* @return The string representation.
*/
std::string MpftToString(const mpf_t* value);

/**
* @brief Writes the content of a mpf_t value to the given stream such that it can completely recovered.
*
* @param value The mpf_t value.
* @param stream The stream.
*/
void StoreMpft(mpf_t* value, std::ofstream* stream);
/**
* @brief Recovers a mpf_t value from an input stream.
*
* @param stream The stream.
*
* @return The recovered value.
*/
mpf_t* LoadMpft(std::ifstream* stream);

} // namespace mpftoperations
} // namespace arbitraryprecisioncalculation

#endif /* ARBITRARY_PRECISION_CALCULATION_OPERATIONS_H_ */
