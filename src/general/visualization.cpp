/**
* @file   visualization.cpp
* @author Alexander Raß (alexander.rass@fau.de)
* @date   July, 2013
* @brief  This file contains functionality to visualize the particle swarm.
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

#include "general/visualization.h"

#include <cmath>
#include <gmp.h>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <vector>

#include "function/function.h"
#include "general/check_condition.h"
#include "general/configuration.h"
#include "arbitrary_precision_calculation/operations.h"
#include "general/particle.h"
#include "neighborhood/neighborhood.h"
#include "statistics/statistics.h"

namespace highprecisionpso {
namespace visualization {

void VisualizeCurrentSwarm(){
	static std::vector<std::vector<double> > footprints;
	unsigned int maximal_footprint_size = configuration::g_debug_swarm_global_attractor_trajectory;
	static int last_change = 0;
	static std::vector<std::vector<std::vector<double> > > recent_limits;
	static std::vector<std::vector<double> > current_limits;
	static bool initialized = false;
	int resolution_x = configuration::g_debug_swarm_x_resolution;
	int resolution_y = configuration::g_debug_swarm_y_resolution;
	unsigned int remember_limits = 4;
	unsigned int output_precision = 14;
	unsigned int image_generation_frequency = configuration::g_debug_swarm_image_generation_frequency;
	int dbg_id = -1;
	bool values_out_of_range = false;
	bool bounds_out_of_range = false;
	std::vector<std::vector<double> > bounds;
	{
		std::vector<mpf_t*> tmp = configuration::g_function->GetLowerSearchSpaceBound();
		bounds.push_back(vectoroperations::MpftToDouble(tmp));
		vectoroperations::ReleaseValues(tmp);
		tmp = configuration::g_function->GetUpperSearchSpaceBound();
		bounds.push_back(vectoroperations::MpftToDouble(tmp));
		vectoroperations::ReleaseValues(tmp);
		for(auto v: bounds)for(auto a: v){
			if(std::isnan(a) || !std::isfinite(a)){
				bounds_out_of_range = true;
			}
		}
	}
	if(!initialized){
		initialized = true;
		current_limits = bounds;
		for(unsigned int d = 0; d < current_limits[0].size(); d++){
			double center = current_limits[0][d] + (current_limits[1][d] - current_limits[0][d]) * 0.5;
			current_limits[0][d] = (current_limits[0][d] - center) * 2.0 + center;
			current_limits[1][d] = (current_limits[1][d] - center) * 2.0 + center;
		}
		bool limits_invalid = false;
		for(auto v : current_limits)for(auto a: v) {
			if(std::isnan(a) || !std::isfinite(a)) {
				limits_invalid = true;
			}
		}
		if(limits_invalid){
			current_limits = std::vector<std::vector<double> >(2, std::vector<double>(bounds[0].size(), 0.0));
		}
	}
	std::vector<Particle*>* &swarm = configuration::g_statistics->swarm;
	std::vector<std::vector<double> > positions, velocities, local_attractors, global_attractors;
	std::vector<double> overall_global_attractor;
	for(unsigned int i = 0; i < (*swarm).size(); i++){
		positions.push_back(vectoroperations::MpftToDouble((*swarm)[i]->position));
		velocities.push_back(vectoroperations::MpftToDouble((*swarm)[i]->velocity));
		std::vector<mpf_t*> tmp = (*swarm)[i]->GetLocalAttractorPosition();
		local_attractors.push_back(vectoroperations::MpftToDouble(tmp));
		vectoroperations::ReleaseValues(tmp);
		tmp = configuration::g_neighborhood->GetGlobalAttractorPosition((*swarm)[i]->id);
		global_attractors.push_back(vectoroperations::MpftToDouble(tmp));
		vectoroperations::ReleaseValues(tmp);
	}
	{
		std::vector<mpf_t*> tmp = configuration::g_neighborhood->GetGlobalAttractorPosition();
		overall_global_attractor = vectoroperations::MpftToDouble(tmp);
		vectoroperations::ReleaseValues(tmp);
	}
	for(auto v: positions)for(auto a: v){
		if(std::isnan(a) || !std::isfinite(a)){
			values_out_of_range = true;
		}
	}
	for(auto v: velocities)for(auto a: v){
		if(std::isnan(a) || !std::isfinite(a)){
			values_out_of_range = true;
		}
	}
	for(auto v: local_attractors)for(auto a: v){
		if(std::isnan(a) || !std::isfinite(a)){
			values_out_of_range = true;
		}
	}
	for(auto v: global_attractors)for(auto a: v){
		if(std::isnan(a) || !std::isfinite(a)){
			values_out_of_range = true;
		}
	}
	for(auto a: overall_global_attractor){
		if(std::isnan(a) || !std::isfinite(a)){
			values_out_of_range = true;
		}
	}
	std::vector<std::vector<double> > extremal_values;
	extremal_values.push_back(positions[0]);
	extremal_values.push_back(positions[0]);

	footprints.push_back(overall_global_attractor);
	if(footprints.size() > maximal_footprint_size){
		for(unsigned int p = 1; p < footprints.size(); p++){
			footprints[p-1] = footprints[p];
		}
		footprints.pop_back();
	}
	for(unsigned int p = 0; p < positions.size(); p++){
		for(unsigned int d = 0; d < positions[p].size(); d++){
			extremal_values[0][d] = std::min(extremal_values[0][d], positions[p][d]);
			extremal_values[1][d] = std::max(extremal_values[1][d], positions[p][d]);
			extremal_values[0][d] = std::min(extremal_values[0][d], local_attractors[p][d]);
			extremal_values[1][d] = std::max(extremal_values[1][d], local_attractors[p][d]);
			extremal_values[0][d] = std::min(extremal_values[0][d], global_attractors[p][d]);
			extremal_values[1][d] = std::max(extremal_values[1][d], global_attractors[p][d]);
			extremal_values[0][d] = std::min(extremal_values[0][d], positions[p][d] + velocities[p][d]);
			extremal_values[1][d] = std::max(extremal_values[1][d], positions[p][d] + velocities[p][d]);
		}
	}

	for(auto v: extremal_values)for(auto a: v){
		if(std::isnan(a) || !std::isfinite(a)){
			values_out_of_range = true;
		}
	}
	if(!values_out_of_range){
		for(unsigned int d = 0; d < extremal_values[0].size(); d++){
			if(extremal_values[0][d] < current_limits[0][d]){
				current_limits[0][d] = extremal_values[0][d];
			}
			if(extremal_values[1][d] > current_limits[1][d]){
				current_limits[1][d] = extremal_values[1][d];
			}
		}

		for(unsigned int p = 0; p < footprints.size(); p++){
			for(unsigned int d = 0; d < footprints[p].size(); d++){
				current_limits[0][d] = std::min(current_limits[0][d], footprints[p][d]);
				current_limits[1][d] = std::max(current_limits[1][d], footprints[p][d]);
			}
		}

		for(unsigned int d = 0; d < extremal_values[0].size(); d++){
			AssertCondition(extremal_values[0][d] >= current_limits[0][d], "");
			AssertCondition(extremal_values[1][d] <= current_limits[1][d], "");
		}
	}
	recent_limits.push_back(extremal_values);
	if(recent_limits.size() > remember_limits){
		for(unsigned int i = 0; i < remember_limits; i++)recent_limits[i] = recent_limits[i+1];
		recent_limits.pop_back();
	}
	if( configuration::g_debug_swarm_show_global_attractor_on_terminal ){
		std::vector<mpf_t*> tmp_vec = configuration::g_neighborhood->GetGlobalAttractorPosition();
		std::cout << "Step " << configuration::g_statistics->current_iteration << ": x=(";
		for(unsigned int d = 0; d < global_attractors[0].size(); d++){
			std::cout << mpftoperations::MpftToString(tmp_vec[d]) << " ";
		}
		std::cout << "), f(x) ="; 
		mpf_t* tmp = configuration::g_neighborhood->GetGlobalAttractorValue();
		std::cout << mpftoperations::MpftToString(tmp) << std::endl;
		vectoroperations::ReleaseValues(tmp_vec);
		mpftoperations::ReleaseValue(tmp);
	}
	if(image_generation_frequency == 0)return;
	if(configuration::g_statistics->current_iteration % image_generation_frequency != 0)return;
	last_change++;
	if(values_out_of_range){
		std::cerr << "Image generation failed at step " << configuration::g_statistics->current_iteration << ". Values are out of double range.\n";
		return;
	}
	if(recent_limits.size() == remember_limits && last_change > 2){
		for(unsigned int d = 0; d < extremal_values[0].size(); d++){
			double min_val = extremal_values[0][d];
			double max_val = extremal_values[1][d];
			for(unsigned int i = 0; i < recent_limits.size(); i++){
				min_val = std::min(recent_limits[i][0][d], min_val);
				max_val = std::max(recent_limits[i][1][d], max_val);
			}
			for(unsigned int p = 0; p < footprints.size(); p++){
				min_val = std::min(min_val, footprints[p][d]);
				max_val = std::max(max_val, footprints[p][d]);
			}
			double center = 0.5 * (min_val + max_val);
			min_val = (min_val - center) * 1.5 + center;
			max_val = (max_val - center) * 1.5 + center;
			if(min_val == center || max_val == center){
				min_val *= 1-1e-9;
				max_val *= 1+1e-9;
				if(min_val > max_val)std::swap(min_val, max_val);
				if(min_val == center || max_val == center){
					min_val -= 1e-9;
					max_val += 1e-9;
				}
			}
			while(true){
				if(std::isnan(min_val) || !std::isfinite(min_val)
					|| std::isnan(max_val) || !std::isfinite(max_val)) {
					break;
				}
				std::ostringstream os1, os2;
				os1.precision(output_precision);
				os2.precision(output_precision);
				os1 << min_val;
				os2 << max_val;
				std::string s1 = os1.str();
				std::string s2 = os2.str();
				std::string con = s1 + s2;
				bool ok = false;
				for(unsigned int l = 0; l < con.size(); l++){
					if(con[l] > '0' && con[l] <= '9')ok = true;
				}
				if(!ok || (s1 == s2)){
					min_val = (min_val - center) * 2.0 + center;
					max_val = (max_val - center) * 2.0 + center;
				} else {
					break;
				}
			}
			if(!std::isnan(min_val) && std::isfinite(min_val)
					&& !std::isnan(max_val) && std::isfinite(max_val)) {
				if(//min_val > current_limits[0][d] && max_val < current_limits[1][d] &&
						(max_val - min_val) * 2.0 < (current_limits[1][d] - current_limits[0][d])){

					current_limits[0][d] = min_val;
					current_limits[1][d] = max_val;
					last_change = 0;
				}
			}
		}
	}
	for(unsigned int d = 0; d < extremal_values[0].size(); d++){
		AssertCondition(extremal_values[0][d] >= current_limits[0][d], "");
		AssertCondition(extremal_values[1][d] <= current_limits[1][d], "");
	}

	mkdir(configuration::g_file_prefix.c_str(), S_IRWXU | S_IRWXG);
	unsigned int num_pictures = (positions[0].size() + 1) / 2;
	unsigned int picture_rows = 1, picture_columns = 1;
	while(picture_rows * picture_columns < num_pictures) {
		if(picture_rows == picture_columns)++picture_rows;
		else ++picture_columns;
	}
	AssertCondition(configuration::g_dimensions >= 2, "Graphical visualization needs at least two dimensions.");
	std::string step_string;
	{
		{
			std::ostringstream os;
			os << configuration::g_statistics->current_iteration;
			step_string = os.str();
		}
		{
			std::string full_steps;
			std::ostringstream os;
			os << configuration::g_max_iterations;
			full_steps = os.str();
			while(step_string.size() < full_steps.size())step_string = "0" + step_string;
		}
	}
	std::string image_suffix = configuration::g_debug_swarm_image_file_extension;
	std::string gnuplot_terminal = configuration::g_debug_swarm_gnuplot_terminal;
	std::string filename = configuration::g_file_prefix + "/" + step_string + ".gnuplot.txt";
	std::string outfile = step_string + "." + image_suffix;
	std::ofstream file(filename.c_str());
	file.precision(output_precision);
	file << "set terminal " << gnuplot_terminal << " size " << resolution_x << "," << resolution_y << "\n"
		<< "set output \'" << outfile << "\'\n"
		<< "set multiplot layout " << picture_rows << "," << picture_columns << " rowsfirst title \"PSO after " << configuration::g_statistics->current_iteration << " steps\"\n";

	for(int start_dim = 0; start_dim < configuration::g_dimensions; start_dim += 2){
		if(start_dim + 1 == configuration::g_dimensions)--start_dim;
		file << "set xlabel \'dimension " << start_dim << "\'\n"
			<< "set ylabel \'dimension " << start_dim + 1 << "\'\n"
			<< "set xrange [" << current_limits[0][start_dim] << ":" << current_limits[1][start_dim] << "]\n"
			<< "set yrange [" << current_limits[0][start_dim + 1] << ":" << current_limits[1][start_dim + 1] << "]\n";
		int particles = positions.size();
		for(int i = 0; i < particles; i++){
			if(dbg_id != -1 && dbg_id != i)continue;
			file << "set arrow from " << positions[i][start_dim + 0] << "," << positions[i][start_dim + 1] << " to " << positions[i][start_dim + 0] + velocities[i][start_dim + 0] << "," << positions[i][start_dim + 1] + velocities[i][start_dim + 1] << std::endl;
		}
		for(int i = 0; i < particles; i++){
			if(dbg_id != -1 && dbg_id != i)continue;
			file << "set label \"(" << i << ")\" at " << positions[i][start_dim + 0] << "," << positions[i][start_dim + 1] << "\n";
		}
		file << "set grid\n";
		if(!bounds_out_of_range){
			std::vector<std::pair<std::pair<double,double>, std::pair<double, double> > > current_bounds;
			for(int i = 0; i < 2; i++){
				for(int j = 0; j < 2; j++){
					if(i == 0)current_bounds.push_back(std::make_pair(std::make_pair( bounds[i][start_dim] , bounds[j][start_dim + 1]), std::make_pair( bounds[i ^ 1][start_dim] , bounds[j][start_dim + 1])));
					if(j == 0)current_bounds.push_back(std::make_pair(std::make_pair( bounds[i][start_dim] , bounds[j][start_dim + 1]), std::make_pair( bounds[i][start_dim] , bounds[j ^ 1][start_dim + 1])));
				}
			}
			for( auto current_bound: current_bounds ) {
				double x1 = current_bound.first.first;
				double y1 = current_bound.first.second;
				double x2 = current_bound.second.first;
				double y2 = current_bound.second.second;
				if(std::min(x1, x2) > current_limits[1][start_dim])continue;
				if(std::max(x1, x2) < current_limits[0][start_dim])continue;
				if(std::min(y1, y2) > current_limits[1][start_dim + 1])continue;
				if(std::max(y1, y2) < current_limits[0][start_dim + 1])continue;
				x1 = std::max(x1, current_limits[0][start_dim]);
				x2 = std::max(x2, current_limits[0][start_dim]);
				y1 = std::max(y1, current_limits[0][start_dim + 1]);
				y2 = std::max(y2, current_limits[0][start_dim + 1]);
				x1 = std::min(x1, current_limits[1][start_dim]);
				x2 = std::min(x2, current_limits[1][start_dim]);
				y1 = std::min(y1, current_limits[1][start_dim + 1]);
				y2 = std::min(y2, current_limits[1][start_dim + 1]);
				file << "set arrow from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " nohead\n";
			}
		}
		file << "plot ";
		file << "  \'-\' using 1:2 with lines title \'best global attractor footprints\'";
		file << ", \'-\' using 1:2 title \'positions\'";
		file << ", \'-\' using 1:2 title \'local attractors\'";
		file << ", \'-\' using 1:2 title \'global attractors\'";
		file << "\n";
		for(unsigned int i = 0; i < footprints.size(); i++){
			if(dbg_id != -1 && dbg_id != (int)i)continue;
			file << footprints[i][start_dim + 0] << " " << footprints[i][start_dim + 1] << "\n";
		}
		file << "e\n";
		for(int i = 0; i < particles; i++){
			if(dbg_id != -1 && dbg_id != i)continue;
			AssertCondition(((int)positions[i].size()) == configuration::g_dimensions, "The number of dimensions is inconsistent.");
			file << positions[i][start_dim + 0] << " " << positions[i][start_dim + 1] << "\n";
		}
		file << "e\n";
		for(int i = 0; i < particles; i++){
			if(dbg_id != -1 && dbg_id != i)continue;
			file << local_attractors[i][start_dim + 0] << " " << local_attractors[i][start_dim + 1] << "\n";
		}
		file << "e\n";
		for(int i = 0; i < particles; i++){
			if(dbg_id != -1 && dbg_id != i)continue;
			file << global_attractors[i][start_dim + 0] << " " << global_attractors[i][start_dim + 1] << "\n";
		}
		file << "e\n";
		file << "unset label\n";
		file << "unset arrow\n";
	}
	file.close();
	// generate script:
	{
		std::string script_file_name = configuration::g_file_prefix + "/make_image_files.sh";
		{
			std::ifstream script_file(script_file_name);
			if(script_file.good()){
				script_file.close();
				return;
			}
			script_file.close();
		}
		std::ofstream script_file(script_file_name);
		script_file << "#!/bin/bash\n"
			<< "for file in *.gnuplot.txt ; do\n"
			<< "	if [ -f \"$file\" ]; then\n"
			<< "		gnuplot $file\n"
			<< "	fi\n"
			<< "done\n";
		script_file.close();
		chmod(script_file_name.c_str(), S_IRWXU | S_IRWXG);
	}
}

}
} // namespace highprecisionpso
