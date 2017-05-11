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
#include <vector>

#include "position_and_velocity_updater/position_and_velocity_updater.h"

namespace highprecisionpso {

/**
* @brief The delta updater implements a specific position and velocity update procedure.
*
* In most cases this updater acts according to the standard movement equations,
* but if the velocity and the distance to the global attractor drops below some bound
* the velocity is reinitialized with small random values.
* This variant is described in the paper "Particle Swarm Optimization Almost Surely Finds Local Optima" by M. Schmitt and R. Wanka published in Proceedings of GECCO 2013.
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
    * According to the result of function DimensionsOfDeltaUpdate for some dimensions the velocities are reseted to some small random values.
    * In all other dimensions the standard movement equations apply.
    *
    * @param p The particle, which should be updated.
    */
	void Update(Particle* p);
	std::string GetName();
	void LoadData(std::ifstream* input_stream, ProgramVersion* version_of_stored_data);
	void StoreData(std::ofstream* output_stream);

    /**
    * @brief Determines in which dimensions the delta update should be applied.
    *
    * If the sum of absolute values of the velocity plus the distance to the global attractor drops below a bound for all dimensions of the current particle,
    * then the velocity is reinitialized with some small values.
    * In all other cases the standard movement equations for the particle swarm algorithm are used.
    * This variant is described in the paper "Particle Swarm Optimization Almost Surely Finds Local Optima" by M. Schmitt and R. Wanka published in Proceedings of GECCO 2013.
    *
    * @param p The particle, which should be updated.
    *
    * @return A boolean vector, which contains a truth value for each dimension specifying whether the delta update should be applied to the respective dimension.
    */
    virtual std::vector<bool> DimensionsOfDeltaUpdate(Particle* p);
    /**
    * @brief Returns the number of delta updates per particle and dimension.
    *
    * For each particle and each dimension the number of delta updates is counted.
    *
    * @return The current counters for all particles and dimensions enclosed in a vector of vectors.
    */
    std::vector<std::vector<unsigned long long> > NumberOfDeltaUpdates();
protected:
	mpf_t* delta_;
	const double initial_delta_;
	const double gamma_;
private:
    void CheckInitDeltaUpdateCounters_();
    std::vector<std::vector<unsigned long long> > delta_update_counters_;
};

/**
* @brief The delta updater implements a specific position and velocity update procedure.
*
* In most cases this updater acts according to the standard movement equations,
* but if the velocity and the distance to the global attractor drops below some bound
* the velocity is reinitialized with small random values.
* This variant is described in the paper "Particle Swarm Optimization Almost Surely Finds Local Optima" by M. Schmitt and R. Wanka published in Journal TCS 2014.
*/
class DeltaUpdaterTCS : public DeltaUpdater {
public:
	/**
	* @brief The constructor, where the fixed bound for sum of absolute velocities and distance to the global optimum can be specified.
	*
	* @param delta The bound for velocities and the distance to the global attractor.
	*/
	DeltaUpdaterTCS(double delta);
	/**
	* @brief The constructor, where the adaptable bound for the sum of absolute velocities and distance to the global optimum can be specified.
	*
	* The bound is initially the value of initial_delta.
	* For each usage of the non standard movement equations the bound is multiplied by gamma.
	*
	* @param initial_delta The initial bound for velocities and the distance to the global attractor.
	* @param gamma The scaling factor after each non standard move of a particle.
	*/
	DeltaUpdaterTCS(double initial_delta, double gamma);

    /**
    * @brief Determines in which dimensions the delta update should be applied.
    *
    * If the sum of absolute values of the velocity plus the distance to the global attractor drops below a bound for all particles in the current dimension,
    * then the velocity is reinitialized with some small values.
    * In all other cases the standard movement equations for the particle swarm algorithm are used.
    * This variant is described in the paper "Particle Swarm Optimization Almost Surely Finds Local Optima" by M. Schmitt and R. Wanka published in Journal TCS 2014.
    *
    * @param p The particle, which should be updated.
    *
    * @return A boolean vector, which contains a truth value for each dimension specifying whether the delta update should be applied to the respective dimension.
    */
    virtual std::vector<bool> DimensionsOfDeltaUpdate(Particle* p);

	std::string GetName();
};
} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_DELTA_UPDATER_H_ */

