/**
* @file   position_and_velocity_updater/position_and_velocity_updater.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains an abstract class for processing the velocity and position update of each particle.
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

#ifndef HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_POSITION_AND_VELOCITY_UPDATER_H_
#define HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_POSITION_AND_VELOCITY_UPDATER_H_

#include <fstream>
#include <string>

#include "general/general_objects.h"
#include "general/particle.h"
#include "statistics/statistics.h"

namespace highprecisionpso {

/**
* @brief This class is an abstract class for processing the velocity and position update of each particle.
*/
class PositionAndVelocityUpdater {
public:
	/**
	* The destructor.
	*/
	virtual ~PositionAndVelocityUpdater(){}
	/**
	* @brief This method initializes the velocity and position update of the specified particle.
	*
	* The new velocity is directly calculated in this method.
	* After the new velocity is calculated the bound handling strategy determines the new position.
	* @warning Do not forget to call the method of the bound handling specified in configuration section for setting the new position.
	*
	* @param p The particle, which should be updated.
	*/
	virtual void Update(Particle* p) = 0;
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
	/**
	* @brief Loads data to reset the state of the position and velocity updater to the state which was stored.
	*
	* @param input_stream The stream which contains the data about the state of the position and velocity updater.
	* @param version_of_stored_data The version of the program when the backup was created.
	*/
	virtual void LoadData(std::ifstream* input_stream, ProgramVersion* version_of_stored_data);
	/**
	* @brief Stores the current state of this position and velocity updater such that it can completely restore the current state with this data.
	*
	* @param output_stream The output stream where the state should be stored.
	*/
	virtual void StoreData(std::ofstream* output_stream);
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_POSITION_AND_VELOCITY_UPDATER_POSITION_AND_VELOCITY_UPDATER_H_ */
