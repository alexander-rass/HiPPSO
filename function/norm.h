/**
* @file   norm.h 
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains the description of various norm functions.
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

#ifndef HIGH_PRECISION_PSO_FUNCTION_NORM_H_
#define HIGH_PRECISION_PSO_FUNCTION_NORM_H_

#include "function/function.h"

namespace highprecisionpso {

/**
* @brief This class represents a function which calculates the Manhattan norm of a vector.
*/
class Norm1: public Function {
public:
	/**
	* @brief Returns the Manhattan norm of the given position.
	*
	* @param pos The position.
	*
	* @return \f$\sum_{d=0}^{D-1}abs(pos[d])\f$
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	/**
	* @brief Calculates for a position and a dimension the next local optimum if only the value of the position in the specified dimension is varied and returns the distance to it.
	*
	* @param pos The position.
	* @param d The index of the observed dimension.
	*
	* @return The absolute value of the position entry in dimension d.
	*/
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);
};

/**
* @brief This class represents a function which calculates the infinity norm of a vector.
*/
class NormInfinity: public Function {
public:
	/**
	* @brief Returns the infinity norm of the given position. 
	*
	* @param pos The position.
	*
	* @return \f$\max_{d\in\lbrace0,\ldots,D-1\rbrace}abs(pos[d])\f$.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	/**
	* @brief Calculates for a position and a dimension the next local optimum if only the value of the position in the specified dimension is varied and returns the distance to it.
	*
	* @param pos The position.
	* @param d The index of the observed dimension.
	*
	* @return The absolute value of the position entry in dimension d.
	*/
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);
};

/**
* @brief This class represents a function which calculates the p-norm for all p which are powers of 2.
*/
class Norm2PowerK: public Function {
public:
	/**
	* @brief The constructor, where the exponent of the power of two can be specified.
	*
	* @param exponent The exponent of the power of 2.
	*/
	Norm2PowerK(int exponent);
	/**
	* @brief Returns the \f$2^{exponent}\f$-norm of the given  position.
	*
	* @param pos The position.
	*
	* @return \f$\sum_{d=0}^{D-1}(pos[d])^{(2^{exponent})}\f$.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	/**
	* @brief Calculates for a position and a dimension the next local optimum if only the value of the position in the specified dimension is varied and returns the distance to it.
	*
	* @param pos The position.
	* @param d The index of the observed dimension.
	*
	* @return The absolute value of the position entry in dimension d.
	*/
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);

private:
	const int exponent_;
};

/**
* @brief This class represents a function, which calculates the 2-norm.
*/
class Norm2 : public Norm2PowerK {
public:
	/**
	* @brief The constructor.
	*
	* The parent constructor is called with exponent 1.
	*/
	Norm2();
};
/**
* @brief This class represents a function, which calculates the 4-norm.
*/
class Norm4 : public Norm2PowerK {
public:
	/**
	* @brief The constructor.
	*
	* The parent constructor is called with exponent 2.
	*/
	Norm4();
};
/**
* @brief This class represents a function, which calculates the 8-norm.
*/
class Norm8 : public Norm2PowerK {
public:
	/**
	* @brief The constructor.
	*
	* The parent constructor is called with exponent 3.
	*/
	Norm8();
};

/**
* @brief This class represents a function which calculates the 2-norm (which is equivalent to the sphere function).
*
* This function is a Norm2pk function with k=1.
*/
class Sphere : public Norm2PowerK {
public:
	/**
	* @brief The constructor.
	*
	* The parent constructor is called with exponent 1.
	*/
	Sphere();
};

/**
* @brief This class represents a function, which calculates the p-norm for all \f$p=1+1/2^{exponent}\f$.
*/
class Norm1Plus2PowerMinusK: public Function {
public:
	/**
	* @brief The constructor, where the exponent of the power of two can be specified.
	*
	* @param exponent The exponent of the power of 2.
	*/
	Norm1Plus2PowerMinusK(int exponent);
	/**
	* @brief Returns the \f$(1+1/2^{exponent})\f$-norm of the given  position.
	*
	* @param pos The position.
	*
	* @return \f$\sum_{d=0}^{D-1}(abs(pos[d]))^{(1+1/2^{exponent})}\f$.
	*/
	mpf_t* Eval(const std::vector<mpf_t*> & pos);
	std::string GetName();
	/**
	* @brief Calculates for a position and a dimension the next local optimum if only the value of the position in the specified dimension is varied and returns the distance to it.
	*
	* @param pos The position.
	* @param d The index of the observed dimension.
	*
	* @return The absolute value of the position entry in dimension d.
	*/
	mpf_t* DistanceTo1DLocalOptimum(const std::vector<mpf_t*> & pos, int d);

private:
	const int exponent_;
};

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_FUNCTION_NORM_H_ */
