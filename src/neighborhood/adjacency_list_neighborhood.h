/**
* @file   neighborhood/adjacency_list_neighborhood.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   March, 2015
* @brief  This file contains neighborhood topologies specified by adjacency lists.
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

#ifndef HIGH_PRECISION_PSO_NEIGHBORHOOD_ADJACENCY_LIST_NEIGHBORHOOD_H_
#define HIGH_PRECISION_PSO_NEIGHBORHOOD_ADJACENCY_LIST_NEIGHBORHOOD_H_

#include "neighborhood/neighborhood.h"

namespace highprecisionpso {

/**
* @brief This class is an abstract class, which implements a complete neighborhood topology through a neighborhood relationship specified by adjacency lists.
*/
class AdjacencyListNeighborhood: public Neighborhood {
public:
	/**
	* @brief The constructor.
	*/
	AdjacencyListNeighborhood();
	/**
	* @brief The destructor.
	*/
	~AdjacencyListNeighborhood();
	void SetGlobalAttractorPositions(std::vector<std::vector<mpf_t*> > global_attractors);
	std::vector<mpf_t*> GetGlobalAttractorPosition();
	std::vector<mpf_t*> GetGlobalAttractorPosition(int particle_id);
	mpf_t* GetGlobalAttractorValue();
	mpf_t* GetGlobalAttractorValue(int particle_id);
	void UpdateAttractorInstantly(std::vector<mpf_t*> position, mpf_t* value, int particle_id);
	virtual std::string GetName() = 0;
	void LoadData(std::ifstream* input_stream, ProgramVersion* version_of_stored_data);
	void StoreData(std::ofstream* output_stream);

protected:
	/**
	* @brief This method initializes the adjacency list.
	*
	* It is called after the number of particles is already set in the configuration section.
	*/
	virtual void InitializeAdjacencyList() = 0;
	/**
	* @brief The adjacency lists, which specify the neighborhood relationship.
	*/
	std::vector<std::vector<unsigned int> > adjacency_list;

private:
	int GetGlobalAttractorIndex();

	// index of particle, who's global attractor is 
	// equal to the attractor of the common swarm
	int global_attractor_index_;
	// global attractor visible to each particle
	std::vector<std::vector<mpf_t*> > global_attractor_positions_;
	std::vector<mpf_t*> global_attractor_values_cached_;
	std::vector<unsigned int> global_attractor_values_cached_precision_;
};

/**
* @brief This class implements the lbest neighborhood topology for the particle swarm.
*
* Each particle is connected to the next and previous l particles (according to their indices in a circular manner).
*/
class LBestNeighborhood: public AdjacencyListNeighborhood{
public:
	/**
	* @brief The constructor, which specifies the number of neighbors.
	*
	* num_neighbors should be an even number, such that for each particle the next and previous num_neighbors/2 particles are its neighbors.
	*
	* @param num_neighbors The number of neighbors for each particle.
	*/
	LBestNeighborhood(int num_neighbors);
	std::string GetName();

protected:
	void InitializeAdjacencyList();

private:
	const int num_neighbors_;
};

/**
* @brief This class implements the ring neighborhood topology for the particle swarm.
*
* Each particle is connected to the next and previous particle (according to their indices in a circular manner).
*/
class RingNeighborhood: public LBestNeighborhood{
public:
	/**
	* @brief The constructor.
	*/
	RingNeighborhood();
	std::string GetName();
};

/**
* @brief This class implements the grid or von Neumann topology for the particle swarm.
*
* Each particle is assigned to a grid position.
* The four neighbors of a particle are the particle above, the particle below, the particle to the right and the particle to the left.
*/
class GridNeighborhood: public AdjacencyListNeighborhood{
public:
	/**
	* @brief The constructor, which specifies the size of the grid.
	*
	* Make sure that the number of grid positions is equal to the number of particles.
	*
	* @param rows The number of rows in the grid.
	* @param columns The number of columns in the grid.
	*/
	GridNeighborhood(int rows, int columns);
	std::string GetName();

protected:
	void InitializeAdjacencyList();

private:
	const int grid_rows_;
	const int grid_columns_;
};

/**
* @brief This class implements the wheel topology for the particle swarm.
*
* Each particle particle is the neighbor of the first particle. No further neighborhood relations are active.
*/
class WheelNeighborhood: public AdjacencyListNeighborhood{
public:
	/**
	* @brief The constructor.
	*/
	WheelNeighborhood();
	std::string GetName();

protected:
	void InitializeAdjacencyList();
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_NEIGHBORHOOD_ADJACENCY_LIST_NEIGHBORHOOD_H_ */
