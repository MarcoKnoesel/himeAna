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

#ifndef Module_h
#define Module_h

#include <vector>
#include "TH2F.h"
#include "TF1.h"
#include "TGraphErrors.h"

class Module{
	
	public:

	Module();
	// parameters: module ID and voltages of the measurements
	Module(int id, std::vector<float> voltages, bool linearGainFitModel);
	
	// 2D correlation plot: ToT vs. tDiff
	std::vector<TH2F> hVecTotVsTDiff;
	// ToT spectrum (projection of Module::hVecTotVsTDiff)
	std::vector<TH1F> hVecTot;
	// voltages of the measurements
	std::vector<float> voltages;
	// Gaussian fits for the cosmic-muon peaks
	std::vector<TF1> fits;

	// Result of the gain-matching procedure:
	// voltage vs. ToT of the cosmic-muon line
	// (one data point for each measurement)
	TGraphErrors gainGraph;
	// Fit for Module::gainGraph
	TF1 gainFit;
	
	// normalize the ToT spectra; require integral == 1
	void normalizeToT();
	// fill the histograms with data from the tDiff part of himeAna
	void fill(int iVoltage, const TH2F* histogramFromFile);
	// get the ID of this module
	int getID() const { return id; }
	// get the target voltage for this module (result of the gain-matching procedure)
	float getTargetVoltage() const { return targetVoltage; }
	// add a point to the TGraph containing (ToT, voltage) data points
	void addPoint(int iVoltage, const std::array<double,2>& peakPositionAndUncertainty);
	// fit Module::gainGraph to get a continuous mapping from ToT values onto voltages
	void fitGainGraph(float desiredToT);
	// returns true if any of the ToT spectra of this module has a count and false otherwise
	bool gotHits() const;

	private:

	// ID of this module
	int id;
	// result of the gain-matching procedure
	float targetVoltage;
	// find the largest number in a vector
	float findLargestEntry(const std::vector<float>& v) const;
	// find the smallest number in a vector
	float findSmallestEntry(const std::vector<float>& v) const;
	// copy all bin contents from one TH2F to another 
	void copy(const TH2F* hSource, TH2F& hTarget) const;
	// copy all bin contents from a TH1D to a TH1F
	void copy(const TH1D* hSource, TH1F& hTarget) const;
};

#endif