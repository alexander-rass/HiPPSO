/**
* @file   parse.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   October, 2015
* @brief  This file contains functions for parsing the configuration file.
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

#include "general/parse.h"

#include <iostream>
#include <sstream>

#include "general/includes.h"

namespace highprecisionpso {
namespace parse {

SpecificFunction* ParseSpecificFunction(const std::vector<std::string> & parameters, unsigned int & parsed_parameters){
	// parse Operation
	{
		Operation* operation = ParseOperation(parameters, parsed_parameters);
		if(operation != NULL) {
			SpecificFunction* specificFunction = ParseSpecificFunction(parameters, parsed_parameters);
			if(specificFunction == NULL){
				parsed_parameters = parameters.size();
				return NULL;
			}
			return new OperatedSpecificFunction(operation, specificFunction);
		}
	}
	// parse combined specific Function
	{
		SpecificFunction* combinedSpecificFunction = ParseCombineSpecificFunction(parameters, parsed_parameters);
		if(combinedSpecificFunction != NULL){
			return combinedSpecificFunction;
		}
	}
	// parse direct specific function
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	unsigned int mem_parsed_parameters = parsed_parameters;
	std::string parameter = parameters[parsed_parameters++];
	if(parameter == "identity"){
		return new IdentitySpecificFunction;
	} else if(parameter == "constant"){
		ConstantEvaluation* constantEvaluation = ParseConstantEvaluation(parameters, parsed_parameters);
		if(constantEvaluation == NULL){
			parsed_parameters = parameters.size();
			return NULL;
		}
		return new ConstantSpecificFunction(constantEvaluation);
	}
	parsed_parameters = mem_parsed_parameters;
	return NULL;
}

SpecificFunction* ParseCombineSpecificFunction(const std::vector<std::string> & parameters, unsigned int & parsed_parameters){
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	unsigned int mem_parsed_parameters = parsed_parameters;
	std::string parameter = parameters[parsed_parameters++];
	if(parameter != "combine"){
		parsed_parameters = mem_parsed_parameters;
		return NULL;
	}
	PairCombinationOperation* combinationOperation = ParsePairCombinationOperation(parameters, parsed_parameters);
	SpecificFunction* operator1 = ParseSpecificFunction(parameters, parsed_parameters);
	SpecificFunction* operator2 = ParseSpecificFunction(parameters, parsed_parameters);
	if(combinationOperation == NULL || operator1 == NULL || operator2 == NULL){
		parsed_parameters = parameters.size();
		return NULL;
	}
	return new CombineSpecificFunction(combinationOperation, operator1, operator2);
}

Function* ParseFunctionMergeOperator(const std::vector<std::string> & parameters, unsigned int & parsed_parameters){
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	unsigned int mem_parsed_parameters = parsed_parameters;
	std::string parameter = parameters[parsed_parameters++];
	if(parameter != "merge"){
		parsed_parameters = mem_parsed_parameters;
		return NULL;
	}
	VectorMergeOperation* mergeOperation = ParseVectorMergeOperation(parameters, parsed_parameters);
	SpecificFunction* specificFunction = ParseSpecificFunction(parameters, parsed_parameters);
	if(mergeOperation == NULL || specificFunction == NULL){
		parsed_parameters = parameters.size();
		return NULL;
	}
	return new FunctionMergeOperator(mergeOperation, specificFunction);
}

Function* ParseStandardFunction(const std::vector<std::string> & parameters, unsigned int & parsed_parameters){
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	unsigned int mem_parsed_parameters = parsed_parameters;
	std::string parameter = parameters[parsed_parameters++];
	if(parameter != "standard"){
		parsed_parameters = mem_parsed_parameters;
		return NULL;
	}
	if(parsed_parameters == parameters.size()){
		parsed_parameters = parameters.size();
		return NULL;
	}
	std::string functionName = mpftoperations::ToLowerCase(parameters[parsed_parameters++]);
	//************************ Please add new functions here ***************************//
	if (functionName == "sphere") {
		return new Sphere;
	} else if(functionName == "norm1"){
		return new Norm1;
	} else if(functionName == "normoo"){
		return new NormInfinity;
	} else if(functionName == "norm2"){
		return new Norm2;
	} else if(functionName == "norm4"){
		return new Norm4;
	} else if(functionName == "norm8"){
		return new Norm8;
	} else if(functionName == "norm2pk"){
		if(parsed_parameters == parameters.size()){
			parsed_parameters = parameters.size();
			return NULL;
		}
		std::istringstream is(parameters[parsed_parameters++]);
		int K;
		if(!(is >> K)){
			parsed_parameters = parameters.size();
			return NULL;
		}
		return new Norm2PowerK(K);
	} else if(functionName == "norm1pl2pmk"){
		if(parsed_parameters == parameters.size()){
			parsed_parameters = parameters.size();
			return NULL;
		}
		std::istringstream is(parameters[parsed_parameters++]);
		int K;
		if(!(is >> K)){
			parsed_parameters = parameters.size();
			return NULL;
		}
		return new Norm1Plus2PowerMinusK(K);
	} else if(functionName == "randomposdef"){
		if(parsed_parameters == parameters.size()){
			parsed_parameters = parameters.size();
			return NULL;
		}
		unsigned int rng_parsed_parameters = parsed_parameters;
		RandomNumberGenerator* rng = ParseRandomNumberGenerator(parameters, parsed_parameters);
		if(rng == NULL) {
			parsed_parameters = parameters.size();
			return NULL;
		}
		auto start_subvector = parameters.begin() + rng_parsed_parameters;
		auto end_subvector = parameters.begin() + parsed_parameters;
		std::vector<std::string> rng_description(start_subvector, end_subvector);
		return new RandomPositiveDefiniteSecondDerivative(rng_description);
	} else if(functionName == "scaledsphererand"){
		if(parsed_parameters == parameters.size()){
			parsed_parameters = parameters.size();
			return NULL;
		}
		unsigned int rng_parsed_parameters = parsed_parameters;
		RandomNumberGenerator* rng = ParseRandomNumberGenerator(parameters, parsed_parameters);
		if(rng == NULL) {
			parsed_parameters = parameters.size();
			return NULL;
		}
		auto start_subvector = parameters.begin() + rng_parsed_parameters;
		auto end_subvector = parameters.begin() + parsed_parameters;
		std::vector<std::string> rng_description(start_subvector, end_subvector);
		return new ScaledSphereRand(rng_description);
	} else if(functionName == "scaledsphere"){
		return new ScaledSphereFixed();
	} else if(functionName == "scaledsphere2"){
		if(parsed_parameters == parameters.size()){
			parsed_parameters = parameters.size();
			return NULL;
		}
		std::istringstream is(parameters[parsed_parameters++]);
		double scale;
		if(!(is >> scale)){
			parsed_parameters = parameters.size();
			return NULL;
		}
		return new ScaledSphere(scale);
	} else if(functionName == "scaledandhadamardrotatedshpere2"){
		if(parsed_parameters == parameters.size()){
			parsed_parameters = parameters.size();
			return NULL;
		}
		std::istringstream is(parameters[parsed_parameters++]);
		double scale;
		if(!(is >> scale)){
			parsed_parameters = parameters.size();
			return NULL;
		}
		return new ScaledHadamardRotatedSphere(scale); 
	} else if(functionName == "scaledandhadamardrotatedshpere"){
		return new ScaledHadamardRotatedSphereFixed(); 
	} else if(functionName == "monosphere"){
		return new MonoSphere();
	} else if(functionName == "schwefel"){
		return new Schwefel1();
	} else if(functionName == "schwefel2"){
		return new Schwefel2();
	} else if(functionName == "rosenbrock"){
		return new Rosenbrock();
	} else if(functionName == "movedrosenbrock"){
		return new MovedRosenbrock();
	} else if(functionName == "rastrigin"){
		return new Rastrigin();
	} else if(functionName == "diagonal"){
		if(parsed_parameters == parameters.size()){
			parsed_parameters = parameters.size();
			return NULL;
		}
		std::istringstream is(parameters[parsed_parameters++]);
		double scale;
		if(!(is >> scale)){
			parsed_parameters = parameters.size();
			return NULL;
		}
		return new DiagonalFunction(scale);
	} else if(functionName == "sphereplus"){
		return new SpherePlus();
	} else if(functionName == "inclinedplane"){
		return new InclinedPlane();
	} else if(functionName == "inclinedplaneasym"){
		return new InclinedPlaneAsym();
	} else if(functionName == "twocupsfunction"){
		return new TwoCupsFunction();
	} else if(functionName == "sortfunction"){
		return new SortingFunction();
	} else if(functionName == "testing"){
		return new TestingFunction();
	} else if(functionName == "singledifferentdirection"){
		double single_dimension_power, remaining_dimension_power;
		{
			if(parsed_parameters == parameters.size()){
				parsed_parameters = parameters.size();
				return NULL;
			}
			std::istringstream is(parameters[parsed_parameters++]);
			if(!(is >> single_dimension_power)){
				parsed_parameters = parameters.size();
				return NULL;
			}
		}
		{
			if(parsed_parameters == parameters.size()){
				parsed_parameters = parameters.size();
				return NULL;
			}
			std::istringstream is(parameters[parsed_parameters++]);
			if(!(is >> remaining_dimension_power)){
				parsed_parameters = parameters.size();
				return NULL;
			}
		}
		if(parsed_parameters == parameters.size()){
			parsed_parameters = parameters.size();
			return NULL;
		}
		parameter = parameters[parsed_parameters++];
		if(parameter == "firstDimension"){
			return new SingleDifferentDirection(single_dimension_power, 
					remaining_dimension_power, SINGLE_DIFFERENT_DIRECTION_MODE_FIRST);
		} else if(parameter == "diagonalDirection"){
			return new SingleDifferentDirection(single_dimension_power, 
					remaining_dimension_power, SINGLE_DIFFERENT_DIRECTION_MODE_DIAGONAL);
		} else if(parameter == "randomDirection"){
			unsigned int rng_parsed_parameters = parsed_parameters;
			RandomNumberGenerator* rng = ParseRandomNumberGenerator(parameters, parsed_parameters);
			if(rng == NULL) {
				parsed_parameters = parameters.size();
				return NULL;
			}
			auto start_subvector = parameters.begin() + rng_parsed_parameters;
			auto end_subvector = parameters.begin() + parsed_parameters;
			std::vector<std::string> rng_description(start_subvector, end_subvector);
			return new SingleDifferentDirection(single_dimension_power, 
					remaining_dimension_power, SINGLE_DIFFERENT_DIRECTION_MODE_RANDOM, rng_description);
		} else {
			parsed_parameters = parameters.size();
			return NULL;
		}
	} else {
		parsed_parameters = parameters.size();
		return NULL;
	}
	parsed_parameters = parameters.size();
	return NULL;
}

