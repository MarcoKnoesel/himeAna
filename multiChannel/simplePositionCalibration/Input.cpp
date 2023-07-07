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

#include "Input.h"
#include "Constants.h"
#include <iostream>
using std::cout;
using std::endl;
using std::vector;



Input::Input(const char *trb3dir, const char *subdir, std::vector<const char*> filenames, vector<Module>& modules){

	files = vector<TFile*>(filenames.size());

	for(int iFile = 0; iFile < filenames.size(); iFile++){

		TString pathIn = TString(trb3dir) + "/data/totVsTDiffHistograms/" + TString(subdir) + "/" + TString(filenames[iFile]);
		cout << "[Input] Reading file " << pathIn << endl;
		files[iFile] = new TFile(pathIn, "read");
		
		for(int mod = 0; mod < Constants::nModules; mod++){

			TString histName("hTotVsTDiff_");
			histName += std::to_string(mod).data();
		
			modules[mod].add((TH2F*) files[iFile]->Get(histName));
		}
		if(iFile) files[iFile]->Close();
	}
}