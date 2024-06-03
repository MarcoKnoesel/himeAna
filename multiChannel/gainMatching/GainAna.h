/*
	HIMEana: Analyze HIME data.
	
	Copyright (C) 2023, 2024 Marco Knösel (mknoesel@ikp.tu-darmstadt.de)

	This file is part of HIMEana.
	
	HIMEana is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	HIMEana is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with HIMEana.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef GainAna_h
#define GainAna_h

#include "Module.h"
#include "HistogramCollection.h"
#include <vector>
#include <array>

namespace GainAna{
	// fit Module::gainGraph to get a continuous mapping from ToT values onto voltages
	void fit(Module& m, float desiredToT);
	// Check if the (ToT, voltage) data points are monotonously increasing.
	// If that's not the case, check if it was decreasing without the data point
	// for the lowest or highest voltage.
	// If so, remove the corresponding point.
	void removeDeviatingPoints(Module& m);
	// Convert an instance of TGraphErrors to a std:vector of std::array's
	std::vector<std::array<double,2>> graphToSortedVector(const TGraphErrors& g);
	// replace all points from pointStart (inclusive) to pointStop (exclusive) of a TGraphErrors by the entries of an vector<array<double,2>>
	void replacePoints(const std::vector<std::array<double,2>>& points, TGraphErrors& g, int pointStart, int pointStop);
	// check if the points from pointStart (inclusive) to pointStop (exclusive) are increasing monotonously 
	bool checkMonotonouslyIncreasing(const std::vector<std::array<double,2>> points, int pointStart, int pointStop);
	// Print all data points of one of the TGraphErrors objects of a module
	// (The TGraphErrors either represents the voltage-vs.-tot data points of one of the PMTs, or of the module itself.
	// In the latter case, the ToT is the combined ToT of the two PMTs.)
	void printGainPoints(const Module& m, int pmtOrModule);
};

#endif