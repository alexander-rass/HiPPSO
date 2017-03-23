/**
* @file   general/main.h
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

#ifndef HIGH_PRECISION_PSO_GENERAL_MAIN_H_
#define HIGH_PRECISION_PSO_GENERAL_MAIN_H_

#include <string>

#include "general/general_objects.h"
#include "statistics/statistics.h"

/**
* @brief The main starting method to start the PSO program.
*
* @param argc The number of arguments in argv.
* @param argv[] The supplied arguments.
*
* @return 0 if execution was successful and something different otherwise. 
*/
int main(int argc, char* argv[]);

namespace highprecisionpso {

/**
* @brief Describes the current version of this PSO program.
*/
extern const ProgramVersion PSO_PROGRAM_VERSION;

/**
* @brief This method proceeds the activities of the PSO algorithm.
*
* Here the PSO iterations are controlled.  At each iteration the update of each
* position of the particles is started.  The actual update of the global
* attractor is initialized here.  Statistical evaluations, the swarm debugging,
* backups and the check whether the program is allowed to run are started here.
*
* @param statistics The statistics where data about the current PSO execution
* are stored.
*
* @return The final statistic.
*/
Statistics* DoPso(Statistics* statistics);
/**
* @brief This method initializes the particles and starts the swarm.
*
* @return The final statistic.
*/
Statistics* InitAndDoPso();
/**
* @brief Loads information from the backup file and restarts the particle
* swarm.
*
* @return The final statistic.
*/
Statistics* RestoreAndDoPso();
/**
* @brief Writes a backup file with the specified file name and stores there all
* needed data to restart the particle swarm.
*
* @param filename The file name of the backup file.
*/
void WriteCurrentState(std::string filename);

/**
* @brief If run check is activated then This method checks whether the program
* is currently allowed to run.
*
* @return The truth value whether the program is allowed to run.
*/
bool AllowedToRun();
/**
* @brief Stores all data and generates a shutdown file to ease restarting of
* this particle swarm execution.
*/
void Shutdown();
/**
* @brief Restarts all particle swarms which are correctly terminated in the
* folder specified in argument 2 of the supplied char * array. 
*
* @param argv[] The arguments of the initial call of this program.
*
* @return 0 if execution was successful and something different otherwise.
*/
int StartTasks(char *argv[]);

} // namespace highprecisionpso

#endif /* HIGH_PRECISION_PSO_GENERAL_MAIN_H_ */