Function* ParseFunction(const std::vector<std::string> & parameters, unsigned int & parsed_parameters){
	std::vector<std::string> invalidFunctionOptions = {"log2dbl"};
	Function* parsedFunction = NULL;
	unsigned int mem_parsed_parameters = parsed_parameters;
	// parse modified function
	{
		Operation* operation = ParseOperation(parameters, parsed_parameters);
		if(operation != NULL) {
			Function* function = ParseFunction(parameters, parsed_parameters);
			if(function == NULL) {
				parsed_parameters = parameters.size();
				return NULL;
			}
			parsedFunction = new OperatedFunction(operation, function);
		}
	}
	// parse standard function
	if(parsedFunction == NULL){
		Function* standardFunction = ParseStandardFunction(parameters, parsed_parameters);
		if(standardFunction != NULL){
			parsedFunction = standardFunction;
		}
	}
	// parse combined function
	if(parsedFunction == NULL){
		Function* combineFunction = ParseCombineFunction(parameters, parsed_parameters);
		if(combineFunction != NULL){
			parsedFunction = combineFunction;
		}
	}
	// parse merged function
	if(parsedFunction == NULL){
		Function* mergedFunction = ParseFunctionMergeOperator(parameters, parsed_parameters);
		if(mergedFunction != NULL){
			parsedFunction = mergedFunction;
		}
	}
	// parse constant function
	if(parsedFunction == NULL){
		if(parsed_parameters == parameters.size()){
			return NULL;
		}
		std::string parameter = parameters[parsed_parameters++];
		if(parameter != "constant"){
			parsed_parameters = mem_parsed_parameters;
			return NULL;
		}
		ConstantEvaluation* constantEvaluation = ParseConstantEvaluation(parameters, parsed_parameters);
		if(constantEvaluation == NULL){
			parsed_parameters = parameters.size();
			return NULL;
		}
		parsedFunction = new ConstantFunction(constantEvaluation);
	}
	if(parsedFunction != NULL){
		for(auto invalidFunctionOption: invalidFunctionOptions){
			for(unsigned int i = mem_parsed_parameters; i < parsed_parameters; i++){
				if(parameters[i] == invalidFunctionOption) {
					std::cerr << invalidFunctionOption << " is an invalid option for function.\n";
					parsed_parameters = parameters.size();
					return NULL;
				}
			}
		}
	}
	return parsedFunction;
}

