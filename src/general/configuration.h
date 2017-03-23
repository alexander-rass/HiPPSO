/**
* @file   configuration.h 
* @author Alexander Raß (alexander.rass@fau.de)
* @date   June, 2013
* @brief  This file contains general information about the configuration for running the particle swarm optimization algorithm.
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

#ifndef HIGH_PRECISION_PSO_GENERAL_CONFIGURATION_H_
#define HIGH_PRECISION_PSO_GENERAL_CONFIGURATION_H_

#include <string>
#include <vector>

#include "bound_handling/bound_handling.h"
#include "function/function.h"
#include "neighborhood/neighborhood.h"
#include "position_and_velocity_updater/position_and_velocity_updater.h"
#include "arbitrary_precision_calculation/random_number_generator.h"
#include "statistics/direct_statistics.h"
#include "statistics/statistics.h"
#include "velocity_adjustment/velocity_adjustment.h"

/**
 * @brief base namespace for this project.
 */
namespace highprecisionpso {

/**
* @brief Contains the variables which store information about the configuration for the particle swarm optimization algorithm (PSO).
*/
namespace configuration {

/**
* @brief Inertia weight of PSO for movement equation.
*/
extern double g_chi;
/**
* @brief Coefficient for the weight of the direction to the local attractor for the movement equation.
*/
extern double g_coefficient_local_attractor;
/**
* @brief Coefficient for the weight of the direction to the global attractor for the movement equation.
*/
extern double g_coefficient_global_attractor;

/**
* @brief Specification possibilities how the velocity is initialized.
*/
enum InitializeVelocityMode {
	/**
	* @brief The velocity is initialized with a zero vector.
	*/
	INITIALIZE_VELOCITY_MODE_ZERO,
	/**
	* @brief The velocity is initialized by sampling a second position and taking the half difference vector from the position to this second position.
	*/
	INITIALIZE_VELOCITY_MODE_HALFDIFF,
	/**
	* @brief The velocity is initialized randomly according to the diameter of the search space.
	*/
	INITIALIZE_VELOCITY_MODE_RANDOM
};
/**
* @brief Stores the information how the velocity should be initialized.
*/
extern InitializeVelocityMode g_initialize_velocity_mode;

/**
* @brief Specification possibilities how often the global attractor is updated.
*/
enum UpdateGlobalAttractorMode {
	/**
	* @brief The global attractor is updated after each iteration is completed.
	*/
	UPDATE_GLOBAL_ATTRACTOR_MODE_EACH_ITERATION,
	/**
	* @brief The global attractor is updated as soon as a better position appears.
	*
	* I.e. if a particle finds a better position than the global attractor then the next particle directly works with this new position as global attractor.
	*/
	UPDATE_GLOBAL_ATTRACTOR_MODE_EACH_PARTICLE
};
/**
* @brief Stores the information how often the global attractor is updated
*/
extern UpdateGlobalAttractorMode g_update_global_attractor_mode;

/**
* @brief The number of particles for the particle swarm optimization algorithm.
*/
extern int g_particles;
/**
* @brief The number of dimensions of the search space for the particle swarm optimization algorithm.
*/
extern int g_dimensions;
/**
* @brief The number of iterations / steps for the particle swarm optimization algorithm.
* Specifies how many iterations of the particle swarm are done.
*/
extern long long g_max_iterations;
/**
* @brief Specifies the initial precision of the mpf_t data type.
*/
extern int g_initial_precision;
/**
* @brief Remembers whether the initial precision is already set manually.
* If this is not the case then a specification of the precision also overwrites the initial precision.
*/
extern bool g_initial_precision_already_set;
/**
* @brief Specifies the additional precision. If precision checks are activated then the check whether the precision should be increased uses this number to increase the calculated needed precision additionally.
*/
extern int g_precision_safety_margin;

/**
* @brief Specification possibilities in which cases precision checks occur.
*/
enum CheckPrecisionMode {
	/**
	* @brief If there are statistical calculations then no precision is checked. Beside that the precision is checked on each addition and subtraction.
	*/
	CHECK_PRECISION_ALWAYS_EXCEPT_STATISTICS,
	/**
	* @brief The precision is checked on each addition and subtraction.
	*/
	CHECK_PRECISION_ALWAYS,
	/**
	* @brief The precision is never checked.
	*/
	CHECK_PRECISION_NEVER
};
/**
* @brief Stores the information in which cases precision checks occur.
*/
extern CheckPrecisionMode g_check_precision_mode;
/**
* @brief Specifies the probability whether a precision check is actually done.
*/
extern double g_check_precision_probability;
/**
* @brief Specifies whether the precision will be increased shortly.
*/
extern bool g_increase_precision;

/**
* @brief Specifies the number of digits for the output of mpf_t values while using printing functions.
* The value -1 specifies that the output precision should match the current precision of the mpf_t data type.
*/
extern int g_output_precision;

/**
* @brief Specifies at which steps / iterations statistical evaluations are done.
* Each vector entry contains 3 long long values which specify start iteration, end iteration and periodicity.
*/
extern std::vector<std::vector<long long> > g_iterations_with_statistic_evaluations;
/**
* @brief Contains a list of statistics which should be evaluated.
*/
extern std::vector<Statistic*> g_statistics_list;
/**
* @brief Specification possibilities how the objective function is evaluated outside of the search space bounds.
*/
enum FunctionBehaviorOutsideOfBoundsMode {
	/**
	* @brief If the function is evaluated outside of the search space bounds always +infinity is returned.
	*/
	FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_INFINITY,
	/**
	* @brief If the function is evaluated outside of the search space bounds also the specified function is evaluated.
	*/
	FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_NORMAL_EVALUATION,
	/**
	* @brief If the function is evaluated outside of the search space bounds, the function is repeated periodically and then evaluated.
	*
	* If the search space bounds are [1,10]x[2,20] and it should be evaluated at (40,-10) then it will be evaluated at (4,8).
	*/
	FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_PERIODIC
};
/**
* @brief Stores the information how the objective function is evaluated outside of the search space bounds.
*/
extern FunctionBehaviorOutsideOfBoundsMode g_function_behavior_outside_of_bounds_mode;

/**
* @brief Contains a list of time steps at which backups of the current situation (positions, velocities, ... ) are stored.
*/
extern std::vector<long long> g_preserve_backup_times;
/**
* @brief Specifies periodicity in seconds of the creation of backups and the continuation of statistics.
*/
extern double g_time_between_backups;
/**
* @brief Specifies periodicity in seconds of checking whether the program is allowed to run or not.
* (see run check configuration in configuration files)
*/
extern double g_time_between_run_checks;
/**
* @brief File name of the run check configuration file. If this value is the empty string then no run check is done.
*/
extern std::string g_run_check_configuration_file;

/**
* @brief File prefix for all generated files. (Will be initialized in the first call of getFilePrefix)
*/
extern std::string g_file_prefix;
/**
* @brief User defined part of the file prefix. It is empty if the user has not specified it.
*/
extern std::string g_user_defined_file_prefix;
/**
* @brief Specifies whether the time and the date should be included in the generated files.
*/
extern bool g_show_system_time_in_file_prefix;

/**
* @brief Specifies the objective function.
*/
extern Function* g_function;
/**
* @brief Specifies the bound handling method.
*/
extern BoundHandling* g_bound_handling;
/**
* @brief Specifies the method for velocity adjustment in case that the bound handling method was active.
*/
extern VelocityAdjustment* g_velocity_adjustment;
/**
* @brief Specifies the way how the positions and the velocities are updated at each iteration.
*/
extern PositionAndVelocityUpdater* g_position_and_velocity_updater;
/**
* @brief Specifies the random number generator for the particle swarm optimization algorithm.
*/
extern RandomNumberGenerator* g_standard_random_number_generator;
/**
* @brief Specifies the neighborhood topology.
*/
extern Neighborhood* g_neighborhood;
/**
* @brief Stores information about the current swarm situation.
*/
extern Statistics* g_statistics;

/**
* @brief Specifies whether the debug swarm mode is activated. 
*/
extern bool g_debug_swarm_activated;
/**
* @brief Specifies width in pixels of generated images.
*/
extern unsigned int g_debug_swarm_x_resolution;
/**
* @brief Specifies height in pixels of generated images.
*/
extern unsigned int g_debug_swarm_y_resolution;
/**
* @brief Specifies how many of the previous global attractor positions are visualized.
*/
extern unsigned int g_debug_swarm_global_attractor_trajectory;
/**
* @brief Specifies how often images are created.
*/
extern unsigned int g_debug_swarm_image_generation_frequency;
/**
* @brief Specifies whether the position value and the function value of the particle swarm should be displayed at each iteration.
*/
extern bool g_debug_swarm_show_global_attractor_on_terminal;
/**
* @brief Specifies the gnuplot terminal for image generation.
*/
extern std::string g_debug_swarm_gnuplot_terminal;
/**
* @brief Specifies the file extension for image generation with gnuplot.
*/
extern std::string g_debug_swarm_image_file_extension;

/**
* @brief Specification possibilities for position or velocity initialization.
*/
enum InitializationInformationType {
		/**
		* @brief InitializationInformation with this type contains an upper bound and a lower bound as well as the related dimension and particle range.
		*/
		INITIALIZATION_INFORMATION_TYPE_BOUNDS,
		/**
		* @brief InitializationInformation with this type contains a center and a range as well as the related dimension and particle range.
		*/
		INITIALIZATION_INFORMATION_TYPE_CENTER_RANGE,
		/**
		* @brief InitializationInformation with this type contains an upper bound and a lower bound for the random center and a range as well as the related dimension and particle range.
		*/
		INITIALIZATION_INFORMATION_TYPE_RANDOM_CENTER_RANGE,
		/**
		* @brief InitializationInformation with this type contains factor which scales the range of previously specified intervals as well as the related dimension and particle range.
		*/
		INITIALIZATION_INFORMATION_TYPE_NORMAL_SCALE,
		/**
		* @brief InitializationInformation with this type contains an exponential factor which scales the range of previously specified intervals as well as the related dimension and particle range.
		*/
		INITIALIZATION_INFORMATION_TYPE_POWER_SCALE,
};
/**
* @brief Stores the information how positions or velocities should be initialized.
*/
class InitializationInformation {
public:
	/**
	* @brief Specifies the current type of information about initialization.
	*/
	InitializationInformationType information_type;
	/**
	* @brief Lower bound for initialization. May be unused depending on information type.
	*/
	double lower_bound;
	/**
	* @brief Upper bound for initialization. May be unused depending on information type.
	*/
	double upper_bound;
	/**
	* @brief Range for initialization. May be unused depending on information type.
	*/
	double range;
	/**
	* @brief Exponential scale (with base two) for initialization. May be unused depending on information type.
	*/
	int power_scale;
	/**
	* @brief Scale for initialization. May be unused depending on information type.
	*/
	double normal_scale;
	/**
	* @brief Lowest particle id which is affected by current initialization information.
	*/
	int particle_from;
	/**
	* @brief Highest particle id which is affected by current initialization information.
	*/
	int particle_to;
	/**
	* @brief Lowest dimension id which is affected by current initialization information.
	*/
	int dimension_from;
	/**
	* @brief Highest dimension id which is affected by current initialization information.
	*/
	int dimension_to;
};
/**
* @brief Initialization informations for position initialization.
*/
extern std::vector<InitializationInformation> g_position_initialization_informations;
/**
* @brief Initialization informations for velocity initialization.
*/
extern std::vector<InitializationInformation> g_velocity_initialization_informations;


/**
* @brief A list of search space bounds for the current objective function.
*/
extern std::vector<SearchSpaceBound> g_function_bounds;

/**
* @brief Reads data from the given configuration file and stores them in the configuration section.
*
* @param fileName The file name of the configuration file.
*
* @return true if reading the file was successful and false otherwise.
*/
bool ReadConfigurationFile(std::string fileName);

/**
* @brief Generates a short string, which includes the main information about the current configuration.
*
* @return The generated configuration string.
*/
std::string GetConfigurationString();

/**
* @brief If a file prefix is already specified then this file prefix is returned. Otherwise the file prefix is generated.
*
* @return The respective file prefix.
*/
std::string GetFilePrefix();

/**
* @brief Resets all variables in the configuration section to the default values.
*/
void Init();

} // namespace configuration
} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_GENERAL_CONFIGURATION_H_ */
