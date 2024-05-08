/*
	HIMEana: Analyze HIME data.
	
	Copyright (C) 2023 Marco Knösel (mknoesel@ikp.tu-darmstadt.de)

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

#ifndef HistogramCollection_h
#define HistogramCollection_h

#include <vector>
#include <array>
#include "TH1F.h"
#include "TFile.h"
#include "Module.h"

class HistogramCollection{
	
	public:
	HistogramCollection(){}
	HistogramCollection(const std::vector<float>& voltages, int nModules, float desiredToT);
	
	// fill histograms
	void fill(const std::vector<Module>& modules);
	// write histograms to a ROOT file
	void write(TFile* f) const;
	
	// Number of hits vs. module ID for all voltages
	std::vector<TH1F> hVecNHits;
	// Fit parameter vs. module ID
	// for all 3 fit parameters of the Gaussian ToT fits
	std::array<std::vector<TH1F>, 3> histogramsFitPar;
	// Fit parameter vs. module ID
	// for all fit parameters of the voltage-vs.-ToT fits
	std::array<TH1F, 3> histogramsGainFitPar;
	// Result of the gain-matching procedure:
	// Target voltage vs. module ID
	TH1F hTargetVoltages;
};

#endif