Function* ParseCombineFunction(const std::vector<std::string> & parameters, unsigned int & parsed_parameters){
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	unsigned int mem_parsed_parameters = parsed_parameters;
	std::string parameter = parameters[parsed_parameters++];
	if(parameter != "combine"){
		parsed_parameters = mem_parsed_parameters;
		return NULL;
	}
	PairCombinationOperation* combinationOperation = ParsePairCombinationOperation(parameters, parsed_parameters);
	Function* operator1 = ParseFunction(parameters, parsed_parameters);
	Function* operator2 = ParseFunction(parameters, parsed_parameters);
	if(combinationOperation == NULL || operator1 == NULL || operator2 == NULL){
		parsed_parameters = parameters.size();
		return NULL;
	}
	return new CombineFunction(combinationOperation, operator1, operator2);
}



Operation* ParseOperation(const std::vector<std::string> & parameters, unsigned int & parsed_parameters) {
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	Operation* operation = NULL;
	unsigned int mem_parsed_parameters = parsed_parameters;
	std::string parameter = parameters[parsed_parameters++];
	if(parameter == "sqrt"){
		operation = new SqrtOperation;
	} else if(parameter == "log2"){
		operation = new Log2Operation;
	} else if(parameter == "log2dbl"){
		operation = new Log2DoubleOperation;
	} else if(parameter == "abs"){
		operation = new AbsOperation;
	} else if(parameter == "pow"){
		if(parsed_parameters == parameters.size()){
			parsed_parameters = parameters.size();
			return NULL;
		}
		std::istringstream is(parameters[parsed_parameters++]);
		double exponent;
		if(!(is >> exponent)){
			parsed_parameters = parameters.size();
			return NULL;
		}
		operation = new PowOperation(exponent);
	} else if(parameter == "exp"){
		operation = new ExpOperation;
	} else if(parameter == "sin"){
		operation = new SinOperation;
	} else if(parameter == "cos"){
		operation = new CosOperation;
	} else if(parameter == "tan"){
		operation = new TanOperation;
	} else if(parameter == "arcsin"){
		operation = new ArcsinOperation;
	} else if(parameter == "arccos"){
		operation = new ArccosOperation;
	} else if(parameter == "arctan"){
		operation = new ArctanOperation;
	} else if(parameter == "logE"){
		operation = new LogEOperation;
	}
	if(operation != NULL){
		return operation;
	}
	parsed_parameters = mem_parsed_parameters;
	return NULL;
}

