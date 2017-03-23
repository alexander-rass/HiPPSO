/**
* @file   function/function.h
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains abstract classes for function generation.
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_FUNCTION_H_
#define HIGH_PRECISION_PSO_FUNCTION_FUNCTION_H_

#include <gmp.h>
#include <vector>

namespace highprecisionpso {

/**
* @brief This class can store information about where the bounds of the objective function are.
*/
class SearchSpaceBound {
public:
	/**
	* @brief Lower bound of the search space.
	*/
	double lower_search_space_bound;
	/**
	* @brief Upper bound of the search space.
	*/
	double upper_search_space_bound;
	/**
	* @brief Lowest affected dimension id.
	*/
	int dimension_from;
	/**
	* @brief Highest affected dimension id.
	*/
	int dimension_to;
};

/**
* @brief This class is an abstract class. It supplies the interface for objective functions.
*/
class Function{
public:
	/**
	* @brief The destructor.
	*/
	virtual ~Function(){};
	/**
	* @brief A constructor.
	*/
	Function();
	/**
	* @brief A constructor, specifying the lower and upper search space bounds.
	*
	* @param search_space_lower_bound The lower search space bound for each dimension.
	* @param search_space_upper_bound The upper search space bound for each dimension.
	*/
	Function(double search_space_lower_bound, double search_space_upper_bound);
	/**
	* @brief A constructor specifying the lower and upper search space bounds.
	*
	* @param search_space_lower_bound The lower search space bound.
	* @param search_space_upper_bound The upper search space bound.
	*/
	Function(std::vector<double> search_space_lower_bound, std::vector<double> search_space_upper_bound);
	/**
	* @brief This is the initial function for evaluation of a position.
	*
	* Here the cases are handled where the position lies outside of the search space bounds.
	*
	* @warning Do not override this method. If you write your own function override the function eval!
	*
	* @param pos The position which should be evaluated.
	*
	* @return The function value of the position.
	*/
	mpf_t* Evaluate(const std::vector<mpf_t*> & pos);
	/**
	* @brief Returns the lower search space bound of the search space for this objective function.
	*
	* @return The lower bound.
	*/
	virtual std::vector<mpf_t*> GetLowerSearchSpaceBound();
	/**
	* @brief Returns the upper search space bound of the search space for this objective function.
	*
	* @return The upper bound.
	*/
	virtual std::vector<mpf_t*> GetUpperSearchSpaceBound();
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
	/**
	* @brief Calculates for a position and a dimension the next local optimum if only the value of the position in the specified dimension is varied and returns the distance to it.
	*
	* For a given position all values (beside the value in the specified dimension) are regarded as constants.
	* By a ternary search, where the value of the position in the specified dimension is varied, a local optimum is calculated.
	* The result is the distance of the evaluated position and the found local optimum.
	*
	* @param pos The position.
	* @param d The index of the observed dimension.
	*
	* @return The distance to the next local optimum in a single dimensional setting.
	*/
	virtual mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);
	/**
	* @brief Returns the evaluation of the current function at the given position.
	*
	* If you write your own function please override this function.
	*
	* @warning The cases where the position appears to be outside of the search space bounds is handled already in the evaluate function.
	*
	* @param pos The position.
	*
	* @return The function value of the position.
	*/
	virtual mpf_t* Eval(const std::vector<mpf_t*> & pos) = 0;
	/**
	* @brief Calculates for a position and a dimension the next local optimum if only the value of the position in the specified dimension is varied and returns the distance to it.
	*
	* For a given position all values (beside the value in the specified dimension) are regarded as constants.
	* By a ternary search, where the value of the position in the specified dimension is varied, a local optimum is calculated.
	* The result is the distance of the evaluated position and the found local optimum.
	*
	* @param pos The position.
	* @param d The index of the observed dimension.
	* @param start_distance The initial step length for the ternary search.
	*
	* @return The distance to the next local optimum in a single dimensional setting.
	*/
	virtual mpf_t* DistanceTo1DLocalOptimumTernarySearch(const std::vector<mpf_t*> & pos, int d, mpf_t* start_distance);
	/**
	 * @brief Updates the bound of the search space according to the given data.
	 *
	 * @param search_space_bound The information about the updated search space bounds.
	 */
	void SetFunctionBound(SearchSpaceBound* search_space_bound);
	/**
	 * @brief Initializes the search space bounds.
	 */
	void InitSearchSpaceBounds();
private:
	/**
	* @brief The double values of the lower search space bound.
	*/
	std::vector<double> search_space_lower_bound_;
	/**
	* @brief The double values of the upper search space bound.
	*/
	std::vector<double> search_space_upper_bound_;
};

/**
* @brief This class is an abstract class. It supplies an interface for operations on vectors.
*/
class SpecificFunction {
public:
	/**
	* @brief The destructor.
	*/
	virtual ~SpecificFunction(){};
	/**
	* @brief Returns the evaluation of the current specific function at the given position.
	*
	* @param pos The position.
	*
	* @return The vector of function values of the position.
	*/
	virtual std::vector<mpf_t*> Eval(const std::vector<mpf_t*> & pos) = 0;
	/**
	* @brief Returns a string representation of this object.
	*
	* @return The name of the object.
	*/
	virtual std::string GetName() = 0;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_FUNCTION_H_ */
