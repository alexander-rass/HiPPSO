/**
* @file   random_back.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file contains the random back velocity adjustment strategy.
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

#ifndef HIGH_PRECISION_PSO_VELOCITY_ADJUSTMENT_RANDOM_BACK_H_
#define HIGH_PRECISION_PSO_VELOCITY_ADJUSTMENT_RANDOM_BACK_H_

#include "velocity_adjustment/velocity_adjustment.h"

namespace highprecisionpso {

/**
* @brief This class implements the random back velocity adjustment strategy.
*/
class VelocityAdjustmentRandomBack: public VelocityAdjustment{
public:
	/**
	* @brief Adjusts the velocity of a particle after the position update is finished. 
	*
	* The velocity is negated and scaled by a random factor between 0 and 1 in all marked dimensions.
	*
	* @param p The particle, which should be updated.
	* @param adjust_dimensions Marks the dimensions, which should be adjusted with the value true. 
	* @param old_position The previous position of the particle.
	*/
	virtual void AdjustVelocity(Particle * p, std::vector<bool> adjust_dimensions, std::vector<mpf_t*> old_position);
	virtual std::string GetName();
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_VELOCITY_ADJUSTMENT_RANDOM_BACK_H_ */
