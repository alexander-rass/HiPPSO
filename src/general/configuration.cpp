/**
* @file   general/configuration.cpp
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

#include "general/configuration.h"

#include <fstream>
#include <gmp.h>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <algorithm>

#include "general/includes.h"
#include "general/check_condition.h"

namespace highprecisionpso {
namespace configuration {

double g_chi = 0.72984;
double g_coefficient_local_attractor = 1.496172;
double g_coefficient_global_attractor = 1.496172;

InitializeVelocityMode g_initialize_velocity_mode = INITIALIZE_VELOCITY_MODE_ZERO;

enum UpdateGlobalAttractorMode g_update_global_attractor_mode = UPDATE_GLOBAL_ATTRACTOR_MODE_EACH_PARTICLE;

int g_particles = 2;
int g_dimensions = 2;
long long g_max_iterations = 2;

double g_time_between_backups = 60.0;
double g_time_between_run_checks = 30.0;
std::string g_run_check_configuration_file = "";

std::vector<std::vector<long long> > g_iterations_with_statistic_evaluations;
std::vector<Statistic*> g_statistics_list;

std::vector<InitializationInformation> g_position_initialization_informations;
std::vector<InitializationInformation> g_velocity_initialization_informations;

std::vector<SearchSpaceBound> g_function_bounds;

std::vector<long long> g_preserve_backup_times = std::vector<long long>();
Function* g_function = new Sphere;
enum FunctionBehaviorOutsideOfBoundsMode g_function_behavior_outside_of_bounds_mode = FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_NORMAL_EVALUATION;
BoundHandling* g_bound_handling = new BoundHandlingNoBounds;
VelocityAdjustment* g_velocity_adjustment = new VelocityAdjustmentNone;
PositionAndVelocityUpdater* g_position_and_velocity_updater = new DefaultUpdater;
Neighborhood* g_neighborhood = new GlobalBest;
Statistics* g_statistics = NULL;

std::string g_file_prefix = "";
std::string g_user_defined_file_prefix = "";
bool g_show_system_time_in_file_prefix = true;

bool g_debug_swarm_activated = false;
unsigned int g_debug_swarm_x_resolution = 2560;
unsigned int g_debug_swarm_y_resolution = 1440;
unsigned int g_debug_swarm_global_attractor_trajectory = 100;
unsigned int g_debug_swarm_image_generation_frequency = 10;
bool g_debug_swarm_show_global_attractor_on_terminal = true;
std::string g_debug_swarm_gnuplot_terminal = "png";
std::string g_debug_swarm_image_file_extension = "png";

void Init(){
	g_chi = 0.72984;
	g_coefficient_local_attractor = 1.496172;
	g_coefficient_global_attractor = 1.496172;

	g_initialize_velocity_mode = INITIALIZE_VELOCITY_MODE_ZERO;

	g_update_global_attractor_mode = UPDATE_GLOBAL_ATTRACTOR_MODE_EACH_PARTICLE;

	g_particles = 2;
	g_dimensions = 2;
	g_max_iterations = 2;

	arbitraryprecisioncalculation::configuration::Init();

	g_preserve_backup_times = std::vector<long long>();
	g_time_between_backups = 60.0;
	g_time_between_run_checks = 30.0;

	g_run_check_configuration_file = "";

	g_iterations_with_statistic_evaluations.clear();
	g_statistics_list.clear();

	g_position_initialization_informations.clear();
	g_velocity_initialization_informations.clear();

	g_function_bounds.clear();

	g_function = new Sphere;
	g_function_behavior_outside_of_bounds_mode = FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_NORMAL_EVALUATION;
	g_bound_handling = new BoundHandlingNoBounds;
	g_velocity_adjustment = new VelocityAdjustmentNone;
	g_position_and_velocity_updater = new DefaultUpdater;
	g_neighborhood = new GlobalBest;
	g_statistics = NULL;

	g_debug_swarm_activated = false;
	g_debug_swarm_x_resolution = 2560;
	g_debug_swarm_y_resolution = 1440;
	g_debug_swarm_global_attractor_trajectory = 100;
	g_debug_swarm_image_generation_frequency = 10;
	g_debug_swarm_show_global_attractor_on_terminal = true;
	std::string DEBUG_SWARM_GNUPLOT_TERMINAL = "png";
	std::string DEBUG_SWARM_FILE_EXTENSION = "png";
}


bool ReadConfigurationFile(std::string fileName) {
	Init();
	std::ifstream configFile(fileName.c_str());
	if (!configFile.good()) {
		return false;
	}
	while (!configFile.eof()) {
		std::string in;
		getline(configFile, in);
		if (in.size() == 0)
			continue;
		if (in.find("#") != std::string::npos)
			continue;
		std::vector<std::string> input;
		{
			std::istringstream is(in);
			while (is >> in) {
				input.push_back(in);
			}
			for(int i = 0; i < (int)input.size(); i++){
				if(input[i] == ""){
					for(int j = i + 1; j < (int)input.size(); j++){
						input[j - 1] = input[j];
					}
					input.pop_back();
					--i;
				}
			}
		}
		if (input.size() == 0)
			continue;
		std::string option = input[0];
		std::transform(option.begin(), option.end(), option.begin(), ::tolower);
		if (option == "fileprefix") {
			if(input.size() != 2){
				parse::SignalInvalidCommand(input);
				return false;
			}
			g_user_defined_file_prefix = input[1];
		} else if (option == "includesystemtimeinfilename") {
			if(input.size() != 2){
				parse::SignalInvalidCommand(input);
				return false;
			}
			if(input[1] == "true"){
				g_show_system_time_in_file_prefix = true;
			} else if(input[1] == "false") {
				g_show_system_time_in_file_prefix = false;
			} else {
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "particles") {
			if(input.size() != 2){
				parse::SignalInvalidCommand(input);
				return false;
			}
			std::istringstream is(input[1]);
			if(!(is >> g_particles)){
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "dimensions") {
			if(input.size() != 2){
				parse::SignalInvalidCommand(input);
				return false;
			}
			std::istringstream is(input[1]);
			if(!(is >> g_dimensions)){
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "steps") {
			if(input.size() != 2){
				parse::SignalInvalidCommand(input);
				return false;
			}
			std::istringstream is(input[1]);
			if(!(is >> g_max_iterations)){
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "function") {
			if (g_function != NULL) {
				delete (g_function);
				g_function = NULL;
			}
			unsigned int parsed_parameters = 1;
			g_function = parse::ParseFunction(input, parsed_parameters);
			if(g_function == NULL){
				parse::SignalInvalidCommand(input);
				return false;
			}
			if(parsed_parameters != input.size()){
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "functionbehavioroutsideofbounds") {
			if( input.size() != 2 ){
				parse::SignalInvalidCommand(input);
				return false;
			}
			if ( input[1] == "infinity" ) {
				configuration::g_function_behavior_outside_of_bounds_mode = configuration::FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_INFINITY;
			} else if ( input[1] == "normal" ) {
				configuration::g_function_behavior_outside_of_bounds_mode = configuration::FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_NORMAL_EVALUATION;
			} else if ( input[1] == "periodic" ) {
				configuration::g_function_behavior_outside_of_bounds_mode = configuration::FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_PERIODIC;
			} else {
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "boundhandling") {
			if (g_bound_handling != NULL) {
				delete (g_bound_handling);
				g_bound_handling = NULL;
			}
			std::string suboption1 = input[1];
			std::transform(suboption1.begin(), suboption1.end(), suboption1.begin(), ::tolower);
			if (suboption1 == "nobounds") {
				if(input.size() != 2){
					parse::SignalInvalidCommand(input);
					return false;
				}
				g_bound_handling = new BoundHandlingNoBounds;
			} else if(suboption1 == "absorption") {
				if(input.size() != 3){
					parse::SignalInvalidCommand(input);
					return false;
				}
				if(input[2] == "markAllOnChange")g_bound_handling = new BoundHandlingAbsorption(ABSORPTION_MARK_DIMENSIONS_ALL_ON_CHANGE);
				else if(input[2] == "markAllOutsideBoundsWithoutChange") g_bound_handling = new BoundHandlingAbsorption(ABSORPTION_MARK_DIMENSIONS_OUTSIDE_BOUNDS);
				else if(input[2] == "markAllOnBoundsAfterChange")g_bound_handling = new BoundHandlingAbsorption(ABSORPTION_MARK_DIMENSIONS_ON_BOUNDARY);
				else {
					parse::SignalInvalidCommand(input);
					return false;
				}
			} else if(suboption1 == "randomforth") {
				if(input.size() != 3){
					parse::SignalInvalidCommand(input);
					return false;
				}
				if(input[2] == "markAllOnChange")g_bound_handling = new BoundHandlingRandomForth(RANDOM_FORTH_MARK_DIMENSIONS_ALL_ON_CHANGE);
				else if(input[2] == "markAllOutsideBoundsWithoutChange") g_bound_handling = new BoundHandlingRandomForth(RANDOM_FORTH_MARK_DIMENSIONS_OUTSIDE_BOUNDS);
				else if(input[2] == "markAllOnBoundsAfterChange")g_bound_handling = new BoundHandlingRandomForth(RANDOM_FORTH_MARK_DIMENSIONS_ON_BOUNDARY);
				else {
					parse::SignalInvalidCommand(input);
					return false;
				}
			} else if (suboption1 == "hyperbolic") {
				if(input.size() != 2){
					parse::SignalInvalidCommand(input);
					return false;
				}
				g_bound_handling = new BoundHandlingHyperbolic;
			} else if (suboption1 == "nearest") {
				if(input.size() != 2){
					parse::SignalInvalidCommand(input);
					return false;
				}
				g_bound_handling = new BoundHandlingNearest;
			} else if (suboption1 == "reflect") {
				if(input.size() != 2){
					parse::SignalInvalidCommand(input);
					return false;
				}
				g_bound_handling = new BoundHandlingReflect;
			} else if (suboption1 == "random") {
				if(input.size() != 3){
					parse::SignalInvalidCommand(input);
					return false;
				}
				if(input[2] == "resetAll"){
					g_bound_handling = new BoundHandlingRandom(true);
				} else if(input[2] == "resetViolated") {
					g_bound_handling = new BoundHandlingRandom(false);
				} else {
					parse::SignalInvalidCommand(input);
					return false;
				}
			} else if (suboption1 == "torus") {
				if(input.size() != 2){
					parse::SignalInvalidCommand(input);
					return false;
				}
				g_bound_handling = new BoundHandlingTorus;
			} else {
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "velocityadjustment") {
			if(input.size() == 1){
				parse::SignalInvalidCommand(input);
				return false;
			}
			if (g_velocity_adjustment != NULL) {
				delete (g_velocity_adjustment);
				g_velocity_adjustment = NULL;
			}
			if (input[1] == "none") {
				if(input.size() != 2){
					parse::SignalInvalidCommand(input);
					return false;
				}
				g_velocity_adjustment = new VelocityAdjustmentNone;
			} else if (input[1] == "zero") {
				if(input.size() != 2){
					parse::SignalInvalidCommand(input);
					return false;
				}
				g_velocity_adjustment = new VelocityAdjustmentZero;
			} else if (input[1] == "mirror") {
				if(input.size() != 2){
					parse::SignalInvalidCommand(input);
					return false;
				}
				g_velocity_adjustment = new VelocityAdjustmentDeterministicBack(1.0);
			} else if (input[1] == "adjust") {
				if(input.size() != 2){
					parse::SignalInvalidCommand(input);
					return false;
				}
				g_velocity_adjustment = new VelocityAdjustmentAdjust;
			} else if (input[1] == "randomBack") {
				if(input.size() != 2){
					parse::SignalInvalidCommand(input);
					return false;
				}
				g_velocity_adjustment = new VelocityAdjustmentRandomBack;
			} else if (input[1] == "deterministicBack") {
				if(input.size() != 3){
					parse::SignalInvalidCommand(input);
					return false;
				}
				double lambda;
				{
					std::istringstream is(input[2]);
					if(!(is >> lambda)){
						parse::SignalInvalidCommand(input);
						return false;
					}
				}
				g_velocity_adjustment = new VelocityAdjustmentDeterministicBack(lambda);
			} else {
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "neighborhood") {
			if (g_neighborhood != NULL) {
				delete (g_neighborhood);
				g_neighborhood = NULL;
			}
			if (input[1] == "gBest") {
				g_neighborhood = new GlobalBest();
			} else if(input[1] == "wheel") {
				g_neighborhood = new WheelNeighborhood();
			} else if(input[1] == "lBest") {
				int km2;
				std::istringstream is(input[2]);
				is >> km2;
				AssertCondition(km2 % 2 == 0 && km2 > 0, "lBest neighborhood topology needs an even positive integer parameter.");
				g_neighborhood = new LBestNeighborhood(km2);
			} else if(input[1] == "ring") {
				g_neighborhood = new RingNeighborhood();
			} else if(input[1] == "grid") {
				int r, c;
				{
					std::istringstream is(input[2]);
					is >> r;
				}
				{
					std::istringstream is(input[3]);
					is >> c;
				}
				g_neighborhood = new GridNeighborhood(r,c);
			} else {
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "positionandvelocityupdater") {
			if (g_position_and_velocity_updater != NULL) {
				g_position_and_velocity_updater = NULL;
			}
			std::string suboption1 = input[1];
			std::transform(suboption1.begin(), suboption1.end(), suboption1.begin(), ::tolower);
			if (suboption1 == "default") {
				g_position_and_velocity_updater = new DefaultUpdater;
			} else if(suboption1 == "delta") {
				std::istringstream is(input[2]);
				double v;
				is >> v;
				if(input.size() == 3){
					g_position_and_velocity_updater = new DeltaUpdater(v);
				} else {
					std::istringstream is2(input[3]);
					double gamma;
					is2 >> gamma;
					g_position_and_velocity_updater = new DeltaUpdater(v, gamma);
				}
			} else if(suboption1 == "orientationchange") {
				std::istringstream is(input[2]);
				double v;
				is >> v;
				g_position_and_velocity_updater = new OrientationChangeUpdater(v);
			} else if(suboption1 == "dimensionindependent") {
				if(input.size() != 4){
					parse::SignalInvalidCommand(input);
					return false;
				}
				double reduction;
				int max_dimensions;
				{
					std::istringstream is(input[2]);
					is >> reduction;
				}
				{
					std::istringstream is(input[3]);
					is >> max_dimensions;
				}
				g_position_and_velocity_updater = new DimensionIndependentUpdater(reduction, max_dimensions);
			} else if (suboption1 == "testing") {
				g_position_and_velocity_updater = new TestingUpdater;
			} else {
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "initialprecision") {
			std::istringstream is(input[1]);
			is >> arbitraryprecisioncalculation::configuration::g_initial_precision;
			arbitraryprecisioncalculation::configuration::g_initial_precision_already_set = true;
		} else if (option == "precision") {
			std::istringstream is(input[1]);
			is >> arbitraryprecisioncalculation::configuration::g_precision_safety_margin;
			if(!arbitraryprecisioncalculation::configuration::g_initial_precision_already_set){
				arbitraryprecisioncalculation::configuration::g_initial_precision = arbitraryprecisioncalculation::configuration::g_precision_safety_margin;
			}
		} else if (option == "outputprecision") {
			std::istringstream is(input[1]);
			is >> arbitraryprecisioncalculation::configuration::g_output_precision;
		} else if (option == "checkprecision") {
			if(input[1] == "all"){
				arbitraryprecisioncalculation::configuration::g_check_precision_mode = arbitraryprecisioncalculation::configuration::CHECK_PRECISION_ALWAYS;
			} else if(input[1] == "allExceptStatistics"){
				arbitraryprecisioncalculation::configuration::g_check_precision_mode = arbitraryprecisioncalculation::configuration::CHECK_PRECISION_ALWAYS_EXCEPT_STATISTICS;
			} else if(input[1] == "never"){
				arbitraryprecisioncalculation::configuration::g_check_precision_mode = arbitraryprecisioncalculation::configuration::CHECK_PRECISION_NEVER;
			} else {
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "checkprecisionprobability"){
			std::istringstream is(input[1]);
			is >> arbitraryprecisioncalculation::configuration::g_check_precision_probability;
		} else if (option == "showstatistics") {
			std::vector<long long> newTimeTable;
			for(int k = 1; k <= 3; k++){
				std::istringstream is(input[k]);
				long long tmp;
				if(!(is >> tmp)){
					parse::SignalInvalidCommand(input);
					return false;
				}
				newTimeTable.push_back(tmp);
			}
			g_iterations_with_statistic_evaluations.push_back(newTimeTable);
		} else if (option == "shownamedstatistic" ) {
			unsigned int parsed = 2;
			Statistic* statistic = parse::ParseStatistic(input, parsed);
			if(statistic == NULL) {
				parse::SignalInvalidCommand(input);
				return false;
			}
			statistic = new NamedStatistic(input[1], statistic);
			for(auto st : g_statistics_list){
				if(statistic->GetName() == st->GetName()){
					std::cerr << "\"" << statistic->GetName() << "\" is used multiple times as statistic name.\n";
					parse::SignalInvalidCommand(input);
					return false;
				}
			}
			g_statistics_list.push_back(statistic);
		} else if (option == "showstatistic" ) {
			unsigned int parsed = 1;
			Statistic* statistic = parse::ParseStatistic(input, parsed);
			if(statistic == NULL || parsed != input.size()) {
				parse::SignalInvalidCommand(input);
				return false;
			}
			for(auto st : g_statistics_list){
				if(statistic->GetName() == st->GetName()){
					std::cerr << "\"" << statistic->GetName() << "\" is used multiple times as statistic name.\n";
					parse::SignalInvalidCommand(input);
					return false;
				}
			}
			g_statistics_list.push_back(statistic);
		} else if (option == "srand") {
			std::istringstream is(input[1]);
			unsigned int parsed = 1;
			arbitraryprecisioncalculation::configuration::g_standard_random_number_generator = arbitraryprecisioncalculation::parse::ParseRandomNumberGenerator(input, parsed);
			if(arbitraryprecisioncalculation::configuration::g_standard_random_number_generator == NULL || parsed != input.size()) {
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "runcheck"){
			g_run_check_configuration_file = input[1];
		} else if (option == "parameterchi"){
			std::istringstream is(input[1]);
			is >> g_chi;
		} else if (option == "parametercoefficientlocalattractor"){
			std::istringstream is(input[1]);
			is >> g_coefficient_local_attractor;
		} else if (option == "parametercoefficientglobalattractor"){
			std::istringstream is(input[1]);
			is >> g_coefficient_global_attractor;
		} else if (option == "initializevelocity"){
			if (input[1] == "zero") {
				g_initialize_velocity_mode = INITIALIZE_VELOCITY_MODE_ZERO;
			} else if (input[1] == "halfDiff"){
				g_initialize_velocity_mode = INITIALIZE_VELOCITY_MODE_HALFDIFF;
			} else if (input[1] == "random"){
				g_initialize_velocity_mode = INITIALIZE_VELOCITY_MODE_RANDOM;
			} else {
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "updateglobalattractor") {
			if (input[1] == "eachIteration") {
				g_update_global_attractor_mode = UPDATE_GLOBAL_ATTRACTOR_MODE_EACH_ITERATION;
			} else if (input[1] == "eachParticle") {
				g_update_global_attractor_mode = UPDATE_GLOBAL_ATTRACTOR_MODE_EACH_PARTICLE;
			} else {
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "preservebackup") {
			std::istringstream is(input[1]);
			long long backupstep;
			is >> backupstep;
			g_preserve_backup_times.push_back(backupstep);
		} else if (option == "initializationinformation"){
			if(input.size() < 7) {
				parse::SignalInvalidCommand(input);
				return false;
			}
			configuration::InitializationInformation info;
			bool isPosition = false;
			bool isVelocity = false;
			if(input[1] == "position"){
				isPosition = true;
			} else if(input[1] == "velocity") {
				isVelocity = true;
			} else {
				parse::SignalInvalidCommand(input);
				return false;
			}
			{
				std::istringstream is(input[2]);
				if(!(is >> info.particle_from)){
					parse::SignalInvalidCommand(input);
					return false;
				}
			}
			{
				std::istringstream is(input[3]);
				if(!(is >> info.particle_to)){
					parse::SignalInvalidCommand(input);
					return false;
				}
			}
			{
				std::istringstream is(input[4]);
				if(!(is >> info.dimension_from)){
					parse::SignalInvalidCommand(input);
					return false;
				}
			}
			{
				std::istringstream is(input[5]);
				if(!(is >> info.dimension_to)){
					parse::SignalInvalidCommand(input);
					return false;
				}
			}
			if(input[6] == "bounds"){
				if(input.size() != 9){
					parse::SignalInvalidCommand(input);
					return false;
				}
				info.information_type = configuration::INITIALIZATION_INFORMATION_TYPE_BOUNDS;
				{
					std::istringstream is(input[7]);
					if(!(is >> info.lower_bound)){
						parse::SignalInvalidCommand(input);
						return false;
					}
				}
				{
					std::istringstream is(input[8]);
					if(!(is >> info.upper_bound)){
						parse::SignalInvalidCommand(input);
						return false;
					}
				}
			} else if(input[6] == "centerandrange") {
				if(input.size() != 9){
					parse::SignalInvalidCommand(input);
					return false;
				}
				info.information_type =  configuration::INITIALIZATION_INFORMATION_TYPE_CENTER_RANGE;
				{
					std::istringstream is(input[7]);
					if(!(is >> info.lower_bound)){
						parse::SignalInvalidCommand(input);
						return false;
					}
				}
				{
					info.upper_bound = info.lower_bound;
				}
				{
					std::istringstream is(input[8]);
					if(!(is >> info.range)){
						parse::SignalInvalidCommand(input);
						return false;
					}
				}
			} else if(input[6] == "randomcenterandrange") {
				if(input.size() != 10){
					parse::SignalInvalidCommand(input);
					return false;
				}
				info.information_type =  configuration::INITIALIZATION_INFORMATION_TYPE_RANDOM_CENTER_RANGE;
				{
					std::istringstream is(input[7]);
					is >> info.lower_bound;
				}
				{
					std::istringstream is(input[8]);
					is >> info.upper_bound;
				}
				{
					std::istringstream is(input[9]);
					is >> info.range;
				}
			} else if(input[6] == "scale") {
				if(input.size() != 8){
					parse::SignalInvalidCommand(input);
					return false;
				}
				info.information_type =  configuration::INITIALIZATION_INFORMATION_TYPE_NORMAL_SCALE;
				{
					std::istringstream is(input[7]);
					is >> info.normal_scale;
				}
			} else if(input[6] == "powerscale") {
				if(input.size() != 8){
					parse::SignalInvalidCommand(input);
					return false;
				}
				info.information_type =  configuration::INITIALIZATION_INFORMATION_TYPE_POWER_SCALE;
				{
					std::istringstream is(input[7]);
					is >> info.power_scale;
				}
			} else {
				parse::SignalInvalidCommand(input);
				return false;
			}
			if(isPosition)configuration::g_position_initialization_informations.push_back(info);
			if(isVelocity)configuration::g_velocity_initialization_informations.push_back(info);
		} else if (option == "functionbounds"){
			SearchSpaceBound info;
			{
				std::istringstream is(input[1]);
				is >> info.dimension_from;
			}
			{
				std::istringstream is(input[2]);
				is >> info.dimension_to;
			}
			{
				std::istringstream is(input[3]);
				is >> info.lower_search_space_bound;
			}
			{
				std::istringstream is(input[4]);
				is >> info.upper_search_space_bound;
			}
			configuration::g_function_bounds.push_back(info);
		} else if (option == "debugswarm" ){
			g_debug_swarm_activated = true;
		} else if (option == "debugswarmyresolution" ){
			std::istringstream is (input[1]);
			is >> configuration::g_debug_swarm_y_resolution;
		} else if (option == "debugswarmxresolution" ){
			std::istringstream is (input[1]);
			is >> configuration::g_debug_swarm_x_resolution;
		} else if (option == "debugswarmshowglobalattractortrajectory" ){
			std::istringstream is (input[1]);
			is >> configuration::g_debug_swarm_global_attractor_trajectory;
		} else if (option == "debugswarmimagegenerationfrequency" ){
			std::istringstream is (input[1]);
			is >> configuration::g_debug_swarm_image_generation_frequency;
		} else if (option == "debugswarmshowglobalattractor" ){
			if(input[1] == "true"){
				configuration::g_debug_swarm_show_global_attractor_on_terminal = true;
			} else if(input[1] == "false"){
				configuration::g_debug_swarm_show_global_attractor_on_terminal = false;
			} else {
				parse::SignalInvalidCommand(input);
				return false;
			}
		} else if (option == "debugswarmgnuplotterminal") {
			if(input.size() == 1){
				parse::SignalInvalidCommand(input);
				return false;
			}
			std::ostringstream os;
			for(unsigned int i = 1; i < input.size(); i++){
				if(i > 1)os << " ";
				os << input[i];
			}
			g_debug_swarm_gnuplot_terminal = os.str();
		} else if (option == "debugswarmoutput") {
			if(input.size() != 2) {
				parse::SignalInvalidCommand(input);
				return false;
			}
			g_debug_swarm_image_file_extension = input[1];
		} else {
			parse::SignalInvalidCommand(input);
			return false;
		}
	}
	configFile.close();

	for (unsigned int i = 0; i < configuration::g_function_bounds.size(); i++) {
		SearchSpaceBound* info = &(configuration::g_function_bounds[i]);
		g_function->SetFunctionBound(info);
	}
	return true;
}

std::string GetConfigurationString() {
	std::ostringstream res;
	res << "N" << g_particles;
	res << "D" << g_dimensions;
	res << "S";
	{
		int exponent = 0;
		int stepstmp = g_max_iterations;
		if(stepstmp % 100 == 0 && stepstmp != 0){
			while(stepstmp % 10 == 0){
				++exponent;
				stepstmp /= 10;
			}
		}
		res << stepstmp;
		if(exponent > 0) res << "e" << exponent;
	}
	res << "F" << g_function->GetName();
	res << "B" << g_bound_handling->GetName();
	if (g_function_behavior_outside_of_bounds_mode == FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_INFINITY) res << "+";
	else if(g_function_behavior_outside_of_bounds_mode == FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_NORMAL_EVALUATION) res << "N";
	else if(g_function_behavior_outside_of_bounds_mode == FUNCTION_BEHAVIOR_OUTSIDE_OF_BOUNDS_PERIODIC) res << "P";
	res << "V" << g_velocity_adjustment->GetName();
	res << "N" << g_neighborhood->GetName();
	res << "PVU" << g_position_and_velocity_updater->GetName();
#if 0 // possible further information in filename
	res << "ShPa";
	if (SHOW_PARTICLES){
		res << SHOW_PARTICLES_N_TIMES;
	} else {
		res <<"-";
	}
	res << "Stat" << ((SHOW_LOG_POSITION) ? "+" : "-")
		<< ((SHOW_LOG_POTENTIAL) ? "+" : "-")
		<< ((SHOW_LOG_DIFF_TO_OPT) ? "+" : "-")
		<< ((SHOW_LOG_VELOCITY) ? "+" : "-");
#endif 
	res << "Pr" << arbitraryprecisioncalculation::configuration::g_precision_safety_margin;
	if(arbitraryprecisioncalculation::configuration::g_initial_precision != arbitraryprecisioncalculation::configuration::g_precision_safety_margin)res << "IPr" << arbitraryprecisioncalculation::configuration::g_initial_precision;
	if(arbitraryprecisioncalculation::configuration::g_check_precision_mode == arbitraryprecisioncalculation::configuration::CHECK_PRECISION_ALWAYS) res << "A";
	else if(arbitraryprecisioncalculation::configuration::g_check_precision_mode == arbitraryprecisioncalculation::configuration::CHECK_PRECISION_ALWAYS_EXCEPT_STATISTICS) res << "AeS";
	else if(arbitraryprecisioncalculation::configuration::g_check_precision_mode == arbitraryprecisioncalculation::configuration::CHECK_PRECISION_NEVER) res << "N";
	else res << "FAIL";
	if(arbitraryprecisioncalculation::configuration::g_check_precision_probability < 1){
		res << "_CP" << arbitraryprecisioncalculation::configuration::g_check_precision_probability;
	}
	std::set<int> modifiedDimensionsSet;
	for(auto info: g_position_initialization_informations){
		for(int d = std::max(info.dimension_from, 0); d <= std::min(info.dimension_to, configuration::g_dimensions - 1); d++){
			modifiedDimensionsSet.insert(d);
		}
	}
	std::vector<int> modifiedDimensions(modifiedDimensionsSet.begin(), modifiedDimensionsSet.end());
	if(modifiedDimensions.size() > 0){
		res << "_MD";
		unsigned int i = 0;
		while(i < modifiedDimensions.size()){
			unsigned int j = i + 1;
			while(j < modifiedDimensions.size()){
				if(modifiedDimensions[j] != modifiedDimensions[j-1] + 1)break;
				j++;
			}
			if(j <= i + 2){
				res << modifiedDimensions[i] << ".";
				i++;
			} else {
				res << modifiedDimensions[i] << ".." << modifiedDimensions[j-1] << ".";
				i = j;
			}
		}
	}
	res << "_" << g_chi << "_" << g_coefficient_local_attractor << "_" << g_coefficient_global_attractor;
	res << "IV";
	switch(g_initialize_velocity_mode){
		case INITIALIZE_VELOCITY_MODE_ZERO:
			res << "0";
			break;
		case INITIALIZE_VELOCITY_MODE_HALFDIFF:
			res << "hd";
			break;
		case INITIALIZE_VELOCITY_MODE_RANDOM:
			res << "rand";
			break;
		default:
			res << "fail";
	}
	res << "UG";
	if(g_update_global_attractor_mode == UPDATE_GLOBAL_ATTRACTOR_MODE_EACH_PARTICLE) res << "p";
	else if(g_update_global_attractor_mode == UPDATE_GLOBAL_ATTRACTOR_MODE_EACH_ITERATION) res << "i";
	else res << "fail";
	res << arbitraryprecisioncalculation::configuration::g_standard_random_number_generator->GetName();
	return std::string(res.str());
}

std::string GetFilePrefix(){
	std::ostringstream fName;
	if(g_user_defined_file_prefix == ""){
		fName << configuration::GetConfigurationString();
	} else {
		fName << g_user_defined_file_prefix;
	}

	if(g_show_system_time_in_file_prefix){
		time_t t = time(0); // get time now
		struct tm * now = localtime(&t);
		fName << "." << (now->tm_year + 1900) << "-";
		if (now->tm_mon + 1 < 10)
			fName << "0";
		fName << (now->tm_mon + 1) << '-';
		if (now->tm_mday < 10)
			fName << "0";
		fName << now->tm_mday << '_';
		if (now->tm_hour < 10)
			fName << "0";
		fName << now->tm_hour << '.';
		if (now->tm_min < 10)
			fName << '0';
		fName << now->tm_min << '.';
		if (now->tm_sec < 10)
			fName <<  '0';
		fName << now->tm_sec;
	}
	std::string res = fName.str();
	return res;
}

} // namespace configuration
} // namespace highprecisionpso
