/**
* @file   specific_statistical_evaluations.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file contain classes, which can evaluate matrices depending on the current state of the particle swarm, and some helping classes to generate analyzable statistics out of them.
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

#include "statistics/specific_statistical_evaluations.h"

#include <sstream>

#include "general/check_condition.h"
#include "general/configuration.h"
#include "general/general_objects.h"
#include "general/operations.h"
#include "general/particle.h"
#include "neighborhood/neighborhood.h"
#include "statistics/statistics.h"

namespace highprecisionpso {

OperatedSpecificStatisticalEvaluation::OperatedSpecificStatisticalEvaluation(Operation* operation, SpecificStatisticalEvaluation* specificEvaluation):operation_(operation), specific_evaluation_(specificEvaluation){}

std::vector<std::vector<mpf_t*> > OperatedSpecificStatisticalEvaluation::Evaluate(){
	std::vector<std::vector<mpf_t*> > data = specific_evaluation_->Evaluate();
	for(unsigned int i = 0; i < data.size(); i++){
		for(unsigned int j = 0; j < data[i].size(); j++){
			mpf_t* result = operation_->Operate(data[i][j]);
			mpftoperations::ReleaseValue(data[i][j]);
			data[i][j] = result;
		}
	}
	return data;
}

std::string OperatedSpecificStatisticalEvaluation::GetName(){
	return operation_->GetName() + specific_evaluation_->GetName();
}

CombineSpecificStatisticalEvaluation::CombineSpecificStatisticalEvaluation(PairReduceOperation* operation, SpecificStatisticalEvaluation* operator1, SpecificStatisticalEvaluation* operator2):operation_(operation),operator1_(operator1), operator2_(operator2){}

std::vector<std::vector<mpf_t*> > CombineSpecificStatisticalEvaluation::Evaluate(){
	std::vector<std::vector<mpf_t*> > res1, res2, res;
	res1 = operator1_->Evaluate();
	res2 = operator2_->Evaluate();
	AssertCondition(res1.size() == res2.size(), "A combine statistic is forced to combine vectors of different size. Please check whether your statistic definitions are correct.");
	for(unsigned int i = 0; i < res1.size(); i++) {
		res.push_back(std::vector<mpf_t*>(0));
		AssertCondition(res1[i].size() == res2[i].size(), "A combine statistic is forced to combine vectors of different size. Please check whether your statistic definitions are correct.");
		for(unsigned int j = 0; j < res1[i].size(); j++) {
			res[i].push_back(operation_->Evaluate(res1[i][j], res2[i][j]));
		}
		vectoroperations::ReleaseValues(res1[i]);
		vectoroperations::ReleaseValues(res2[i]);
	}
	return res;
}

std::string CombineSpecificStatisticalEvaluation::GetName(){
	std::ostringstream os;
	os << operation_->GetName() << operator1_->GetName() << operator2_->GetName();
	return os.str();
}

std::vector<std::vector<mpf_t*> > PositionEvaluation::Evaluate(){
	std::vector<std::vector<mpf_t*> > data;
	for(int i = 0; i < configuration::g_particles; i++){
		data.push_back((*(configuration::g_statistics->swarm))[i]->GetPosition());
	}
	return data;
}

std::string PositionEvaluation::GetName(){
	return "Pos";
}

std::vector<std::vector<mpf_t*> > VelocityEvaluation::Evaluate(){
	std::vector<std::vector<mpf_t*> > data;
	for(int i = 0; i < configuration::g_particles; i++){
		data.push_back((*(configuration::g_statistics->swarm))[i]->GetVelocity());
	}
	return data;
}

std::string VelocityEvaluation::GetName(){
	return "Vel";
}

std::vector<std::vector<mpf_t*> > LocalAttractorEvaluation::Evaluate(){
	std::vector<std::vector<mpf_t*> > data;
	for(int i = 0; i < configuration::g_particles; i++){
		data.push_back((*(configuration::g_statistics->swarm))[i]->GetLocalAttractorPosition());
	}
	return data;
}

std::string LocalAttractorEvaluation::GetName(){
	return "LocAt";
}

std::vector<std::vector<mpf_t*> > GlobalAttractorEvaluation::Evaluate(){
	std::vector<std::vector<mpf_t*> > data;
	for(int i = 0; i < configuration::g_particles; i++){
		Particle* p = (*(configuration::g_statistics->swarm))[i];
		data.push_back(configuration::g_neighborhood->GetGlobalAttractorPosition(p));
	}
	return data;
}

std::string GlobalAttractorEvaluation::GetName(){
	return "GloAt";
}


ConstantSpecificStatisticalEvaluation::ConstantSpecificStatisticalEvaluation(ConstantEvaluation* constantEvaluation):constant_evaluation_(constantEvaluation){}

std::vector<std::vector<mpf_t*> > ConstantSpecificStatisticalEvaluation::Evaluate(){
	std::vector<std::vector<mpf_t*> > res;
	mpf_t* tmp = constant_evaluation_->Evaluate();
	for(int i = 0; i < configuration::g_particles; i++){
		res.push_back(vectoroperations::GetConstantVector(configuration::g_dimensions, tmp));
	}
	mpftoperations::ReleaseValue(tmp);
	return res;
}

std::string ConstantSpecificStatisticalEvaluation::GetName(){
	return constant_evaluation_->GetName();
}

ComposedStatisticReduceOperation::ComposedStatisticReduceOperation(VectorReduceOperation* vector_reduce_operation):vector_reduce_operation(vector_reduce_operation){}

ComposedParticleReduceOperation::ComposedParticleReduceOperation(VectorReduceOperation* vector_reduce_operation):ComposedStatisticReduceOperation(vector_reduce_operation){}

std::vector<mpf_t*> ComposedParticleReduceOperation::Evaluate(const std::vector<std::vector<mpf_t*> > & vec){
	if(vec.size() == 0)return std::vector<mpf_t*>(0);
	std::vector<std::vector<mpf_t*> > transposed;
	for(unsigned int i = 0; i < vec[0].size(); i++){
		transposed.push_back(std::vector<mpf_t*>(0));
		for(unsigned int j = 0; j < vec.size(); j++){
			AssertCondition(vec[j].size() == vec[0].size(), "A specific statistic produces vectors of different length. This might be a programming error. Contact the program author.");
			transposed[i].push_back(vec[j][i]);
		}
	}
	std::vector<mpf_t*> result;
	for(unsigned int i = 0; i < transposed.size(); i++){
		result.push_back(vector_reduce_operation->Evaluate(transposed[i]));
	}
	return result;
}

std::string ComposedParticleReduceOperation::GetName() {
	std::ostringstream os;
	os << "P" << vector_reduce_operation->GetName();
	return  os.str();
}

ComposedDimensionReduceOperation::ComposedDimensionReduceOperation(VectorReduceOperation* vector_reduce_operation):ComposedStatisticReduceOperation(vector_reduce_operation){}

std::vector<mpf_t*> ComposedDimensionReduceOperation::Evaluate(const std::vector<std::vector<mpf_t*> > & vec){
	std::vector<mpf_t*> result;
	for( unsigned int i = 0; i < vec.size(); i++ ){
		result.push_back( vector_reduce_operation->Evaluate(vec[i]) );
	}
	return result;
}

std::string ComposedDimensionReduceOperation::GetName() {
	std::ostringstream os;
	os << "D" << vector_reduce_operation->GetName();
	return  os.str();
}

} // namespace highprecisionpso
