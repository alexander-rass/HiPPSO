/**
* @file   position_and_velocity_updater/orientation_change_updater.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2014
* @brief  This file contains a position and velocity updater, which changes orientation if some directions have little movement.
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

#ifndef HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_ORIENTATION_CHANGE_UPDATER_H_
#define HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_ORIENTATION_CHANGE_UPDATER_H_

#include <fstream>
#include <gmp.h>
#include <vector>

#include "position_and_velocity_updater/position_and_velocity_updater.h"

namespace highprecisionpso {

/**
* @brief The orientation change updater implements an update procedure, which changes orientation if some directions have little relative movement.
*
* See update function for detailed information about the update process.
*/
class OrientationChangeUpdater : public PositionAndVelocityUpdater {
public:
	/**
	* @brief The constructor.
	*
	* @param log2_limit The allowed logarithmic range for the scale of movement in different directions.
	*/
	OrientationChangeUpdater(double log2_limit);
	/**
	* @brief Processes a single movement of the specified particle.
	*
	* Random values of the standard movement equations are calculated in an orthogonal transformed space.
	* In the orthogonal transformed space the scale of movement in each dimension is calculated.
	* First we sum up the absolute value of the velocity, the distance to the local attractor and the distance to the global attractor.
	* The scale of movement is then the logarithm of this sum.
	* If in the current orientation the minimal and maximal value of the scale of movement differ more than log2limit
	* then the lowest and largest directions are rotated with a hadamard transformation.
	* Beside that change the default behavior is used.
	*
	* @param p The particle, which should be updated.
	*/
	void Update(Particle* p);
	std::string GetName();
	void LoadData(std::ifstream* input_stream, ProgramVersion* version_of_stored_data);
	void StoreData(std::ofstream* output_stream);

private:
	std::vector<mpf_t*> Rotation(std::vector<mpf_t*>);
	std::vector<mpf_t*> InverseRotation(std::vector<mpf_t*>);

	const double log2_limit_;
	std::vector<std::vector<mpf_t* > > orthogonal_transformation_matrix_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_ORIENTATION_CHANGE_UPDATER_H_ */