PairCombinationOperation* ParsePairCombinationOperation(const std::vector<std::string> & parameters, unsigned int & parsed_parameters) {
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	std::string parameter = parameters[parsed_parameters++];
	PairCombinationOperation* combinationOperation = NULL;
	if(parameter == "+") {
		combinationOperation = new AddOperation;
	} else if(parameter == "-") {
		combinationOperation = new SubtractOperation;
	} else if(parameter == "*") {
		combinationOperation = new MultiplyOperation;
	} else if(parameter == "/") {
		combinationOperation = new DivideOperation;
	} else if(parameter == "min") {
		combinationOperation = new MinOperation;
	} else if(parameter == "max") {
		combinationOperation = new MaxOperation;
	} else {
		return NULL;
	}
	return combinationOperation;
}

ConstantEvaluation* ParseConstantEvaluation(const std::vector<std::string> & parameters, unsigned int & parsed_parameters){
	unsigned int mem_parsed_parameters = parsed_parameters;
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	std::string parameter = parameters[parsed_parameters++];
	if(parameter == "E"){
		return (new EConstantEvaluation);
	} else if(parameter == "Pi") {
		return (new PiConstantEvaluation);
	} else if(parameter == "plusInfinity") {
		return new PlusInfinityConstantEvaluation;
	} else if(parameter == "minusInfinity") {
		return new MinusInfinityConstantEvaluation;
	} else {
		std::istringstream is(parameter);
		double scale;
		if(!(is >> scale)){
			parsed_parameters = mem_parsed_parameters;
			return NULL;
		}
		return (new DoubleConstantEvaluation(scale));
	}
}

