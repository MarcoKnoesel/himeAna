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

#include "TDiffData.h"
#include "TFile.h"
#include "Convert.h"
#include <iostream>

using std::cout;
using std::endl;
using std::vector;



TDiffData::TDiffData(TString path, vector<const char*> filenames){

	if(!path.EndsWith("/")) path += "/";

	paths = vector<TString>(filenames.size());

	for(int i = 0; i < filenames.size(); i++){
		
		paths[i] = path + filenames[i];
	}
}



void TDiffData::fillModules(vector<Module>& modules, int iVoltage) const {

	for(int iFile = 0; iFile < paths.size(); iFile++){
		
		cout << "[TDiffData] Reading file " << paths[iFile].Data() << endl;
		TFile* f = new TFile(paths[iFile], "read");
			
		for(Module& m : modules){
			TString name = "hTotVsTDiff_module_" + Convert::toNdigit(m.getID(), 3);
			
			TH2F* histogramFromFile = (TH2F*) f->Get(name);

			if(!histogramFromFile) continue;

			m.fill(iVoltage, histogramFromFile);
		}

		f->Close();
		f->Delete();
	}
	return;
}
