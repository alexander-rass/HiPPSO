/**
* @file   neighborhood/global_best.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   February, 2015
* @brief  This file contains the global best neighborhood topology.
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

#ifndef HIGH_PRECISION_PSO_NEIGHBORHOOD_GLOBAL_BEST_H_
#define HIGH_PRECISION_PSO_NEIGHBORHOOD_GLOBAL_BEST_H_

#include "neighborhood/neighborhood.h"

namespace highprecisionpso {

/**
* @brief This class implements the global best topology for the particle swarm, where all particles are neighbors.
*/
class GlobalBest: public Neighborhood {
public:
	/**
	* @brief The constructor.
	*/
	GlobalBest();
	/**
	* @brief The destructor.
	*/
	~GlobalBest();
	void SetGlobalAttractorPositions(std::vector<std::vector<mpf_t*> > global_attractors);
	std::vector<mpf_t*> GetGlobalAttractorPosition();
	std::vector<mpf_t*> GetGlobalAttractorPosition(int particle_id);
	mpf_t* GetGlobalAttractorValue();
	mpf_t* GetGlobalAttractorValue(int particle_id);
	void UpdateAttractorInstantly(std::vector<mpf_t*> position, mpf_t* value, int particle_id);
	std::string GetName();
	void LoadData(std::ifstream* input_stream, ProgramVersion* version_of_stored_data);
	void StoreData(std::ofstream* output_stream);

private:
	std::vector<mpf_t*> global_attractor_position_;
	mpf_t* global_attractor_value_cached_;
	unsigned int global_attractor_value_cached_precision_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_NEIGHBORHOOD_GLOBAL_BEST_H_ */