SpecificStatisticalEvaluation* ParseCombineSpecificStatistic(const std::vector<std::string> & parameters, unsigned int & parsed_parameters) {
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	std::string parameter = parameters[parsed_parameters];
	if(parameter != "combine"){
		return NULL;
	}
	parsed_parameters++;
	PairCombinationOperation* combinationOperation = ParsePairCombinationOperation(parameters, parsed_parameters);
	if(combinationOperation == NULL){
		parsed_parameters = parameters.size();
		return NULL;
	}
	SpecificStatisticalEvaluation* operator1 = ParseSpecificStatistic(parameters, parsed_parameters);
	SpecificStatisticalEvaluation* operator2 = ParseSpecificStatistic(parameters, parsed_parameters);
	if(operator1 == NULL || operator2 == NULL) {
		parsed_parameters = parameters.size();
		return NULL;
	}
	return new CombineSpecificStatisticalEvaluation(combinationOperation, operator1, operator2);
}

SpecificStatisticalEvaluation* ParseSpecificStatistic(const std::vector<std::string> & parameters, unsigned int & parsed_parameters){
	// check combine statistic
	{
		SpecificStatisticalEvaluation* statistic = ParseCombineSpecificStatistic(parameters, parsed_parameters);
		if(statistic != NULL){
			return statistic;
		}
	}
	// check operation
	{
		Operation* operation = ParseOperation(parameters, parsed_parameters);
		if(operation != NULL){
			SpecificStatisticalEvaluation* statistic = ParseSpecificStatistic(parameters, parsed_parameters);
			if(statistic == NULL){
				parsed_parameters = parameters.size();
				return NULL;
			} else {
				return new OperatedSpecificStatisticalEvaluation(operation, statistic);
			}
		}
	}
	// check direct specific statistic type
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	unsigned int mem_parsed_parameters = parsed_parameters;
	std::string parameter = parameters[parsed_parameters++];
	{
		//************************ Please add new potentials here ***************************//
		if(parameter == "position") {
			return new PositionEvaluation;
		} else if(parameter == "velocity") {
			return new VelocityEvaluation;
		} else if(parameter == "localAttractor") {
			return new LocalAttractorEvaluation;
		} else if(parameter == "globalAttractor") {
			return new GlobalAttractorEvaluation;
		} else if(parameter == "functionDifference") {
			return new PotentialFunctionDifference;
		} else if(parameter == "absVelocityPlusDistToGlobalAttractor") {
			if(parsed_parameters == parameters.size()){
				return NULL;
			}
			std::istringstream is(parameters[parsed_parameters++]);
			double scale;
			if(!(is >> scale)){
				parsed_parameters = parameters.size();
				return NULL;
			}
			return new PotentialAbsVelocityPlusDistToGlobalAttractor(scale);
		} else if(parameter == "sqrtAbsVelocityPlusSqrtDistToGlobalAttractor") {
			if(parsed_parameters == parameters.size()){
				return NULL;
			}
			std::istringstream is(parameters[parsed_parameters++]);
			double scale;
			if(!(is >> scale)){
				parsed_parameters = parameters.size();
				return NULL;
			}
			return new PotentialSqrtAbsVelocityPlusSqrtDistToGlobalAttractor(scale);
		} else if(parameter == "constant") {
			ConstantEvaluation* constantEvaluation = ParseConstantEvaluation(parameters, parsed_parameters);
			if(constantEvaluation == NULL){
				parsed_parameters = parameters.size();
				return NULL;
			}
			return new ConstantSpecificStatisticalEvaluation(constantEvaluation);
		}
	}
	parsed_parameters = mem_parsed_parameters;
	return NULL;
}

