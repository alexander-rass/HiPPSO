/**
* @file   particle.h 
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains information about the particles of the swarm.
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

#ifndef HIGH_PRECISION_PSO_GENERAL_PARTICLE_H_
#define HIGH_PRECISION_PSO_GENERAL_PARTICLE_H_

#include <fstream>
#include <gmp.h>
#include <vector>

#include "general/general_objects.h"

namespace highprecisionpso {

/**
* @brief The particles for the particle swarm optimization algorithm.
*/
class Particle {
public:
	/**
	* @brief The constructor.
	*
	* Initializes the index of the particle. Position and velocity need to be set manually with the set methods.
	*/
	Particle();

	/**
	* @brief Clones the position of the current local attractor.
	*
	* @return The position of the local attractor.
	*/
	std::vector<mpf_t*> GetLocalAttractorPosition();
	/**
	* @brief Calculates the objective function value of the local attractor.
	*
	* @return The function value.
	*/
	mpf_t* GetLocalAttractorValue();
	/**
	* @brief Clones the position of the particle.
	*
	* @return The position.
	*/
	std::vector<mpf_t*> GetPosition();
	/**
	* @brief Clones the velocity of the particle.
	*
	* @return The velocity.
	*/
	std::vector<mpf_t*> GetVelocity();

	/**
	* @brief Sets the position of the local attractor to the supplied position. 
	*
	* @param pos The new position of the local attractor.
	*/
	void SetLocalAttractorPosition(std::vector<mpf_t*> pos);
	/**
	* @brief Sets the position to the supplied position.
	*
	* Additionally it checks whether the new position is better than the local attractor.
	* If this is the case the local attractor position is updated and the updateGlobalAttractor function is called.
	*
	* @param pos The new position of the particle.
	*/
	void SetPosition(std::vector<mpf_t*> pos);
	/**
	* @brief Sets the velocity to the supplied velocity. 
	*
	* @param vel The new velocity of the particle.
	*/
	void SetVelocity(std::vector<mpf_t*> vel);

	/**
	* @brief Informs the neighborhood about a new local attractor, which might be relevant for the choice of a global attractor.
	*
	* @param goodPosition The position of the new local attractor.
	* @param goodValue The value of the objective function at the specified position.
	*/
	void UpdateGlobalAttractor(std::vector<mpf_t*> goodPosition,
			mpf_t* goodValue);
	/**
	* @brief Tells the position and velocity updater to update position and velocity of this particle. 
	*/
	void UpdatePosition();

	/**
	* @brief Loads the complete data of this particle from some input stream.
	*
	* @param input_stream The input stream.
	* @param version_of_stored_data The version of the program at the time of backup generation.
	*/
	void LoadData(std::ifstream* input_stream, ProgramVersion* version_of_stored_data);
	/**
	* @brief Stores the complete data of this particle to some output stream.
	*
	* @param output_stream The output stream.
	*/
	void StoreData(std::ofstream* output_stream);

	/**
	* @brief The index of the particle.
	* @warning Do not change this value.
	*/
	int id;
	/**
	* @brief The local attractor of the particle.
	* @warning Change this value only with the set method.
	*/
	std::vector<mpf_t*> local_attractor_position;
	/**
	* @brief The position of the particle.
	* @warning Change this value only with the set method.
	*/
	std::vector<mpf_t*> position;
	/**
	* @brief The velocity of the particle.
	* @warning Change this value only with the set method.
	*/
	std::vector<mpf_t*> velocity;

private:
	static int active_particles_;
	mpf_t* local_attractor_value_cached_;
	unsigned int local_attractor_value_cached_precision_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_GENERAL_PARTICLE_H_ */
