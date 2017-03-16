/**
* @file   main.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   June, 2013
* @brief  This file contains the main methods to start the PSO algorithm.
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

#include "general/main.h"

#include <algorithm>
#include <dirent.h>
#include <general/random_number_generator.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <unistd.h>

#include "function/function.h"
#include "general/check_condition.h"
#include "general/configuration.h"
#include "general/general_objects.h"
#include "general/operations.h"
#include "general/particle.h"
#include "general/visualization.h"
#include "neighborhood/neighborhood.h"
#include "position_and_velocity_updater/position_and_velocity_updater.h"
#include "statistics/direct_statistics.h"
#include "statistics/statistics.h"

void usage(int argc, char * argv[]){
	std::cout << "usage:\n";
	std::cout << std::string(argv[0]) << " c <config file name>\n";
	std::cout << "\tStarts a PSO run specified by the configuration in the specified configuration file.\n";
	std::cout << std::string(argv[0]) << " r <configuration backup file name with extension confBU>\n";
	std::cout << "\tRestarts a stopped PSO run.\n";
	std::cout << "\tThe file with extension \".SHUTDOWN\" needs to be available.\n";
	std::cout << std::string(argv[0]) << " rf <configuration backup file name with extension confBU>\n";
	std::cout << "\tForces the restart of a stopped PSO run.\n";
	std::cout << "\tThe file with extension \".SHUTDOWN\" is not necessary.\n";
	std::cout << std::string(argv[0]) << " restart <config gile name> <backup file name>\n";
	std::cout << "\tRestarts a PSO with the specified configuration file and backup file.\n";
	std::cout << "\tIt starts running at the time step stored in the backup file.\n";
	std::cout << "\tThe configuration file can be modified as long as the changes do not influence the movement of the swarm.\n";
	std::cout << "\tFor example the number of iterations can be increased or statistics (and when they should be displayed) can be modified completely.\n";
	std::cout << std::string(argv[0]) << " restartAll <folder>\n";
	std::cout << "\tRestarts all PSO runs in the specified folder if file with extension \".SHUTDOWN\" is available.\n";
	std::cout << "\tThis command can be used in a cron job to restart PSO runs automatically if a time interval starts where running PSO is not forbidden.\n";
	std::cout << std::string(argv[0]) << " -version\n";
	std::cout << "\tPrints the version of the PSO program.\n";
	std::cout << std::string(argv[0]) << " -gmpversion\n";
	std::cout << "\tPrints the used version of the gmp library.\n";
}

int main(int argc, char * argv[]) {
	{
		bool commandOK = true;
		if (argc == 3) {
			std::string command(argv[1]);
			if (command == "c") {
				commandOK = highprecisionpso::configuration::ReadConfigurationFile(std::string(argv[2]));
				if (commandOK) {
					mpf_set_default_prec(highprecisionpso::configuration::g_initial_precision);

					highprecisionpso::configuration::g_file_prefix = highprecisionpso::configuration::GetFilePrefix();

					std::ifstream confFile(argv[2]);
					FILE* backup = fopen(
							(highprecisionpso::configuration::g_file_prefix + ".confBU").c_str(),
							"w");
					while (!confFile.eof()) {
						std::string tmp;
						getline(confFile, tmp);
						if(tmp.size() == 0 || tmp[0] == '#'){
							continue;
						}
						fprintf(backup, "%s\n", tmp.c_str());
					}
					fclose(backup);
					confFile.close();

					FILE* logging = fopen(
							(highprecisionpso::configuration::g_file_prefix + ".log").c_str(), "a");
					time_t rawtime = time(NULL);
					struct tm * timeinfo = localtime(&rawtime);
					fprintf(logging, "begin    %s", asctime(timeinfo));
					fclose(logging);

					highprecisionpso::InitAndDoPso();
				}
			} else if (command == "r" || command == "rf") {
				// restore old run
				// r normal mode
				// rf forced mode (restores the run no matter whether a SHUTDOWN file exists or not)
				std::string pref(argv[2]);
				{
					std::string ending = ".confBU";
					if(pref.size() > ending.size()){
						std::string fin = pref.substr(pref.size() - ending.size());
						if(fin == ending){
							pref = pref.substr(0, pref.size() - ending.size());
						}
					}
				}
				highprecisionpso::configuration::g_file_prefix = pref;
				std::string configFileName = pref + ".confBU";
				const char* confFile = configFileName.c_str();
				if (!(highprecisionpso::configuration::ReadConfigurationFile(confFile))){
					std::cerr << "could not read configuration file " << pref << ".confBU" << std::endl;
					return 1;
				}
				if (!highprecisionpso::AllowedToRun()) {
					std::cout << "not allowed to run\n";
					return 0;
				}

				std::ifstream f((highprecisionpso::configuration::g_file_prefix + ".SHUTDOWN").c_str());
				bool isAv = f.good();
				f.close();
				if (isAv) {
					remove((highprecisionpso::configuration::g_file_prefix + ".SHUTDOWN").c_str());
				} else {
					if (command == "r") {
						std::cout << "SHUTDOWN file missing\n";
						return 0;
					}
				}

				highprecisionpso::RestoreAndDoPso();
			} else if (command == "restartAll") {
				return highprecisionpso::StartTasks(argv);
			} else {
				commandOK = false;
			}
		} else if(argc == 2){
			std::string command(argv[1]);
			if(command == "-version"){
				std::cout << "version: " << highprecisionpso::PSO_PROGRAM_VERSION.GetCompleteVersion() << std::endl;
				return 0;
			} else if(command == "-gmpversion"){
					std::cout << "gmp version: " << __GNU_MP_VERSION << "."<< __GNU_MP_VERSION_MINOR << "."<< __GNU_MP_VERSION_PATCHLEVEL << std::endl;
					return 0;
			} else {
				commandOK = false;
			}
		} else if(argc == 4){
			std::string command(argv[1]);
			if (command == "restart") {
				// restarts old run from configuration file and backup file
				std::string configFileName(argv[2]);
				std::string backupFileName(argv[3]);
				const char* confFile = configFileName.c_str();
				if (!(highprecisionpso::configuration::ReadConfigurationFile(confFile))){
					std::cerr << "could not read configuration file " << configFileName << std::endl;
					return 1;
				}
				if (!highprecisionpso::AllowedToRun()) {
					std::cout << "not allowed to run\n";
					return 0;
				}

				highprecisionpso::configuration::g_file_prefix = highprecisionpso::configuration::GetFilePrefix();
				{ // copy config file
					std::ifstream confFile(argv[2]);
					FILE* backup = fopen(
							(highprecisionpso::configuration::g_file_prefix + ".confBU").c_str(),
							"w");
					while (!confFile.eof()) {
						std::string tmp;
						getline(confFile, tmp);
						fprintf(backup, "%s\n", tmp.c_str());
					}
					fclose(backup);
					confFile.close();
				}
				{ // copy backup file
					std::ifstream confFile(argv[3]);
					FILE* backup = fopen(
							(highprecisionpso::configuration::g_file_prefix + ".backup").c_str(),
							"w");
					while (!confFile.eof()) {
						std::string tmp;
						getline(confFile, tmp);
						fprintf(backup, "%s\n", tmp.c_str());
					}
					fclose(backup);
					confFile.close();
				}

				highprecisionpso::RestoreAndDoPso();
			} else {
				commandOK = false;
			}
		} else {
			commandOK = false;
		}
		if (!commandOK) {
			usage(argc, argv);
			return 1;
		}
	}
	return 0;
}

namespace highprecisionpso {

const ProgramVersion PSO_PROGRAM_VERSION (1, 0, 0);

time_t LAST_BACKUP;
time_t LAST_RUN_CHECK;

int RES_SYSCALLS = 0;


Statistics* RestoreAndDoPso() {

	std::vector<Particle*>* swarm = new std::vector<Particle*>();

	Statistics* statistics = new Statistics(swarm);
	configuration::g_statistics = statistics;

	for (int i = 0; i < configuration::g_particles; i++) {
		Particle* p = new Particle();
		swarm->push_back(p);
	}
	std::ifstream bu;
	bu.open((configuration::g_file_prefix + ".backup").c_str());
	std::string version_string;
	bu >> version_string;
	ProgramVersion version_of_stored_data (version_string);
	ProgramVersion minimal_version("0.0.0");
	AssertCondition(version_of_stored_data >= minimal_version, "Stored data was generated with to old or invalid program version.");
	long long prec;
	bu >> prec;
	mpf_set_default_prec(prec);
	configuration::g_standard_random_number_generator->LoadData(&bu, &version_of_stored_data);
	configuration::g_statistics->LoadData(&bu, &version_of_stored_data);
	configuration::g_neighborhood->LoadData(&bu, &version_of_stored_data);
	configuration::g_position_and_velocity_updater->LoadData(&bu, &version_of_stored_data);
	std::string repeated_version_string;
	bu >> repeated_version_string;
	AssertCondition(bu.good(), "Could not read stored data. Data file corrupted.");
	AssertCondition(repeated_version_string == version_string, "Could not read stored data. Data file corrupted.");
	bu.close();

	FILE* logging = fopen(
			(configuration::g_file_prefix + ".log").c_str(), "a");
	time_t rawtime = time(NULL);
	struct tm * timeinfo = localtime(&rawtime);
	std::string tmpstr(asctime(timeinfo));
	if (tmpstr[tmpstr.size() - 1] == '\n')
		tmpstr = tmpstr.substr(0, tmpstr.size() - 1);
	fprintf(logging, "restore  %s with %lld steps\n", tmpstr.c_str(),
			statistics->current_iteration);
	fclose(logging);

	return DoPso(statistics);
}

Statistics* InitAndDoPso() {
	std::vector<Particle*>* swarm = new std::vector<Particle*>();

	Statistics* statistics = new Statistics(swarm);
	configuration::g_statistics = statistics;
	Function* function = configuration::g_function;
	std::vector< std::vector< std::vector<mpf_t*> > > position_velocity_ranges(2), position_velocity_centers(2), position_velocity_scales(2);
	{
		std::vector<mpf_t*> posLow = function->GetLowerSearchSpaceBound();
		std::vector<mpf_t*> posHigh = function->GetUpperSearchSpaceBound();
		std::vector<mpf_t*> posDiff = vectoroperations::Subtract(posHigh, posLow);
		std::vector<mpf_t*> range1P = vectoroperations::Multiply(posDiff, 0.5);
		std::vector<mpf_t*> center1P = vectoroperations::Add(posLow, range1P);
		for (int i = 0; i < configuration::g_particles; i++) {
			position_velocity_ranges[0].push_back(vectoroperations::Clone(range1P));
			position_velocity_ranges[1].push_back(vectoroperations::Clone(range1P));
			position_velocity_centers[0].push_back(vectoroperations::Clone(center1P));
			position_velocity_centers[1].push_back(vectoroperations::GetConstantVector(configuration::g_dimensions, 0.0));
			position_velocity_scales[0].push_back(vectoroperations::GetConstantVector(configuration::g_dimensions, 1.0));
			position_velocity_scales[1].push_back(vectoroperations::GetConstantVector(configuration::g_dimensions, 1.0));
		}
		vectoroperations::ReleaseValues(posLow);
		vectoroperations::ReleaseValues(posHigh);
		vectoroperations::ReleaseValues(posDiff);
		vectoroperations::ReleaseValues(range1P);
		vectoroperations::ReleaseValues(center1P);
	}
	std::vector<std::vector<configuration::InitializationInformation> > initialization_informations = {configuration::g_position_initialization_informations, configuration::g_velocity_initialization_informations};
	for (unsigned int position_or_velocity = 0; position_or_velocity < 2; position_or_velocity++){
		std::vector<configuration::InitializationInformation> * initialization_informations;
		if(position_or_velocity == 0){
			initialization_informations = & configuration::g_position_initialization_informations;
		} else {
			initialization_informations = & configuration::g_velocity_initialization_informations;
		}
		for( auto info : *initialization_informations ){
			int fromParticle = std::max(0, info.particle_from);
			int toParticle = std::min(configuration::g_particles - 1, info.particle_to);
			int fromDimension = std::max(0, info.dimension_from);
			int toDimension = std::min(configuration::g_dimensions - 1, info.dimension_to);
			if (info.information_type == configuration::INITIALIZATION_INFORMATION_TYPE_BOUNDS) {
				mpf_t* low = mpftoperations::ToMpft(info.lower_bound);
				mpf_t* high = mpftoperations::ToMpft(info.upper_bound);
				mpf_t* diff = mpftoperations::Subtract(high, low);
				mpf_t* newRange = mpftoperations::Multiply(diff, 0.5);
				mpf_t* newCenter = mpftoperations::Add(low, newRange);
				for(int did = fromDimension; did <= toDimension; did++) {
					for(int pid = fromParticle; pid <= toParticle; pid++) {
						mpftoperations::ReleaseValue(position_velocity_ranges[position_or_velocity][pid][did]);
						mpftoperations::ReleaseValue(position_velocity_centers[position_or_velocity][pid][did]);
						position_velocity_ranges[position_or_velocity][pid][did] = mpftoperations::Clone(newRange);
						position_velocity_centers[position_or_velocity][pid][did] = mpftoperations::Clone(newCenter);
					}
				}
				mpftoperations::ReleaseValue(low);
				mpftoperations::ReleaseValue(high);
				mpftoperations::ReleaseValue(diff);
				mpftoperations::ReleaseValue(newRange);
				mpftoperations::ReleaseValue(newCenter);
			} else if(info.information_type == configuration::INITIALIZATION_INFORMATION_TYPE_CENTER_RANGE
					|| info.information_type == configuration::INITIALIZATION_INFORMATION_TYPE_RANDOM_CENTER_RANGE) {
				mpf_t* low = mpftoperations::ToMpft(info.lower_bound);
				mpf_t* high = mpftoperations::ToMpft(info.upper_bound);
				mpf_t* diff = mpftoperations::Subtract(high, low);
				mpf_t* newRange = mpftoperations::ToMpft(info.range);
				for(int did = fromDimension; did <= toDimension; did++) {
					mpf_t* part = mpftoperations::Randomize(diff);
					mpf_t* newCenter = mpftoperations::Add(low, part);
					for(int pid = fromParticle; pid <= toParticle; pid++) {
						mpftoperations::ReleaseValue(position_velocity_ranges[position_or_velocity][pid][did]);
						mpftoperations::ReleaseValue(position_velocity_centers[position_or_velocity][pid][did]);
						position_velocity_ranges[position_or_velocity][pid][did] = mpftoperations::Clone(newRange);
						position_velocity_centers[position_or_velocity][pid][did] = mpftoperations::Clone(newCenter);
					}
					mpftoperations::ReleaseValue(newCenter);
					mpftoperations::ReleaseValue(part);
				}
				mpftoperations::ReleaseValue(low);
				mpftoperations::ReleaseValue(high);
				mpftoperations::ReleaseValue(diff);
				mpftoperations::ReleaseValue(newRange);
			} else if(info.information_type == configuration::INITIALIZATION_INFORMATION_TYPE_NORMAL_SCALE) {
				mpf_t* scale = mpftoperations::ToMpft(info.normal_scale);
				for(int did = fromDimension; did <= toDimension; did++) {
					for(int pid = fromParticle; pid <= toParticle; pid++) {
						mpf_t* oldScale = position_velocity_scales[position_or_velocity][pid][did];
						position_velocity_scales[position_or_velocity][pid][did] = mpftoperations::Multiply(oldScale, scale);
						mpftoperations::ReleaseValue(oldScale);
					}
				}
				mpftoperations::ReleaseValue(scale);
			} else if(info.information_type == configuration::INITIALIZATION_INFORMATION_TYPE_POWER_SCALE) {
				mpf_t* start;
				if (info.power_scale < 0) {
					start = mpftoperations::ToMpft(0.5);
				} else {
					start = mpftoperations::ToMpft(2.0);
				}
				mpf_t* scale = mpftoperations::Pow(start, abs(info.power_scale));
				for(int did = fromDimension; did <= toDimension; did++) {
					for(int pid = fromParticle; pid <= toParticle; pid++) {
						mpf_t* oldScale = position_velocity_scales[position_or_velocity][pid][did];
						position_velocity_scales[position_or_velocity][pid][did] = mpftoperations::Multiply(oldScale, scale);
						mpftoperations::ReleaseValue(oldScale);
					}
				}
				mpftoperations::ReleaseValue(start);
				mpftoperations::ReleaseValue(scale);
			} else {
				AssertCondition(false, "Invalid / Unsupported initialization specification.");
			}
		}
	}
	for(unsigned int position_or_velocity = 0; position_or_velocity < 2; position_or_velocity++){
		for(int did = 0; did < configuration::g_dimensions; did++) {
			for(int pid = 0; pid < configuration::g_particles; pid++) {
				mpf_t* newRange = mpftoperations::Multiply(position_velocity_ranges[position_or_velocity][pid][did], position_velocity_scales[position_or_velocity][pid][did]);
				mpftoperations::ReleaseValue(position_velocity_ranges[position_or_velocity][pid][did]);
				mpftoperations::ReleaseValue(position_velocity_scales[position_or_velocity][pid][did]);
				position_velocity_ranges[position_or_velocity][pid][did] = newRange;
			}
		}
	}
	for (int i = 0; i < configuration::g_particles; i++) {
		Particle* p = new Particle();
		std::vector<mpf_t*> position_diff = vectoroperations::Multiply(position_velocity_ranges[0][i], 2.0);
		std::vector<mpf_t*> position_low = vectoroperations::Subtract(position_velocity_centers[0][i], position_velocity_ranges[0][i]);
		std::vector<mpf_t*> velocity_diff = vectoroperations::Multiply(position_velocity_ranges[1][i], 2.0);
		std::vector<mpf_t*> velocity_low = vectoroperations::Subtract(position_velocity_centers[1][i], position_velocity_ranges[1][i]);

		std::vector<mpf_t*> posRandom = vectoroperations::Randomize(position_diff);
		std::vector<mpf_t*> newPosition = vectoroperations::Add(position_low, posRandom);
		p->SetPosition(newPosition);
		vectoroperations::ReleaseValues(posRandom);
		if(configuration::g_initialize_velocity_mode == configuration::INITIALIZE_VELOCITY_MODE_ZERO){  // zero velocity initialisation
			std::vector<mpf_t*> newVelocity = vectoroperations::GetConstantVector(configuration::g_dimensions, 0.0);
			p->SetVelocity(newVelocity);
			vectoroperations::ReleaseValues(newVelocity);
		} else if(configuration::g_initialize_velocity_mode == configuration::INITIALIZE_VELOCITY_MODE_HALFDIFF){ // halfdiff velocity initialisation
			posRandom = vectoroperations::Randomize(position_diff);
			std::vector<mpf_t*> tmpPosition = vectoroperations::Add(position_low, posRandom);
			std::vector<mpf_t*> curDiff = vectoroperations::Subtract(tmpPosition, newPosition);
			std::vector<mpf_t*> newVelocity = vectoroperations::Multiply(curDiff, 0.5);
			p->SetVelocity(newVelocity);
			vectoroperations::ReleaseValues(posRandom);
			vectoroperations::ReleaseValues(tmpPosition);
			vectoroperations::ReleaseValues(curDiff);
			vectoroperations::ReleaseValues(newVelocity);
		} else if(configuration::g_initialize_velocity_mode == configuration::INITIALIZE_VELOCITY_MODE_RANDOM){ // uniform velocity initialisation
			posRandom = vectoroperations::Randomize(velocity_diff);
			std::vector<mpf_t*> newVelocity = vectoroperations::Add(posRandom, velocity_low);
			p->SetVelocity(newVelocity);
			vectoroperations::ReleaseValues(newVelocity);
			vectoroperations::ReleaseValues(posRandom);
		} else {
			return NULL;
		}
		vectoroperations::ReleaseValues(newPosition);
		vectoroperations::ReleaseValues(position_velocity_ranges[0][i]);
		vectoroperations::ReleaseValues(position_velocity_ranges[1][i]);
		vectoroperations::ReleaseValues(position_velocity_centers[0][i]);
		vectoroperations::ReleaseValues(position_velocity_centers[1][i]);
		swarm->push_back(p);

		vectoroperations::ReleaseValues(position_diff);
		vectoroperations::ReleaseValues(position_low);
		vectoroperations::ReleaseValues(velocity_diff);
		vectoroperations::ReleaseValues(velocity_low);
	}
	configuration::g_neighborhood->ProceedAllUpdates();

	statistics->current_iteration = 0;
	return DoPso(statistics);
}

Statistics* DoPso(Statistics* statistics) {
	std::vector<Particle*>* &swarm = statistics->swarm;
	long long & step = statistics->current_iteration;

	int lastNumberOfmpft = mpftoperations::GetNumberOfMpftValuesInUse() - mpftoperations::GetNumberOfMpftValuesCached();
	int startstep = step;

	LAST_BACKUP = time(NULL);
	LAST_RUN_CHECK = 0;

	sort(configuration::g_preserve_backup_times.begin(),
			configuration::g_preserve_backup_times.end());
	unsigned int nextBackupStepId = 0;
	while(nextBackupStepId < configuration::g_preserve_backup_times.size() 
			&& step > configuration::g_preserve_backup_times[nextBackupStepId]){
		++nextBackupStepId;
	}
	configuration::g_increase_precision = false;
	if(configuration::g_debug_swarm_activated){
		visualization::VisualizeCurrentSwarm();
	}
	if(step == 0){
		statistics->EvaluateStatistics();
	}
	while (step < configuration::g_max_iterations) {

		time_t currentTime = time(NULL);
		if (difftime(currentTime, LAST_RUN_CHECK)
				> configuration::g_time_between_run_checks) {
			if (!AllowedToRun()) {
				Shutdown();
				return statistics;
			}
			LAST_RUN_CHECK = currentTime;
		}
		if (difftime(currentTime, LAST_BACKUP)
				> configuration::g_time_between_backups) {
			std::string tmpFilename = configuration::g_file_prefix + ".backup";
			WriteCurrentState(tmpFilename);
		}

		if (nextBackupStepId < configuration::g_preserve_backup_times.size()
				&& configuration::g_preserve_backup_times[nextBackupStepId] == step){
			std::string tmpFilename = configuration::g_file_prefix + ".backup";
			WriteCurrentState(tmpFilename);
			std::ostringstream os;
			os << configuration::g_file_prefix << ".S" << step << ".backup";
			tmpFilename = os.str();
			WriteCurrentState(tmpFilename);

			while(nextBackupStepId < configuration::g_preserve_backup_times.size()
					&& step >= configuration::g_preserve_backup_times[nextBackupStepId]){
				++nextBackupStepId;
			}
		}


		if (lastNumberOfmpft != mpftoperations::GetNumberOfMpftValuesInUse() - mpftoperations::GetNumberOfMpftValuesCached()) {
			if (step > 2 + startstep) {
				std::cerr << "step " << step << std::endl << "last mpf_t "
					<< lastNumberOfmpft << std::endl << "curr mpf_t "
					<< mpftoperations::GetNumberOfMpftValuesInUse() - mpftoperations::GetNumberOfMpftValuesCached() << std::endl;
			}
			lastNumberOfmpft = mpftoperations::GetNumberOfMpftValuesInUse() - mpftoperations::GetNumberOfMpftValuesCached();
		}
		for (int id = 0; id < configuration::g_particles; id++) {
			(*swarm)[id]->UpdatePosition();
			if(configuration::g_update_global_attractor_mode == configuration::UPDATE_GLOBAL_ATTRACTOR_MODE_EACH_PARTICLE){
				configuration::g_neighborhood->ProceedAllUpdates();
			}
			if(configuration::g_increase_precision){
				configuration::g_increase_precision = false;
				mpftoperations::IncreasePrecision();
			}
		}
		if(configuration::g_update_global_attractor_mode == configuration::UPDATE_GLOBAL_ATTRACTOR_MODE_EACH_ITERATION){
			configuration::g_neighborhood->ProceedAllUpdates();
		}

		step++;

		if(configuration::g_debug_swarm_activated){
			visualization::VisualizeCurrentSwarm();
		}

		if(configuration::g_increase_precision){
			configuration::g_increase_precision = false;
			mpftoperations::IncreasePrecision();
		}

		statistics->EvaluateStatistics();

	}

	{
		std::string tmpFilename = configuration::g_file_prefix + ".backup";
		WriteCurrentState(tmpFilename);
	}
	FILE* logging = fopen((configuration::g_file_prefix + ".log").c_str(), "a");
	time_t rawtime = time(NULL);
	struct tm * timeinfo = localtime(&rawtime);
	std::string tmpstr(asctime(timeinfo));
	if (tmpstr[tmpstr.size() - 1] == '\n')
		tmpstr = tmpstr.substr(0, tmpstr.size() - 1);
	fprintf(logging, "finished %s with %lld steps\n", tmpstr.c_str(),
			statistics->current_iteration);
	fclose(logging);

	return statistics;
}

void WriteCurrentState(std::string filename) {

	std::ifstream ofile(filename.c_str());
	bool backup = false;
	if (ofile.good()) {
		ofile.close();
		rename(filename.c_str(), (filename + "TMP").c_str());
		backup = true;
	}

	std::ofstream bu(filename.c_str());
	bu << PSO_PROGRAM_VERSION.GetCompleteVersion() << std::endl;
	bu << mpf_get_default_prec() << std::endl;
	configuration::g_standard_random_number_generator->StoreData(&bu);
	configuration::g_statistics->StoreData(&bu);
	configuration::g_neighborhood->StoreData(&bu);
	configuration::g_position_and_velocity_updater->StoreData(&bu);
	bu << PSO_PROGRAM_VERSION.GetCompleteVersion() << std::endl;
	bu.close();

	Statistics* statistics = configuration::g_statistics;
	if(statistics->stored_statistical_iterations.size() > 0) {
		for(unsigned int i = 0; i < configuration::g_statistics_list.size(); i++){
			FILE* stream = fopen(
					(configuration::g_file_prefix + ".STAT." + configuration::g_statistics_list[i]->GetName() + ".txt").c_str(), "a");
			AssertCondition(statistics->stored_statistical_iterations.size() > 0, "Statistical steps not available.");
			AssertCondition(statistics->stored_statistical_data.size() > i, "Statistical data for statistic \"" + configuration::g_statistics_list[i]->GetName() + "\" not available.");
			AssertCondition(statistics->stored_statistical_iterations.size() == statistics->stored_statistical_data[i].size(), "Statistical data for statistic \"" + configuration::g_statistics_list[i]->GetName() + "\" has wrong number of statistical entries.");
			for (unsigned int j = 0;
					j < statistics->stored_statistical_iterations.size(); j++) {
				fprintf(stream, "%lld %s\n", statistics->stored_statistical_iterations[j], statistics->stored_statistical_data[i][j].c_str());
			}
			statistics->stored_statistical_data[i].clear();
			fclose(stream);

		}
	}
	statistics->stored_statistical_iterations.clear();
	if (backup) {
		remove((filename + "TMP").c_str());
	}
	LAST_BACKUP = time(NULL);
}

bool AllowedToRun() {
	bool ok = true;
	time_t currentTime = time(NULL);
	tm* timeInfo = localtime(&currentTime);
	bool allowed = false;
	bool allowedNeeded = false;
	if (configuration::g_run_check_configuration_file != "") {
		std::ifstream file(configuration::g_run_check_configuration_file.c_str());
		if (file.good()) {
			while (file.good() && !file.eof()) {
				std::string in;
				getline(file, in);
				if (in.size() == 0)
					continue;
				if (in.find("#") != std::string::npos)
					continue;
				std::vector<std::string> input;
				std::istringstream is(in);
				while (is >> in) {
					input.push_back(in);
				}
				if (input.size() == 0)
					continue;
				if (input[0] == "f") { // forbidden time slots format: HH MM HH MM
					if (input.size() != 5)
						continue;
					bool formatOk = true;
					for (int i = 1; i < 5; i++) {
						if (input[i].size() != 2) {
							formatOk = false;
							break;
						}
					}
					if (!formatOk)
						continue;
					int minutes1 = mpftoperations::StringToInt(input[1]) * 60
						+ mpftoperations::StringToInt(input[2]);
					int minutes2 = mpftoperations::StringToInt(input[3]) * 60
						+ mpftoperations::StringToInt(input[4]);
					int cminutes = timeInfo->tm_hour * 60 + timeInfo->tm_min;
					if (minutes1 <= cminutes && cminutes <= minutes2) {
						ok = false;
					}
				} else if (input[0] == "a") { // allowed time slots format: HH MM HH MM
					// current time must be in one of the allowed time slots
					if (input.size() != 5)
						continue;
					bool formatOk = true;
					for (int i = 1; i < 5; i++) {
						if (input[i].size() != 2) {
							formatOk = false;
							break;
						}
					}
					if (!formatOk)
						continue;
					int minutes1 = mpftoperations::StringToInt(input[1]) * 60
						+ mpftoperations::StringToInt(input[2]);
					int minutes2 = mpftoperations::StringToInt(input[3]) * 60
						+ mpftoperations::StringToInt(input[4]);
					int cminutes = timeInfo->tm_hour * 60 + timeInfo->tm_min;
					if (minutes1 <= cminutes && cminutes <= minutes2) {
						allowed = true;
					}
					allowedNeeded = true;

				}
			}
		}
		file.close();
	}
	if (allowedNeeded && !allowed) {
		ok = false;
	}
	return ok;
}

void Shutdown() {
	{
		std::string tmpFilename = configuration::g_file_prefix + ".backup";
		WriteCurrentState(tmpFilename);
	}
	FILE* f = fopen((configuration::g_file_prefix + ".SHUTDOWN").c_str(), "w");
	fprintf(f, "1\n");
	fclose(f);

	FILE* logging = fopen((configuration::g_file_prefix + ".log").c_str(), "a");
	time_t rawtime = time(NULL);
	struct tm * timeinfo = localtime(&rawtime);
	std::string tmpstr(asctime(timeinfo));
	if (tmpstr[tmpstr.size() - 1] == '\n')
		tmpstr = tmpstr.substr(0, tmpstr.size() - 1);
	fprintf(logging, "stop     %s with %lld steps\n", tmpstr.c_str(),
			configuration::g_statistics->current_iteration);
	fclose(logging);
}

int StartTasks( char * argv[]) {
	DIR* directory;
	struct dirent *entry;
	if (( directory = opendir (argv[2])) == NULL ){
		std::cerr << "Could not open directory \"" << std::string(argv[2]) << "\"\n";
		return 1;
	}
	std::vector<std::string> files;
	std::vector<std::string> endings = {".confBU", ".SHUTDOWN"};
	std::vector<std::vector<std::string> > process_files(endings.size());
	while ( ( entry = readdir (directory)) != NULL ){
		std::string file = std::string(entry->d_name);
		files.push_back(file);
	}
	closedir (directory);
	char parameter1[2];
	std::string parameter1_s = "r";
	strcpy(parameter1, parameter1_s.c_str());
	argv[1] = parameter1;
	int errors_occured = 0;
	for(auto file : files) {
		auto rfind_result = file.rfind(endings[0]);
		if(rfind_result == std::string::npos || rfind_result + endings[0].size() != file.size()){
			continue;
		}
		std::string sdf_file = file.substr(0, file.size() - endings[0].size()) + endings[1];
		if(find(files.begin(), files.end(), sdf_file) == files.end()){
			continue;
		}

		if(file.size() > 1000) {
			std::cerr << "File name is too long: \"" << file << "\"\n";
			errors_occured = 1;
			continue;
		}
		char parameter2[1024];
		strcpy(parameter2, file.c_str());
		argv[2] = parameter2;

		pid_t pid = fork();
		AssertCondition(pid >= 0, "Program Error.\n");
		if ( pid == 0 ) { // child process
			return main(3, argv);
		} else if ( pid < 0 ) {
			return 1;
		}
	}
	return errors_occured;
}

} // namespace highprecisionpso
