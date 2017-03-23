/**
* @file   bound_handling/random_forth.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   March, 2015
* @brief  This file contains the random forth bound handling strategy.
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

#ifndef HIGH_PRECISION_PSO_BOUND_HANDLING_RANDOM_FORTH_H_
#define HIGH_PRECISION_PSO_BOUND_HANDLING_RANDOM_FORTH_H_

#include "bound_handling/bound_handling.h"

namespace highprecisionpso {

/**
* @brief This enumeration describes,
* which dimensions are marked as modified,
* if random forth bound handling is active.
* This determines, for which dimensions the velocity adjustment is used.
*/
enum RandomForthMarkDimensions { 
	/**
	* @brief If the position of at least one dimension is situated outside of the search space bounds, the velocity adjustment is applied on all dimensions.
	*/
	RANDOM_FORTH_MARK_DIMENSIONS_ALL_ON_CHANGE,
	/**
	* @brief The velocity adjustment is applied only in dimensions, where the position is situated outside of the search space bounds.
	*/
	RANDOM_FORTH_MARK_DIMENSIONS_OUTSIDE_BOUNDS,
	/**
	* @brief The velocity adjustment is applied only in dimensions, where the new position is situated exactly on the search space bounds.
	* (The event that the new position is situated exactly on the search space bounds is very unlikely).
	*/
	RANDOM_FORTH_MARK_DIMENSIONS_ON_BOUNDARY
};

/**
* @brief This class implements the bound handling strategy random forth for the particle swarm optimization.
*
* If a particle would leave the search space,
* the actual new position is sampled uniformly at random
* on the part of the segment between the old position and the new position,
* which is inside the search space bounds.
*/
class BoundHandlingRandomForth : public BoundHandling{
public:
	/**
	* @brief The constructor.
	*
	* @param random_forth_mark_dimensions Specifies on which dimensions the velocity adjustment is applied.
	*/
	BoundHandlingRandomForth(RandomForthMarkDimensions random_forth_mark_dimensions);
	/**
	* @brief Updates the position of the particle p according to the current position and the new (already calculated) velocity.
	*
	* The new position is calculated by adding the velocity to the position.
	* If the new position would be outside of the search space bounds then the bound handling strategy random forth is activated.
	* Afterwards the specified velocity adjustment is activated with suitable parameters.
	* 
	* @param p The particle, which should be updated.
	*/
	void SetParticleUpdate(Particle * p);
	std::string GetName();

private:
	const RandomForthMarkDimensions random_forth_mark_dimensions_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_BOUND_HANDLING_RANDOM_FORTH_H_ */
