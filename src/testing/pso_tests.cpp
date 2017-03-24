/**
* @file   testing/pso_tests.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   March, 2015
* @brief  This file contains methods for checking correct functionality of the PSO software.
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

#include <algorithm>
#include <assert.h>
#include <cfloat>
#include <cmath>
#include <gmp.h>
#include <iostream>
#include <string>
#include <vector>

#include "general/includes.h"

namespace highprecisionpso {

const double EPS = 1e-6;
const double PI = acos(-1.0);
const int RANDOM_NUMS = 1000;
const int BASE_PRECISION = 256;

unsigned int largePrecision;
unsigned int smallPrecision;

void init(){
	mpf_set_default_prec(BASE_PRECISION);
	smallPrecision = mpf_get_default_prec();
	mpf_set_default_prec(smallPrecision * 2);
	largePrecision = mpf_get_default_prec();
}


double maximalError = 0.0;

mpf_t* getError(mpf_t* pv1, mpf_t* pv2){
	mpf_t* v1 = arbitraryprecisioncalculation::mpftoperations::Clone(pv1);
	mpf_t* v2 = arbitraryprecisioncalculation::mpftoperations::Clone(pv2);
	mpf_t* a1 = arbitraryprecisioncalculation::mpftoperations::Abs(v1);
	mpf_t* a2 = arbitraryprecisioncalculation::mpftoperations::Abs(v2);
	mpf_t* ma = arbitraryprecisioncalculation::mpftoperations::Max(a1, a2);
	if(arbitraryprecisioncalculation::mpftoperations::Compare(ma, 1.0) > 0){
		mpf_t* tmp = arbitraryprecisioncalculation::mpftoperations::Divide(v1, ma);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v1);
		v1 =  tmp;
		tmp = arbitraryprecisioncalculation::mpftoperations::Divide(v2, ma);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v2);
		v2 = tmp;
	}
	mpf_t* dif = arbitraryprecisioncalculation::mpftoperations::Subtract(v1, v2);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v1);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v2);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(a1);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(a2);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(ma);
	mpf_t* err = arbitraryprecisioncalculation::mpftoperations::Abs(dif);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(dif);
	return err;
}

bool smallError(mpf_t* pv1, double d2){
	if(!(std::abs(d2) < DBL_MAX))return true;
	mpf_t* v2 = arbitraryprecisioncalculation::mpftoperations::ToMpft(d2);
	mpf_t* merr = getError(pv1, v2);
	double err = arbitraryprecisioncalculation::mpftoperations::MpftToDouble(merr);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(v2);
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(merr);
	maximalError = std::max(maximalError, err);
	if(err >= EPS){
		std::cout << "error: " << err << std::endl;
	}
	return err < EPS;
}

double getMaximalError(){
	double res = maximalError;
	maximalError = 0.0;
	return res;
}

int testFunction(Function* func, double (*evalFunc)(std::vector<double>), double loValue, double centerValue, double hiValue){
	int mpf_t_used = arbitraryprecisioncalculation::mpftoperations::GetNumberOfMpftValuesInUse() - arbitraryprecisioncalculation::mpftoperations::GetNumberOfMpftValuesCached();
	std::cout << "start function <" << func->GetName() << "> test\n";
	mpf_set_default_prec(BASE_PRECISION);
	std::vector<std::vector<mpf_t*> > checkNums;
	std::vector<double> expectedResults;
	for(int i = 0; i <  RANDOM_NUMS; i++){
		int D = (rand() % 10) + 1;
		if(i == 0)D = 10;
		std::vector<double> position;
		std::vector<mpf_t*> mPosition;
		for(int d = 0; d < D; d++){
			double v = rand() / (double) RAND_MAX;
			double cen = centerValue, dif;
			if(rand() & 1){
				dif = hiValue - centerValue;
			} else {
				dif = loValue - centerValue;
			}
			for(int j = 0; j < 20; j++){
				dif *= 0.5;
				if((rand() & 7) == 7){
					cen += dif;
				}
			}
			double x = cen + dif * v;
			position.push_back(x);
			mPosition.push_back(arbitraryprecisioncalculation::mpftoperations::ToMpft(x));
		}
		checkNums.push_back(mPosition);
		if(evalFunc != NULL){
			expectedResults.push_back((*evalFunc)(position));
		}
	}
	std::vector<mpf_t*> actualResults;
	std::vector<mpf_t*> moreAccurateResults;
	mpf_set_default_prec(smallPrecision);
	{
		int result_should_be_true = (mpf_get_default_prec() == smallPrecision);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	for(unsigned int i = 0; i < checkNums.size(); i++){
		configuration::g_dimensions = checkNums[i].size();
		actualResults.push_back(func->Evaluate(checkNums[i]));
	}
	mpf_set_default_prec(largePrecision);
	{
		int result_should_be_true = (mpf_get_default_prec() == largePrecision);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}

	// check free of mpfts with functions distance_to_1D_local_optimum, getLower, getUpper:
	{
		configuration::g_dimensions = 10;
		std::vector<mpf_t*> tmp = func->GetLowerSearchSpaceBound();
		{
			int result_should_be_true = (tmp.size() == (unsigned int) configuration::g_dimensions);
			assert(result_should_be_true);
			if(!(result_should_be_true)) return 1;
		}
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(tmp);
		tmp = func->GetUpperSearchSpaceBound();
		{
			int result_should_be_true = (tmp.size() == (unsigned int) configuration::g_dimensions);
			assert(result_should_be_true);
			if(!(result_should_be_true)) return 1;
		}
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(tmp);
	}
	{
		{
			int result_should_be_true = (checkNums.size() > 0);
			assert(result_should_be_true);
			if(!(result_should_be_true)) return 1;
		}
		{
			int result_should_be_true = (checkNums[0].size() == 10);
			assert(result_should_be_true);
			if(!(result_should_be_true)) return 1;
		}
		mpf_t* tmp = func->DistanceTo1DLocalOptimum(checkNums[0], 3);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp);
		tmp = func->DistanceTo1DLocalOptimum(checkNums[0], 0);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp);
		tmp = func->DistanceTo1DLocalOptimum(checkNums[0], 9);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(tmp);
	}

	for(unsigned int i = 0; i < checkNums.size(); i++){
		configuration::g_dimensions = checkNums[i].size();
		moreAccurateResults.push_back(func->Evaluate(checkNums[i]));
	}
	if(arbitraryprecisioncalculation::Configuration::isIncreasePrecisionRecommended()){
		arbitraryprecisioncalculation::Configuration::ResetIncreasePrecisionRecommended();
		std::cout << "evaluation leads to an increase of precision\n";
	}
	if(evalFunc != NULL){
		{
			int result_should_be_true = (expectedResults.size() == checkNums.size());
			assert(result_should_be_true);
			if(!(result_should_be_true)) return 1;
		}
	}
	{
		int result_should_be_true = (checkNums.size() == actualResults.size());
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	{
		int result_should_be_true = (checkNums.size() == moreAccurateResults.size());
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	mpf_set_default_prec(largePrecision);
	{
		int result_should_be_true = (mpf_get_default_prec() == largePrecision);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	if(evalFunc != NULL){
		for(unsigned int i = 0; i < checkNums.size(); i++){
			if(!smallError(actualResults[i], expectedResults[i])){
				std::cerr << "WARNING: Error is not small but " << getMaximalError() << " for " << i << "-th position";
				std::cerr << std::endl;
				std::cerr << "pos: [";
				for(auto x : checkNums[i]) {
					std::cerr << " ";
					std::cerr << arbitraryprecisioncalculation::mpftoperations::MpftToString(x);
				}
				std::cerr << " ]\n";
				std::cerr << "  actual result: " << arbitraryprecisioncalculation::mpftoperations::MpftToDouble(actualResults[i]) << std::endl;
				std::cerr << "expected result:" << expectedResults[i] << std::endl;
				assert(false);
				return 1;
			}
		}
		std::cout << "double precision tests passed - ";
		std::cout << "maximal error = " << getMaximalError() << std::endl;
	}
	mpf_t* maxErr = arbitraryprecisioncalculation::mpftoperations::ToMpft(0.0);

	for(unsigned int i = 0; i < checkNums.size(); i++){
		mpf_t* res1 = actualResults[i];
		mpf_t* res2 = moreAccurateResults[i];
		if(arbitraryprecisioncalculation::mpftoperations::Compare(res1, res2) == 0)continue;
		mpf_t* err = getError(res1, res2);
		if(arbitraryprecisioncalculation::mpftoperations::Compare(maxErr, err) < 0){
			std::swap(maxErr, err);
		}
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(err);
	}
	unsigned int minAccuracy = 0;
	while(arbitraryprecisioncalculation::mpftoperations::Compare(maxErr, 1.0) < 0 && minAccuracy < largePrecision){
		minAccuracy++;
		mpf_t* tmp = arbitraryprecisioncalculation::mpftoperations::Add(maxErr, maxErr);
		arbitraryprecisioncalculation::mpftoperations::ReleaseValue(maxErr);
		maxErr = tmp;
	}
	arbitraryprecisioncalculation::mpftoperations::ReleaseValue(maxErr);
	std::cout << "accuracy: min(rel error, abs error):" << minAccuracy << "/" << smallPrecision
		<< " = " << (100.0 * minAccuracy / (double) smallPrecision) << "%\n";
	if(minAccuracy < 0.5 * smallPrecision){
		std::cout << "WARNING: evaluation has bad precision!\n";
	}
	{
		int result_should_be_true = (minAccuracy > 0.2 * smallPrecision);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	for(unsigned int i = 0; i < checkNums.size(); i++){
		arbitraryprecisioncalculation::vectoroperations::ReleaseValues(checkNums[i]);
	}
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(actualResults);
	arbitraryprecisioncalculation::vectoroperations::ReleaseValues(moreAccurateResults);
	if(arbitraryprecisioncalculation::mpftoperations::GetNumberOfMpftValuesInUse() != mpf_t_used + arbitraryprecisioncalculation::mpftoperations::GetNumberOfMpftValuesCached()){
		std::cerr << std::string(65, '!') << std::endl;
		std::cerr << "mpf_t's in use inconsistent. Should be " << mpf_t_used << " but there are " << arbitraryprecisioncalculation::mpftoperations::GetNumberOfMpftValuesInUse() - arbitraryprecisioncalculation::mpftoperations::GetNumberOfMpftValuesCached() << std::endl;
		std::cerr << std::string(65, '!') << std::endl;
		return 1;
	}
	arbitraryprecisioncalculation::Configuration::ResetIncreasePrecisionRecommended();
	return 0;
}

double evalSphere(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += x[d] * x[d];
	}
	return res;
}

double evalNorm1(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += std::abs(x[d]);
	}
	return res;
}

double evalNormOO(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res = std::max(res, std::abs(x[d]));
	}
	return res;
}


double evalNorm4(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += std::pow(x[d], 4);
	}
	return res;
}

double evalNorm8(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += std::pow(x[d], 8);
	}
	return res;
}

double evalNorm2p4(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += std::pow(x[d], 1<<4);
	}
	return res;
}

double evalNorm1pl2pm1(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += std::pow(std::abs(x[d]), 1.5);
	}
	return res;
}

double evalNorm1pl2pm2(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += std::pow(std::abs(x[d]), 1.25);
	}
	return res;
}

double evalScaledSphere1e6(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += x[d] * x[d] * std::pow(1e6, d / (std::max(1.0, x.size() - 1.0)));
	}
	return res;
}

double evalScaledSphere2517e42(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += x[d] * x[d] * std::pow(2517e42, d / (std::max(1.0, x.size() - 1.0)));
	}
	return res;
}

double evalScaledHadamardRotatedSphere1e6(std::vector<double> x){
	double res = 0;
	std::vector<std::vector<bool> > A;
	A.push_back(std::vector<bool>(1,true));
	while(A.size() < x.size()){
		int S = A.size();
		for(int i = 0; i < S; i++){
			A.push_back(A[i]);
			for(int j = 0; j < S; j++){
				A[i].push_back(A[i][j]);
				A[i+S].push_back(!A[i][j]);
			}
		}
	}
	for(unsigned int d = 0; d < x.size(); d++){
		double v = 0;
		for(unsigned int d2 = 0; d2 < x.size(); d2++){
			if(A[d][d2])v+=x[d2];
			else v-=x[d2];
		}
		res += v * v * std::pow(1e6, d / (std::max(1.0, x.size() - 1.0)));
	}
	return res;
}

double evalSchwefel(std::vector<double> x){
	double res = 0;
	double sumx = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		sumx += x[d];
		res += sumx * sumx;
	}
	return res;
}

double evalSchwefel2(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += -x[d] * sin(sqrt(std::abs(x[d])));
	}
	return res;
}

double evalRosenbrock(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size() - 1; d++){
		res += 100.0 * std::pow(x[d+1]-x[d]*x[d],2) + std::pow(1.0-x[d],2);
	}
	return res;
}

double evalRastrigin(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += 10.0 + std::pow(x[d],2) - 10 * cos(2.0 * PI * x[d]);
	}
	return res;
}

double evalDiagonal1e6(std::vector<double> x){
	double res = 0;
	double sumx = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += std::pow(x[d], 2);
		sumx += x[d];
	}
	res += 1e6 * sumx * sumx;
	return res;
}

double evalDiagonal2517e42(std::vector<double> x){
	double res = 0;
	double sumx = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res += std::pow(x[d], 2);
		sumx += x[d];
	}
	res += 2517e42 * sumx * sumx;
	return res;
}

double evalInclinedPlane(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res -= x[d];
	}
	return res;
}

double evalInclinedPlaneAsym(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		res -= (d+1) * x[d];
	}
	return res;
}

double evalTwoCupsFunction(std::vector<double> x){
	double res = 0;
	for(unsigned int d = 0; d < x.size(); d++){
		double xx = x[d];
		if(xx < 0)res += std::pow(xx + 1, 2);
		else res += std::pow(xx -1, 4);
	}
	return res;
}

int testFunctions(){
	Function* func;
	func = new Sphere();
	{
		int result_should_be_true = (testFunction(func, &evalSphere, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new Norm1();
	{
		int result_should_be_true = (testFunction(func, &evalNorm1, -100.0,  0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new NormInfinity();
	{
		int result_should_be_true = (testFunction(func, &evalNormOO, -100.0,  0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new Norm2();
	{
		int result_should_be_true = (testFunction(func, &evalSphere, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new Norm4();
	{
		int result_should_be_true = (testFunction(func, &evalNorm4, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new Norm8();
	{
		int result_should_be_true = (testFunction(func, &evalNorm8, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new Norm2PowerK(4);
	{
		int result_should_be_true = (testFunction(func, &evalNorm2p4, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new Norm1Plus2PowerMinusK(1);
	{
		int result_should_be_true = (testFunction(func, &evalNorm1pl2pm1, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new Norm1Plus2PowerMinusK(2);
	{
		int result_should_be_true = (testFunction(func, &evalNorm1pl2pm2, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new RandomPositiveDefiniteSecondDerivative(std::vector<std::string>({"1273"}));
	{
		int result_should_be_true = (testFunction(func, NULL, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new RandomPositiveDefiniteSecondDerivative(std::vector<std::string>({"42"}));
	{
		int result_should_be_true = (testFunction(func, NULL, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new ScaledSphereRand(std::vector<std::string>({"42"}));
	{
		int result_should_be_true = (testFunction(func, NULL, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new ScaledSphereRand(std::vector<std::string>({"1273"}));
	{
		int result_should_be_true = (testFunction(func, NULL, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new ScaledSphereFixed();
	{
		int result_should_be_true = (testFunction(func, &evalScaledSphere1e6, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new ScaledSphere(2517e42);
	{
		int result_should_be_true = (testFunction(func, &evalScaledSphere2517e42, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new ScaledHadamardRotatedSphereFixed();
	{
		int result_should_be_true = (testFunction(func, &evalScaledHadamardRotatedSphere1e6, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new ScaledHadamardRotatedSphere(2517e42);
	{
		int result_should_be_true = (testFunction(func, NULL, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new Schwefel1();
	{
		int result_should_be_true = (testFunction(func, &evalSchwefel, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new Schwefel2();
	{
		int result_should_be_true = (testFunction(func, &evalSchwefel2, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new Rosenbrock();
	{
		int result_should_be_true = (testFunction(func, &evalRosenbrock, -30.0, 0.0, 30.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new Rastrigin();
	{
		int result_should_be_true = (testFunction(func, &evalRastrigin, -5.12, 0.0, 5.12) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new DiagonalFunction(1e6);
	{
		int result_should_be_true = (testFunction(func, &evalDiagonal1e6, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new DiagonalFunction(2517e42);
	{
		int result_should_be_true = (testFunction(func, &evalDiagonal2517e42, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new SpherePlus();
	{
		int result_should_be_true = (testFunction(func, NULL, -100.0, 50, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	{
		int result_should_be_true = (testFunction(func, &evalSphere, 1e-20, 1e-5, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new InclinedPlane();
	{
		int result_should_be_true = (testFunction(func, &evalInclinedPlane, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new InclinedPlaneAsym();
	{
		int result_should_be_true = (testFunction(func, &evalInclinedPlaneAsym, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	func = new TwoCupsFunction();
	{
		int result_should_be_true = (testFunction(func, &evalTwoCupsFunction, -100.0, 0.0, 100.0) == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	delete func;
	return 0;
}

int start_tests(int argv, char * argc[]) {
	arbitraryprecisioncalculation::Configuration::setOutputPrecision(10);
	arbitraryprecisioncalculation::Configuration::setPrecisionSafetyMargin(BASE_PRECISION / 2);
	arbitraryprecisioncalculation::Configuration::setInitialPrecision(BASE_PRECISION);
	mpf_set_default_prec(BASE_PRECISION);
	init();
	std::cout << "start test series\n";
	std::cout << "initial used mpf_t: " << arbitraryprecisioncalculation::mpftoperations::GetNumberOfMpftValuesInUse() - arbitraryprecisioncalculation::mpftoperations::GetNumberOfMpftValuesCached() << std::endl;
	std::cout << "initial memoized mpf_t: " << arbitraryprecisioncalculation::mpftoperations::GetNumberOfMpftValuesCached() << std::endl;
	{
		int result_should_be_true = (testFunctions() == 0);
		assert(result_should_be_true);
		if(!(result_should_be_true)) return 1;
	}
	std::cout << "end test series\n";
	std::cout << "final used mpf_t: " << arbitraryprecisioncalculation::mpftoperations::GetNumberOfMpftValuesInUse() - arbitraryprecisioncalculation::mpftoperations::GetNumberOfMpftValuesCached() << std::endl;
	std::cout << "final memoized mpf_t: " << arbitraryprecisioncalculation::mpftoperations::GetNumberOfMpftValuesCached() << std::endl;
	std::cout << "All tests passed.\n";
	return 0;
}

} // namespace highprecisionpso

int main(int argv, char * argc[]) {
	return highprecisionpso::start_tests(argv,argc);
}

