/**
* @file   position_and_velocity_updater/delta_updater.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2014
* @brief  This file contains a class for processing the velocity and position update of each particle with random reinitialization of velocity in case of low energy.
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

#ifndef HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_DELTA_UPDATER_H_
#define HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_DELTA_UPDATER_H_

#include <fstream>
#include <gmp.h>

#include "position_and_velocity_updater/position_and_velocity_updater.h"

namespace highprecisionpso {

/**
* @brief The delta updater implements a specific position and velocity update procedure.
*
* In most cases this updater acts according to the standard movement equations,
* but if the velocity and the distance to the global attractor drops below some bound
* the velocity is reinitialized with small random values.
*/
class DeltaUpdater : public PositionAndVelocityUpdater {
public:
	/**
	* @brief The constructor, where the fixed bound for sum of absolute velocities and distance to the global optimum can be specified.
	*
	* @param delta The bound for velocities and the distance to the global attractor.
	*/
	DeltaUpdater(double delta);
	/**
	* @brief The constructor, where the adaptable bound for the sum of absolute velocities and distance to the global optimum can be specified.
	*
	* The bound is initially the value of initial_delta.
	* For each usage of the non standard movement equations the bound is multiplied by gamma.
	*
	* @param initial_delta The initial bound for velocities and the distance to the global attractor.
	* @param gamma The scaling factor after each non standard move of a particle.
	*/
	DeltaUpdater(double initial_delta, double gamma);
	/**
	* @brief Processes a single movement of the specified particle.
	*
	* If the sum of absolute values of the velocity plus the distance to the global attractor drops below a bound,
	* then the velocity is reinitialized with some small values.
	* In all other cases the standard movement equations for the particle swarm algorithm are used.
	*
	* @param p The particle, which should be updated.
	*/
	void Update(Particle* p);
	std::string GetName();
	void LoadData(std::ifstream* input_stream, ProgramVersion* version_of_stored_data);
	void StoreData(std::ofstream* output_stream);

private:
	const double initial_delta_;
	const double gamma_;
	mpf_t* delta_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_DELTA_UPDATER_H_ */

