/**
* @file   position_and_velocity_updater/dimension_independent_updater.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   August, 2015
* @brief  This file contains a position and velocity updater, which acts independent to the coordinate axes.
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

#ifndef HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_DIMENSION_INDEPENDENT_UPDATER_H_
#define HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_DIMENSION_INDEPENDENT_UPDATER_H_

#include "position_and_velocity_updater/position_and_velocity_updater.h"

namespace highprecisionpso {

/**
* @brief This class implements an update procedure, which acts independent to the coordinate axes.
*
* See update function for detailed information about the update process.
*/
class DimensionIndependentUpdater : public PositionAndVelocityUpdater {
public:
	/**
	* @brief The constructor.
	*
	* @param reduction The random proportion in each processed direction.
	* @param max_directions The maximal number of processed direction in each update process.
	*/
	DimensionIndependentUpdater(double reduction, int max_directions);

	/**
	* @brief Processes a single movement of the specified particle.
	*
	* Each update is performed in at most max_directions directions.
	* Each update direction is calculated orthogonally to the previous directions and
	* is performed in main movement direction plus some random direction scaled by the specified reduction.
	* The smaller the value of reduction is the greater is the portion of the
	* main movement direction belonging to the first sampled direction.
	*
	* Formally:
	*
	* First the previous velocity is scaled by the specified swarm parameter.
	* The main movement direction is
	* the difference vector of local and global attractor if they are not equal.
	* Otherwise the main movement direction is the difference vector of position and any of the equal attractors.
	* If this vector is a zero vector then position and attractors are equal and therefore
	* there is no vector to local/global attractor, which can be scaled randomly.
	*
	* The directions are sampled iteratively:
	*
	* Initially there are no used directions.
	*
	* For d from 1 to (max_directions - 1) do the following:
	*
	* Choose a direction uniformly at random orthogonal to used directions.
	* The remaining main direction is the part of main movement direction which is orthogonal to used directions.
	* Normalize both directions to unit length.
	* The next direction is then the random direction * reduction + remaining main direction.
	* The smaller the value of reduction is the more of the remaining main direction
	* will be covered by the next direction.
	* Perform standard PSO velocity update with local/global attractor in this direction.
	* Add the next direction to used directions.
	*
	* After that calculate the orthogonal part of difference vector to attractors and
	* perform standard PSO velocity update in this directions.
	*
	* @param p The particle, which should be updated.
	*/
	void Update(Particle* p);
	std::string GetName();

private:
	const double reduction_;
	const int max_directions_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_DIMENSION_INDEPENDENT_UPDATER_H_ */