VectorMergeOperation* ParseVectorMergeOperation(const std::vector<std::string> & parameters, unsigned int & parsed_parameters) {
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	unsigned int mem_parsed_parameters = parsed_parameters;
	std::string parameter = parameters[parsed_parameters++];
	if(parameter == "specific") {
		if(parsed_parameters >= parameters.size()){
			return NULL;
		}
		std::istringstream is(parameters[parsed_parameters++]);
		int id;
		if(!(is >> id)){
			parsed_parameters = parameters.size();
			return NULL;
		}
		return new SpecificIdMergeOperation(id);
	} else if(parameter == "increasingOrderNthObject") {
		if(parsed_parameters >= parameters.size()){
			return NULL;
		}
		std::istringstream is(parameters[parsed_parameters++]);
		int order_id;
		if(!(is >> order_id)){
			parsed_parameters = parameters.size();
			return NULL;
		}
		return new IncreasingOrderNthObjectMergeOperation(order_id);
	} else if(parameter == "arithmeticAverage") {
		return new ArithmeticAverageMergeOperation;
	} else if(parameter == "geometricAverage") {
		return new GeometricAverageMergeOperation;
	} else if(parameter == "sum") {
		return new SumMergeOperation;
	} else if(parameter == "product") {
		return new ProductMergeOperation;
	} else if(parameter == "maximum") {
		return new MaximalValueMergeOperation;
	} else if(parameter == "minimum") {
		return new MinimalValueMergeOperation;
	} else if(parameter == "functionEvaluation") {
		Function* evalFunc = ParseFunction(parameters, parsed_parameters);
		if(evalFunc == NULL){
			parsed_parameters = mem_parsed_parameters;
			return NULL;
		}
		return new FunctionEvaluationMergeOperation(evalFunc);
	} else if(parameter == "objectiveFunctionEvaluation") {
		return new ObjectiveFunctionEvaluationMergeOperation;
	} else {
		parsed_parameters = mem_parsed_parameters;
		return NULL;
	}
}

Statistic* ParseMergeOperator(const std::vector<std::string> & parameters, unsigned int & parsed_parameters){
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	unsigned int mem_parsed_parameters = parsed_parameters;
	std::string parameter = parameters[parsed_parameters++];
	if(parameter != "merge") {
		parsed_parameters = mem_parsed_parameters;
		return NULL;
	}
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	parameter = parameters[parsed_parameters++];
	StatisticMergeOperation* mergeOperation = NULL;
	if(parameter == "particle") {
		VectorMergeOperation* vectorMergeOperation = ParseVectorMergeOperation(parameters, parsed_parameters);
		if(vectorMergeOperation == NULL){
			parsed_parameters = parameters.size();
			return NULL;
		}
		mergeOperation = new ComposedParticleMergeOperation(vectorMergeOperation);
	} else if(parameter == "dimension") {
		VectorMergeOperation* vectorMergeOperation = ParseVectorMergeOperation(parameters, parsed_parameters);
		if(vectorMergeOperation == NULL){
			parsed_parameters = parameters.size();
			return NULL;
		}
		mergeOperation = new ComposedDimensionMergeOperation(vectorMergeOperation);
	} else {
		parsed_parameters = parameters.size();
		return NULL;
	}
	SpecificStatisticalEvaluation* specificEvaluation = ParseSpecificStatistic(parameters, parsed_parameters);
	if(specificEvaluation == NULL) {
		parsed_parameters = parameters.size();
		return NULL;
	}
	return new StatisticMergeOperator(mergeOperation, specificEvaluation);
}

