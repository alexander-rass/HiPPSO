/**
* @file   hyperbolic.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file contains the hyperbolic bound handling strategy.
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

#ifndef HIGH_PRECISION_PSO_BOUND_HANDLING_HYPERBOLIC_H_
#define HIGH_PRECISION_PSO_BOUND_HANDLING_HYPERBOLIC_H_

#include "bound_handling/bound_handling.h"

namespace highprecisionpso {

/**
* @brief This class implements the bound handling strategy hyperbolic for the particle swarm optimization.
*
* This bound handling strategy applies at each step.
* For each dimension the new velocity \f$vel[i]\f$ is scaled by \f$1/(\vert\frac{vel[i]}{av}\vert +1)\f$,
* where \f$av\f$ is the distance to the search space bound in direction of the velocity.
* As this bound handling strategy already adjusts the velocity no further velocity adjustment is applied.
* The new position is calculated by adding the adjusted new velocity to the old position.
*/
class BoundHandlingHyperbolic : public BoundHandling{
public:
	/**
	* @brief Updates the position of the particle p according to the current position and the new (already calculated) velocity.
	*
	* First the new velocity \f$vel[i]\f$ is scaled by \f$1/(\vert\frac{vel[i]}{av}\vert +1)\f$,
	* where \f$av\f$ is the distance to the search space bound in direction of the velocity.
	* As this bound handling strategy already adjusts the velocity no further velocity adjustment is applied.
	* The new position is calculated by adding the adjusted new velocity to the old position.
	* 
	* @param p The particle, which should be updated.
	*/
	void SetParticleUpdate(Particle * p);
	std::string GetName();
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_BOUND_HANDLING_HYPERBOLIC_H_ */
