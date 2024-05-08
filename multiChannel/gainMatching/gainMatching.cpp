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

#include "gainMatching.h"
#include "ProgressIndicator.h"
#include "TDiffData.h"
#include "Module.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TFile.h"
#include "Output.h"
#include "SpectrumAna.h"
#include "Constants.h"
#include "Convert.h"
#include "HistogramCollection.h"
#include "Drawer.h"
#include <string>
#include <iostream>
using std::vector;
using std::string;
using std::cout;
using std::endl;



void gainMatching(const char* trb3dir, std::vector<float> voltages, std::vector<const char*> subdirs, std::vector<std::vector<const char*>> allFiles, float desiredToT, bool linearGainFitModel, const char* outputSubdir){

	// Check if the number of given voltages matches the number of subdirectories
	if(voltages.size() != subdirs.size()){
		cout << "[gainMatching] The number of listed voltages does not match the number of subdirs!\n";
		cout << "voltages.size() = " << voltages.size() << "\n";
		cout << "subdirs.size() = " << subdirs.size() << endl;
		return;
	}
	if(voltages.size() != allFiles.size()){
		cout << "[gainMatching] The number of listed voltages does not match the size of allFiles!" << endl;
		return;
	}
	if(subdirs.size() != allFiles.size()){
		cout << "[gainMatching] The number of subdirs does not match the size of allFiles!" << endl;
		return;
	}
	
	// The instances of class Module contain (among other data):
	// - a ToT spectrum for each voltage
	// - a 2D correlation plot showing ToT vs. tDiff for each voltage
	// - a TGraph object correlating the position of the cosmic-muon peak with the voltage applied to the module
	vector<Module> modules(Constants::nModules);
	// A collection of histograms showing general information about the gain-matching procedure
	HistogramCollection hc(voltages, modules.size(), desiredToT);

	for(int moduleID = 0; moduleID < Constants::nModules; moduleID++){
		modules[moduleID] = Module(moduleID, voltages, linearGainFitModel);
	}

	// iterate over all voltages that have been applied to the modules
	// in the measurements where the input data for the gain-matching procedure are from
	for(int iVoltage = 0; iVoltage < voltages.size(); iVoltage++){

		cout << "\n[gainMatching] *** " << voltages[iVoltage] << " V ***\n" << endl;

		// ---------------- Input ----------------
		// import data from the subdirectories of data/tDiff
		TDiffData input(TString(trb3dir) + "/data/tDiff/" + TString(subdirs[iVoltage]), allFiles[iVoltage]);

		input.fillModules(modules, iVoltage);
		
		for(Module& m: modules){
			// check if the current module has counts
			if(m.gotHits()){
				// normalize all ToT spectra so that their integrals are 1
				m.normalizeToT();
				// extract the cosmic-muon peak position in the ToT spectrum for each voltage
				std::array<double,2> peakPositionAndUncertainty = SpectrumAna::fitLastPeak(m, iVoltage);
				// fill a TGraph that correlates the peak position with the voltage
				m.addPoint(iVoltage, peakPositionAndUncertainty);
			}
		}
	}

	cout << endl;

	// Apply a fit to the TGraph (the one that correlates the ToT-peak position with the voltage)
	// and evaluate the fit funtion with the desired ToT value, where the cosmic-muon line should be located. 
	// This results in values for the voltage that need to be set for each module.
	for(Module& m: modules){
		if(m.gotHits()){
			m.fitGainGraph(desiredToT);
		}
	}

	// fill histograms that show general information for all modules
	hc.fill(modules);

	// Create a CSV file including all calculated target voltages,
	// which can be read by PowerSupplyControl
	// Write all histograms to a ROOT file.
	// Note that the Drawer writes TCanvas objects to this file.
	TFile* rootFileOut = Output::create(outputSubdir, modules);

	hc.write(rootFileOut);

	// Show histograms and write TCanvas objects to a file
	Drawer dr(voltages);
	dr.drawOverview(hc);
	for(int i = 0; i < modules.size(); i++){
		if(modules[i].hVecTot[0].GetEntries()){
			dr.drawModule(modules[i], true);
		}
	}
}
