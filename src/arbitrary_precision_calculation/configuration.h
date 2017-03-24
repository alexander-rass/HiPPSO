/**
* @file   arbitrary_precision_calculation/configuration.h
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

#ifndef ARBITRARY_PRECISION_CALCULATION_CONFIGURATION_H_
#define ARBITRARY_PRECISION_CALCULATION_CONFIGURATION_H_

#include <string>
#include <vector>

#include "arbitrary_precision_calculation/random_number_generator.h"

/**
 * @brief base namespace for arbitrary precision calculation.
 */
namespace arbitraryprecisioncalculation {

/**
* @brief Contains the variables which store information about the configuration for the arbitrary precision calculation.
*/
namespace configuration {
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
} // namespace configuration

/**
* @brief Contains configuration of arbitrary precision calculation and supplies them by get and set methods.
*/
class Configuration{
public:

	/**
	* @brief Resets all variables in the configuration section to the default values.
	*/
	static void Init();

	/**
	 * @brief Getter function for the check precision mode.
	 *
	 * @return The current check precision mode.
	 */
	static arbitraryprecisioncalculation::configuration::CheckPrecisionMode getCheckPrecisionMode();

	/**
	 * @brief Setter function for the check precision mode. Stores the given check precision mode.
	 *
	 * @param checkPrecisionMode The new check precision mode.
	 */
	static void setCheckPrecisionMode(
			arbitraryprecisioncalculation::configuration::CheckPrecisionMode checkPrecisionMode);

	/**
	 * @brief Getter function for the check precision probability.
	 *
	 * @return The current check precision probability.
	 */
	static double getCheckPrecisionProbability();

	/**
	 * @brief Setter function for the check precision probability. Stores the given check precision probability.
	 *
	 * @param checkPrecisionProbability The new check precision probability.
	 */
	static void setCheckPrecisionProbability(double checkPrecisionProbability);

	/**
	 * @brief Status function for increasing precision.
	 *
	 * @retval TRUE if calculations recommend an increase of the precision.
	 * @retval FALSE otherwise.
	 */
	static bool isIncreasePrecisionRecommended();

	/**
	 * @brief Resets the status whether the precision should be increased to NO.
	 */
	static void ResetIncreasePrecisionRecommended();

	/**
	 * @brief Sets the status whether the precision should be increased to YES.
	 */
	static void RecommendIncreasePrecision();

	/**
	 * @brief Getter function for the initial precision.
	 *
	 * @return The currently stored initial precision.
	 */
	static int getInitialPrecision();

	/**
	 * @brief Setter function for the initial precision. Stores the given initial precision.
	 *
	 * @param initialPrecision The new initial precision.
	 */
	static void setInitialPrecision(int initialPrecision);

	/**
	 * @brief Getter function for the output precision.
	 *
	 * @return The currently stored output precision.
	 */
	static int getOutputPrecision();

	/**
	 * @brief Setter function for the output precision. Stores the given output precision.
	 *
	 * @param outputPrecision The new output precision.
	 */
	static void setOutputPrecision(int outputPrecision);

	/**
	 * @brief Getter function for the precision safety margin.
	 *
	 * @return The currently stored precision safety margin.
	 */
	static int getPrecisionSafetyMargin();

	/**
	 * @brief Setter function for the precision safety margin. Stores the given precision safety margin.
	 *
	 * @param precisionSafetyMargin The new precision safety margin.
	 */
	static void setPrecisionSafetyMargin(int precisionSafetyMargin);

	/**
	 * @brief Getter function for the standard random number generator.
	 *
	 * @return The currently stored standard random number generator.
	 */
	static RandomNumberGenerator* getStandardRandomNumberGenerator();

	/**
	 * @brief Setter function for the standard random number generator. Stores the given standard random number generator.
	 *
	 * @param standardRandomNumberGenerator The new standard random number generator.
	 */
	static void setStandardRandomNumberGenerator(
			RandomNumberGenerator* standardRandomNumberGenerator);

private:
	/**
	* @brief Specifies the initial precision of the mpf_t data type.
	*/
	static int initial_precision_;
	/**
	* @brief Remembers whether the initial precision is already set manually.
	* If this is not the case then a specification of the precision also overwrites the initial precision.
	*/
	static bool initial_precision_already_set_;
	/**
	* @brief Specifies the additional precision. If precision checks are activated then the check whether the precision should be increased uses this number to increase the calculated needed precision additionally.
	*/
	static int precision_safety_margin_;


	/**
	* @brief Stores the information in which cases precision checks occur.
	*/
	static arbitraryprecisioncalculation::configuration::CheckPrecisionMode check_precision_mode_;
	/**
	* @brief Specifies the probability whether a precision check is actually done.
	*/
	static double check_precision_probability_;
	/**
	* @brief Specifies whether the precision will be increased shortly.
	*/
	static bool increase_precision_recommended_;

	/**
	* @brief Specifies the number of digits for the output of mpf_t values while using printing functions.
	* The value -1 specifies that the output precision should match the current precision of the mpf_t data type.
	*/
	static int output_precision_;

	/**
	* @brief Specifies the random number generator for the particle swarm optimization algorithm.
	*/
	static RandomNumberGenerator* standard_random_number_generator_;


}; // class Configuration

} // namespace arbitraryprecisioncalculation

#endif /* ARBITRARY_PRECISION_CALCULATION_CONFIGURATION_H_ */