Statistic* ParseCombineStatistic(const std::vector<std::string> & parameters, unsigned int & parsed_parameters) {
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	std::string parameter = parameters[parsed_parameters];
	if(parameter != "combine"){
		return NULL;
	}
	parsed_parameters++;
	PairCombinationOperation* combinationOperation = ParsePairCombinationOperation(parameters, parsed_parameters);
	if(combinationOperation == NULL){
		parsed_parameters = parameters.size();
		return NULL;
	}
	Statistic* operator1 = ParseStatistic(parameters, parsed_parameters);
	Statistic* operator2 = ParseStatistic(parameters, parsed_parameters);
	if(operator1 == NULL || operator2 == NULL) {
		parsed_parameters = parameters.size();
		return NULL;
	}
	return new CombineStatistic(combinationOperation, operator1, operator2);
}

Statistic* ParseStatistic(const std::vector<std::string> & parameters, unsigned int & parsed_parameters){
	// check combine statistic
	{
		Statistic* statistic = ParseCombineStatistic(parameters, parsed_parameters);
		if(statistic != NULL){
			return statistic;
		}
	}
	// check operation
	{
		Operation* operation = ParseOperation(parameters, parsed_parameters);
		if(operation != NULL){
			Statistic* statistic = ParseStatistic(parameters, parsed_parameters);
			if(statistic == NULL){
				parsed_parameters = parameters.size();
				return NULL;
			} else {
				return new OperatedStatistic(operation, statistic);
			}
		}
	}
	// check merge operator
	{
		Statistic* mergeOperator = ParseMergeOperator(parameters, parsed_parameters);
		if(mergeOperator != NULL){
			return mergeOperator;
		}
	}
	if(parsed_parameters == parameters.size()){
		return NULL;
	}
	unsigned int mem_parsed_parameters = parsed_parameters;
	std::string parameter = parameters[parsed_parameters++];
	// check direct statistic type
	{
		if(parameter == "globalBestPosition"){
			return new GlobalBestPositionStatistic;
		} else if(parameter == "globalBestPositionDistanceTo1DOptimum") {
			return new GlobalBestPositionDistTo1DOptimumStatistic;
		} else if(parameter == "globalBestPositionFunctionEvaluation") {
			return new GlobalBestPositionFunctionEvaluationStatistic;
		} else if(parameter == "localAttractorUpdates"){
			return new LocalAttractorUpdatesStatistic;
		} else if(parameter == "globalAttractorUpdates"){
			return new GlobalAttractorUpdatesStatistic;
		} else if(parameter == "precision") {
			return new PrecisionStatistic;
		} else if(parameter == "constant") {
			if(parsed_parameters == parameters.size()){
				return NULL;
			}
			std::istringstream is(parameters[parsed_parameters++]);
			int dimensions;
			if(!(is >> dimensions)) {
				parsed_parameters = parameters.size();
				return NULL;
			}
			ConstantEvaluation* constantEvaluation = ParseConstantEvaluation(parameters, parsed_parameters);
			if(constantEvaluation == NULL) {
				parsed_parameters = parameters.size();
				return NULL;
			}
			return new ConstantStatistic(dimensions, constantEvaluation);
		}
	}
	parsed_parameters = mem_parsed_parameters;
	return NULL;
}

