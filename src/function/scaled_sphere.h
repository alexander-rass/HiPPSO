/**
* @file   function/scaled_sphere.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   January, 2014
* @brief  This file contains the description of the scaled sphere functions.
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_SCALED_SPHERE_H_
#define HIGH_PRECISION_PSO_FUNCTION_SCALED_SPHERE_H_

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This class represents a scaled version of the well known sphere function.
*/
class ScaledSphere: public Function {
public:
	/**
	* @brief The constructor, where the scale can be specified.
	*
	* @param max_scale The maximal scale.
	*/
	ScaledSphere(double max_scale);
	/**
	* @brief The destructor.
	*/
	~ScaledSphere();
	/**
	* @brief Returns the evaluation of the scaled sphere function evaluated at the given position.
	*
	* @param pos The position vector.
	*
	* @return &sum;<sub>d=0..D-1</sub> max_scale<sup>d/(D-1)</sup>pos[d]<sup>2</sup>.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	/**
	* @brief Calculates for a position and a dimension the next local optimum if only the value of the position in the specified dimension is varied and returns the distance to it.
	*
	* @param pos The position.
	* @param d The index of the observed dimension.
	*
	* @return The absolute value of the position entry in dimension d.
	*/
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);

private:
	const double max_scale_;
	std::vector<mpf_t*> scales_cached_;
	unsigned int scales_cached_precision_;
};

/**
* @brief This class represents a scaled version of the well known sphere function with fixed scale 10<sup>6</sup>.
*/
class ScaledSphereFixed: public ScaledSphere {
public:
	/**
	* @brief The constructor.
	*/
	ScaledSphereFixed();
};

/**
* @brief This class represents a scaled and rotated version of the well known sphere function.
*/
class ScaledHadamardRotatedSphere: public Function {
public:
	/**
	* @brief The constructor, where the scale can be specified.
	*
	* @param max_scale The maximal scale.
	*/
	ScaledHadamardRotatedSphere(double max_scale);
	/**
	* @brief The destructor.
	*/
	~ScaledHadamardRotatedSphere();
	/**
	* @brief Returns the evaluation of the scaled sphere function at the hadamard transformed position.
	*
	* @param pos The position vector.
	*
	* @return Evaluation of the scaled sphere function on hadamard transformed position.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	/**
	* @brief Calculates for a position and a dimension the next local optimum if only the value of the position in the specified dimension is varied and returns the distance to it.
	*
	* For a given position all values (beside the value in the specified dimension) are regarded as constants.
	* By the evaluation of a quadratic equation,
	* where the value of the position in the specified dimension is varied,
	* a local optimum is calculated.
	* The result is the distance of the evaluated position and the found local optimum.
	*
	* @param pos The position.
	* @param d The index of the observed dimension.
	*
	* @return The distance to the next local optimum in a single dimension setting.
	*/
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);

private:
	void InitMatrixCached(const std::vector<mpf_t*> & pos);
	const double max_scale_;
	std::vector<std::vector<mpf_t* > > matrix_cached_;
	unsigned int matrix_cached_precision_;
};

/**
* @brief This class represents a scaled and rotated version of the well known sphere function with fixed scale 10<sup>6</sup>.
*/
class ScaledHadamardRotatedSphereFixed: public ScaledHadamardRotatedSphere {
public:
	/**
	* @brief The constructor.
	*/
	ScaledHadamardRotatedSphereFixed();
};

/**
* @brief This class represents a randomly scaled version of the well known sphere function.
*/
class ScaledSphereRand: public Function {
public:
	/**
	* @brief The constructor, where the random number generator can be specified.
	*
	* @param rng_description The description of the random number generator.
	*/
	ScaledSphereRand(std::vector<std::string> rng_description);
	/**
	* @brief The destructor.
	*/
	~ScaledSphereRand();
	/**
	* @brief Returns the evaluation of the randomly scaled sphere function evaluated at the given position.
	*
	* @param pos The position vector.
	*
	* @return &sum;<sub>d=0..D-1</sub> scale[d] &middot; pos[d]<sup>2</sup>.
	*
	* scale is a once randomly generated vector with non negative entries.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	/**
	* @brief Calculates for a position and a dimension the next local optimum if only the value of the position in the specified dimension is varied and returns the distance to it.
	*
	* @param pos The position.
	* @param d The index of the observed dimension.
	*
	* @return The absolute value of the position entry in dimension d.
	*/
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);

private:
	const std::vector<std::string> rng_description_;
	std::vector<mpf_t*> scales_cached_;
	unsigned int scales_cached_precision_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_SCALED_SPHERE_H_ */
