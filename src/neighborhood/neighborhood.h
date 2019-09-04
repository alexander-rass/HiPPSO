/**
* @file   neighborhood/neighborhood.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2015
* @brief  This file contains an abstract classes for neighborhood generation.
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

#ifndef HIGH_PRECISION_PSO_NEIGHBORHOOD_NEIGHBORHOOD_H_
#define HIGH_PRECISION_PSO_NEIGHBORHOOD_NEIGHBORHOOD_H_

#include <fstream>
#include <gmp.h>
#include <queue>
#include <vector>

#include "general/general_objects.h"
#include "general/particle.h"

namespace highprecisionpso {

/**
* @brief This class is an abstract class. It supplies the interface for neighborhood topologies of the particle swarm.
*/
class Neighborhood{
public:
	/**
	* @brief The destructor.
	*/
	virtual ~Neighborhood();
	/**
	* @brief Returns the position of the global attractor for the given particle.
	*
	* @param p The particle.
	*
	* @return The position of the global attractor for particle p.
	*/
	std::vector<mpf_t*> GetGlobalAttractorPosition(Particle* p);
	/**
	* @brief Returns the position of the global attractor for the particle with the specified particle_id.
	*
	* @param particle_id The index of the particle.
	*
	* @return The position of the global attractor for the particle with index particle_id.
	*/
	virtual std::vector<mpf_t*> GetGlobalAttractorPosition(int particle_id) = 0;
	/**
	* @brief Returns the position of the best global attractor.
	*
	* @return The position of the best global attractor.
	*/
	virtual std::vector<mpf_t*> GetGlobalAttractorPosition() = 0;
	/**
	* @brief Returns the value of the global attractor for the particle p.
	*
	* @param p The particle.
	*
	* @return The value of the global attractor for particle p.
	*/
	mpf_t* GetGlobalAttractorValue(Particle* p);
	/**
	* @brief Returns the value of the global attractor for the particle with the specified particle_id.
	*
	* @param particle_id The index of the particle.
	*
	* @return The value of the global attractor for the particle with index particle_id.
	*/
	virtual mpf_t* GetGlobalAttractorValue(int particle_id) = 0;
	/**
	* @brief Returns the value of the best global attractor.
	*
	* @return The value of the best global attractor.
	*/
	virtual mpf_t* GetGlobalAttractorValue() = 0;
	/**
	* @brief Appends the tuple of specified position, value and particle to the list of pending updates for global attractors.
	*
	* The actual update is executed by the method updateAttractorInstantly.
	* The list of pending updates will be processed depending on mode of updates (Configuration::UPDATE_GLOBAL_ATTRACTOR).
	*
	* @param position A new candidate position for a global attractor.
	* @param value The evaluated function value of this position.
	* @param p The particle, which encountered this new position.
	*/
	void UpdateAttractor(std::vector<mpf_t*> position, mpf_t* value, Particle* p);
	/**
	* @brief Appends the tuple of specified position, value and particle to the list of pending updates for global attractors.
	*
	* The actual update is executed by the method updateAttractorInstantly.
	* The list of pending updates will be processed depending on mode of updates (Configuration::UPDATE_GLOBAL_ATTRACTOR).
	*
	* @param position A new candidate position for a global attractor.
	* @param value The evaluated function value of this position.
	* @param particle_id The index of the particle, which encountered this new position.
	*/
	void UpdateAttractor(std::vector<mpf_t*> position, mpf_t* value, int particle_id);
	/**
	* @brief Depending on the neighborhood topology the new position replaces previous global attractors if the new position is better than previous positions.
	*
	* @param position A new candidate position for a global attractor.
	* @param value The evaluated function value of this position.
	* @param p The particle, which encountered this new position.
	*/
	void UpdateAttractorInstantly(std::vector<mpf_t*> position, mpf_t* value, Particle* p);
	/**
	* @brief Depending on the neighborhood topology the new position replaces previous global attractors if the new position is better than previous positions.
	*
	* @param position A new candidate position for a global attractor.
	* @param value The evaluated function value of this position.
	* @param particle_id The index of the particle, which encountered this new position.
	*/
	virtual void UpdateAttractorInstantly(std::vector<mpf_t*> position, mpf_t* value, int particle_id) = 0;
	/**
	* @brief Initializes the processing of all entries in the list of pending updates for the global attractors.
	*
	* The list of pending updates will be processed depending on mode of updates (Configuration::UPDATE_GLOBAL_ATTRACTOR).
	*/
	void ProceedAllUpdates();
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
	/**
	* @brief Loads data to reset the state of the neighborhood topology to the state which was stored.
	*
	* @param input_stream The stream which contains the data about the state of the neighborhood topology.
	* @param version_of_stored_data The version of the program when the backup was created.
	*/
	virtual void LoadData(std::ifstream* input_stream, ProgramVersion* version_of_stored_data) = 0;
	/**
	* @brief Stores the current state of this neighborhood topology such that it can completely restore the current state with this data.
	*
	* @param output_stream The output stream where the state should be stored.
	*/
	virtual void StoreData(std::ofstream* output_stream) = 0;

private:
	std::queue<std::pair< std::pair< std::vector<mpf_t*>, mpf_t*>, int> > remaining_updates_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_NEIGHBORHOOD_NEIGHBORHOOD_H_ */
