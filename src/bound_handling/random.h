/**
* @file   bound_handling/random.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file contains the random bound handling strategy.
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

#ifndef HIGH_PRECISION_PSO_BOUND_HANDLING_RANDOM_H_
#define HIGH_PRECISION_PSO_BOUND_HANDLING_RANDOM_H_

#include "bound_handling/bound_handling.h"

namespace highprecisionpso {

/**
* @brief This class implements the bound handling strategy random for the particle swarm optimization.
*
* If a particle would leave the search space, the new position is set to a random position inside the search space.
*/
class BoundHandlingRandom : public BoundHandling{
public:
	/**
	* @brief The constructor.
	*
	* @param reset_all Specifies whether all position entries will be reinitialized randomly, or just the dimensions, which leave the search space.
	*/
	BoundHandlingRandom(bool reset_all);
	/**
	* @brief Updates the position of the particle p according to the current position and the new (already calculated) velocity.
	*
	* The new position is calculated by adding the velocity to the position.
	* If the new position would be outside of the search space bounds then the new position is initialized randomly.
	* Afterwards the specified velocity adjustment is activated with suitable parameters.
	* 
	* @param p The particle, which should be updated.
	*/
	void SetParticleUpdate(Particle * p);
	std::string GetName();

private:
	const bool reset_all_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_BOUND_HANDLING_RANDOM_H_ */
