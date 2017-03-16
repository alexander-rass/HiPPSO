/**
* @file   bound_handling.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains an abstract class for different bound handling strategies, which can be used for the particle swarm optimization.
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

#ifndef HIGH_PRECISION_PSO_BOUND_HANDLING_BOUND_HANDLING_H_
#define HIGH_PRECISION_PSO_BOUND_HANDLING_BOUND_HANDLING_H_

#include <gmp.h>
#include <string>
#include <vector>

#include "general/particle.h"

namespace highprecisionpso {

/**
* @brief This class is an abstract class. It supplies the interface for bound handling strategies of the particle swarm optimization algorithm.
*/
class BoundHandling{
public:
	/**
	* @brief The destructor.
	*/
	virtual ~BoundHandling(){};
	/**
	* @brief Updates the position of the particle p according to the current position and the new (already calculated) velocity.
	*
	* The new position is calculated by adding the velocity to the position.
	* If the new position would be outside of the search space bounds then the bound handling strategy should be activated.
	* Afterwards the specified velocity adjustment should be activated with suitable parameters.
	* 
	* @warning Do not forget to call the velocity adjustment method of the velocity adjustment specified in the configuration section.
	*
	* @param p The particle, which should be updated.
	*/
	virtual void SetParticleUpdate(Particle * p) = 0;
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
	/**
	* @brief Calculates the difference vector between the position and the aim.
	* 
	* The standard implementation just calculates the aim vector minus the position vector.
	* For bound handling strategies, which simulate repeated search spaces, this could be changed.
	*
	* @param position The vector, which describes the position.
	* @param aim The vector, which describes the aim.
	*
	* @return The difference from the position to the aim.
	*/
	virtual std::vector<mpf_t*> GetDirectionVector(const std::vector<mpf_t*> & position, const std::vector<mpf_t*> & aim);
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_BOUND_HANDLING_BOUND_HANDLING_H_ */
