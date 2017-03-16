/**
* @file   direct_statistics.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   September, 2015
* @brief  This file contain classes, which evaluate to vectors depending on the current state of the particle swarm.
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

#include "statistics/direct_statistics.h"

#include <sstream>

#include "general/check_condition.h"
#include "general/configuration.h"
#include "general/general_objects.h"
#include "general/operations.h"
#include "neighborhood/neighborhood.h"
#include "statistics/statistics.h"

namespace highprecisionpso {

std::string Statistic::EvaluationToString(){
	std::vector<mpf_t*> data = Evaluate();
	std::ostringstream os;
	for(unsigned int i = 0; i < data.size(); i++){
		if(i) os << " ";
		os << mpftoperations::MpftToString(data[i]);
	}
	vectoroperations::ReleaseValues(data);
	std::string res = os.str();
	return res;
}

CombineStatistic::CombineStatistic(PairCombinationOperation* operation, Statistic* operator1, Statistic* operator2):operation_(operation),operator1_(operator1), operator2_(operator2){}

std::vector<mpf_t*> CombineStatistic::Evaluate(){
	std::vector<mpf_t*> res1, res2, res;
	res1 = operator1_->Evaluate();
	res2 = operator2_->Evaluate();
	AssertCondition(res1.size() == res2.size(), "A combine statistic is forced to combine vectors of different size. Please check whether your statistic definitions are correct.");
	for(unsigned int i = 0; i < res1.size(); i++) {
		res.push_back(operation_->Evaluate(res1[i], res2[i]));
	}
	vectoroperations::ReleaseValues(res1);
	vectoroperations::ReleaseValues(res2);
	return res;
}

std::string CombineStatistic::GetName(){
	std::ostringstream os;
	os << operation_->GetName() << operator1_->GetName() << operator2_->GetName();
	return os.str();
}

StatisticMergeOperator::StatisticMergeOperator(StatisticMergeOperation* mergeOperation, SpecificStatisticalEvaluation* specificEvaluation):merge_operation_(mergeOperation), specific_evaluation_(specificEvaluation){}

std::vector<mpf_t*> StatisticMergeOperator::Evaluate(){
	std::vector<std::vector<mpf_t*> > data = specific_evaluation_->Evaluate();
	std::vector<mpf_t*> result = merge_operation_->Evaluate(data);
	for(unsigned int i = 0; i < data.size(); i++) {
		vectoroperations::ReleaseValues(data[i]);
	}
	return result;
}

std::string StatisticMergeOperator::GetName(){
	std::ostringstream os;
	os << merge_operation_->GetName() << specific_evaluation_->GetName();
	return os.str();
}

ModifiedStatistic::ModifiedStatistic(Modification* modification, Statistic* statistic):modification_(modification), statistic_(statistic){}

std::vector<mpf_t*> ModifiedStatistic::Evaluate(){
	std::vector<mpf_t*> values = statistic_->Evaluate();
	std::vector<mpf_t*> result;
	for(unsigned int i = 0; i < values.size(); i++){
		result.push_back(modification_->Modify(values[i]));
	}
	vectoroperations::ReleaseValues(values);
	return result;
}

std::string ModifiedStatistic::GetName(){
	return modification_->GetName() + statistic_->GetName();
}

NamedStatistic::NamedStatistic(std::string name, Statistic* statistic):name_(name), statistic_(statistic){}

std::vector<mpf_t*> NamedStatistic::Evaluate(){
	return statistic_->Evaluate();
}

std::string NamedStatistic::GetName(){
	return name_;
}

std::vector<mpf_t*> GlobalBestPositionStatistic::Evaluate(){
	return configuration::g_neighborhood->GetGlobalAttractorPosition();
}

std::string GlobalBestPositionStatistic::GetName(){
	return "GlBest";
}

std::vector<mpf_t*> GlobalBestPositionDistTo1DOptimumStatistic::Evaluate(){
	std::vector<mpf_t*> globBest = configuration::g_neighborhood->GetGlobalAttractorPosition();
	DistTo1DOptimumModification modification;
	std::vector<mpf_t*> res = modification.Modify(globBest);
	vectoroperations::ReleaseValues(globBest);
	return res;
}

std::string GlobalBestPositionDistTo1DOptimumStatistic::GetName(){
	return "GlBestDistTo1DOpt";
}

std::vector<mpf_t*> GlobalBestPositionFunctionEvaluationStatistic::Evaluate(){
	std::vector<mpf_t*> result;
	result.push_back(configuration::g_neighborhood->GetGlobalAttractorValue());
	return result;
}

std::string GlobalBestPositionFunctionEvaluationStatistic::GetName(){
	return "GlBestFuncValue";
}


std::vector<mpf_t*> LocalAttractorUpdatesStatistic::Evaluate() {
	std::vector<long long> vec = configuration::g_statistics->local_attractor_update_counter;
	std::vector<mpf_t*> res;
	for(unsigned int i = 0; i < vec.size(); i++ ){
		res.push_back(mpftoperations::ToMpft(vec[i]));
	}
	return res;
}

std::string LocalAttractorUpdatesStatistic::GetName() {
	return "LocAttUpd";
}

std::vector<mpf_t*> GlobalAttractorUpdatesStatistic::Evaluate() {
	std::vector<long long> vec = configuration::g_statistics->global_attractor_update_counter;
	std::vector<mpf_t*> res;
	for(unsigned int i = 0; i < vec.size(); i++ ){
		res.push_back(mpftoperations::ToMpft(vec[i]));
	}
	return res;
}

std::string GlobalAttractorUpdatesStatistic::GetName() {
	return "GloAttUpd";
}

std::vector<mpf_t*> PrecisionStatistic::Evaluate() {
	std::vector<mpf_t*> res;
	res.push_back(mpftoperations::ToMpft((int)mpf_get_default_prec()));
	return res;
}

std::string PrecisionStatistic::GetName() {
	return "Prec";
}

ConstantStatistic::ConstantStatistic(int dimensions, ConstantEvaluation* constantEvaluation):dimensions_(dimensions), constant_evaluation_(constantEvaluation){}

std::vector<mpf_t*> ConstantStatistic::Evaluate(){
	mpf_t* tmp = constant_evaluation_->Evaluate();
	std::vector<mpf_t*> res = vectoroperations::GetConstantVector(dimensions_, tmp);
	mpftoperations::ReleaseValue(tmp);
	return res;
}

std::string ConstantStatistic::GetName(){
	return constant_evaluation_->GetName();
}

} // namespace highprecisionpso