RandomNumberGenerator* ParseRandomNumberGenerator(const std::vector<std::string> & parameters, unsigned int & parsed_parameters) {
	if(parsed_parameters == parameters.size()) {
		return NULL;
	}
	unsigned int mem_parsed_parameters = parsed_parameters;
	std::string parameter = parameters[parsed_parameters++];
	if(parameter == "linearCongruenceRNG") {
		if(parsed_parameters + 2 > parameters.size()) {
			parsed_parameters = mem_parsed_parameters;
			return NULL;
		}
		// parse seed:
		unsigned long long seed;
		{
			std::istringstream is(parameters[parsed_parameters++]);
			if(!(is >> seed)) {
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
		}
		// parse <lcrng>
		std::string subparameter1 = parameters[parsed_parameters++];
		if(subparameter1 == "standard") {
			if(parsed_parameters + 1 > parameters.size()) {
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
			std::string subparameter2 = parameters[parsed_parameters++];
			if(subparameter2 == "fast") {
				return new FastM2P63LinearCongruenceRandomNumberGenerator(1571204578482947281ULL, 12345678901234567ULL, seed);
			} else if(subparameter2 == "intense") {
				if(parsed_parameters + 1 > parameters.size()) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
				int used_bits;
				{
					std::istringstream is(parameters[parsed_parameters++]);
					if(!(is >> used_bits)) {
						parsed_parameters = mem_parsed_parameters;
						return NULL;
					}
				}
				return new IntenseM2P63LinearCongruenceRandomNumberGenerator(1571204578482947281ULL, 12345678901234567ULL, seed, used_bits);
			} else {
				// invalid option;
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
		} else if(subparameter1 == "mod2p63") {
			if(parsed_parameters + 3 > parameters.size()) {
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
			unsigned long long multiplier, adder;
			{
				std::istringstream is(parameters[parsed_parameters++]);
				if(!(is >> multiplier)) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
			}
			{
				std::istringstream is(parameters[parsed_parameters++]);
				if(!(is >> adder)) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
			}
			std::string subparameter2 = parameters[parsed_parameters++];
			if(subparameter2 == "fast") {
				return new FastM2P63LinearCongruenceRandomNumberGenerator(multiplier, adder, seed);
			} else if(subparameter2 == "intense") {
				if(parsed_parameters + 1 > parameters.size()) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
				int used_bits;
				{
					std::istringstream is(parameters[parsed_parameters++]);
					if(!(is >> used_bits)) {
						parsed_parameters = mem_parsed_parameters;
						return NULL;
					}
				}
				return new IntenseM2P63LinearCongruenceRandomNumberGenerator(multiplier, adder, seed, used_bits);
			} else {
				// invalid option;
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
		} else if(subparameter1 == "specific") {
			if(parsed_parameters + 4 > parameters.size()) {
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
			unsigned long long multiplier, adder, modulus;
			{
				std::istringstream is(parameters[parsed_parameters++]);
				if(!(is >> multiplier)) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
			}
			{
				std::istringstream is(parameters[parsed_parameters++]);
				if(!(is >> adder)) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
			}
			{
				std::istringstream is(parameters[parsed_parameters++]);
				if(!(is >> modulus)) {
					parsed_parameters = mem_parsed_parameters;
					return NULL;
				}
			}
			std::string subparameter2 = parameters[parsed_parameters++];
			if(subparameter2 == "fast") {
				return new FastLinearCongruenceRandomNumberGenerator(multiplier, adder, modulus, seed);
			} else if(subparameter2 == "intense") {
				return new IntenseLinearCongruenceRandomNumberGenerator(multiplier, adder, modulus, seed);
			} else {
				// invalid option;
				parsed_parameters = mem_parsed_parameters;
				return NULL;
			}
		} else {
			// invalid option;
			parsed_parameters = mem_parsed_parameters;
			return NULL;
		}
	} else { // try to parse seed
		unsigned long long seed;
		std::istringstream is(parameter);
		if(!(is >> seed)){
			parsed_parameters = mem_parsed_parameters;
			return NULL;
		}
		return new FastM2P63LinearCongruenceRandomNumberGenerator(1571204578482947281ULL, 12345678901234567ULL, seed);
	}
}

void SignalInvalidCommand(const std::vector<std::string> & command){
	std::cerr << ("One of the specified commands in the configuration file can not be interpreted:\n\"");
	for(unsigned int i = 0; i < command.size(); i++){
		if(i != 0)std::cerr << " ";
		std::cerr << command[i];
	}
	std::cerr << ("\"\n");
	exit(1);
}

} // namespace parse
} // namespace highprecisionpso
