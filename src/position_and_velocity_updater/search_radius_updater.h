/**
* @file   position_and_velocity_updater/search_radius_updater.h
* @author Alexander Dietsch (alexander.dietsch@fau.de)
* @date   August, 2020
* @brief This file contains a class for processing the velocity and position update of each particle with random offset in position that scales with particle velocity. 
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

#ifndef HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_SEARCH_RADIUS_UPDATER_H_
#define HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_SEARCH_RADIUS_UPDATER_H_

#include "position_and_velocity_updater/position_and_velocity_updater.h"

namespace highprecisionpso {

/**
* @brief This search radius updater implements an updater which adds an random offset chosen from positions within a cube
* to the new position which scales with the velocity.
*
* See update function for more details.
*/
class SearchRadiusUpdaterCube : public PositionAndVelocityUpdater {
public:
	/**
	* @brief Constructor, where the coefficient for scaling the position offset and the number of additional sample points can be set.
	*
	* @param coefficient_radius A scaling coefficient for the radius of the random offset.
	* @param sample_points The number of additional sample points in the search radius.
	*/
	SearchRadiusUpdaterCube(double coefficient_radius = 1, long long sample_points = 0);

	/**
	* @brief Processes a single movement of the specified particle.
	*
	* As a first step the new velocity and position is determined by the default particle swarm equations.
	* Around this position a search area is spanned which size depends on the length of the velocity vector,
	* the radius coefficient and a random value between [0, 1].
	*
	* The area a sample point is chosen from is a hyper cube with the default particle position as center.
	*
	* If the number of additional sampling points is 0, the new position is randomly chosen within this area.
	* Otherwise additional points within the area are sampled and the best is chosen for new position.
	*/
	void Update(Particle* p);

	std::string GetName();
private:
	const double coefficient_radius_;
	long long sample_points_;
};

/**
* @brief This search radius updater implements an updater which adds an random offset chosen from positions created by a normal distribution
* to the new position which scales with the velocity.
*
* See update function for more details.
*/
class SearchRadiusUpdaterNormalDistr : public PositionAndVelocityUpdater {
public:
	/**
	* @brief Constructor, where the coefficient for scaling the position offset and the number of additional sample points can be set.
	*
	* @param coefficient_radius A scaling coefficient for the radius of the random offset.
	* @param sample_points The number of additional sample points in the search radius.
	*/
	SearchRadiusUpdaterNormalDistr(double coefficient_radius = 1, long long sample_points = 0);

	/**
	* @brief Processes a single movement of the specified particle.
	*
	* As a first step the new velocity and position is determined by the default particle swarm equations.
	* Around this position a search area is created with a normal distribution and the default particle position as expected value.
	* The standard deviation is dependant on the velocity vector lenght and the radius coefficient.
	*
	* If the number of additional sampling points is 0, the new position is randomly chosen within this area.
	* Otherwise additional points within the area are sampled and the best is chosen for new position.
	*/
	void Update(Particle* p);

	std::string GetName();
private:
	const double coefficient_radius_;
	long long sample_points_;
};




} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_SEARCH_RADIUS_UPDATER_H_ */
