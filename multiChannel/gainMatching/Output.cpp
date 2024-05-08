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

#include "Output.h"
#include "Convert.h"
#include "TFile.h"
#include <cmath>
#include <fstream>
#include <iostream>
using std::ofstream;
using std::cout;
using std::endl;



TFile* Output::create(TString outputSubdir, const std::vector<Module>& modules){

	// open the CSV file 
	if(!outputSubdir.EndsWith("/")){
		outputSubdir += "/";
	}
	TString path("../../data/gainMatching/" + outputSubdir + "voltages.csv");
	cout << "[Output] Writing file " << path << endl;
	ofstream ofs(path, std::ofstream::out);
	
	// at first, write a comment that says what data is listed in this file
	ofs << "# module ID, voltage (V)\n";
	
	for(const Module& m: modules){

		// skip modules that have no counts
		if(!m.gotHits()) continue;

		// skip modules where the voltage could not be calculated properly
		if(!m.getTargetVoltage()) continue;
		
		ofs << Convert::toStr(m.getID()) << "," << Convert::toStr((int) std::lround(m.getTargetVoltage())) << "\n";
	}

	// create a ROOT file for histograms
	path = TString("../../data/gainMatching/" + outputSubdir + "histograms.root");
	cout << "[Output] Writing file " << path << endl;
	return new TFile(path, "recreate");
}

