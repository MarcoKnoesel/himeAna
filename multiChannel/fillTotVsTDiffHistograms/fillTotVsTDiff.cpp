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

#include "fillTotVsTDiff.h"

#include <iostream>

#include "ProgressIndicator.h"
#include "Helpers.h"
#include "Constants.h"
#include "TH2F.h"
#include "TROOT.h"

using std::cout;
using std::endl;
using std::array;
using std::vector;
using std::to_string;

//     _______________
//     | .       ~   |
//     |  HIME ~   . |
//     | .~  rocks  .|
//     |_____________|
//            |
//      |\/\  |
//      (^.^) |
//     ,/   \-o
//  ----------------------------  



void fillTotVsTDiff(const char *trb3dir, const char *subdir, const char *filename){

	// ---------------- Input ----------------
	
	TString pathIn = TString(trb3dir) + "/data/tDiff/" + TString(subdir) + "/" + TString(filename); 
	TDiffData input(pathIn);
	const int nEvents = input.getNEvents();

	
	// ---------------- Create histograms ----------------

	std::vector<TH2F*> hVecTotVsTDiff(Constants::nModules);
	for(int i = 0; i < hVecTotVsTDiff.size(); i++){
		hVecTotVsTDiff[i] = new TH2F(TString("hTotVsTDiff_") + to_string(i).data(), "", 240, -30, 30, 100, 0, 50);
	}


	// ---------------- First loop over events: Fill histograms ----------------
	ProgressIndicator pi(nEvents, "[fillTotVsTDiff] Filling histograms. Processed events:");

	for(int eventCounter = 0; eventCounter < nEvents; eventCounter++){
		
		pi.showProgress(eventCounter);
		input.getEvent(eventCounter);

		// look at coincidences of modules?
		//if( !Helpers::coincidence(input, 0, 1) ) continue;

		for(int hit = 0; hit < input.nHits; hit++){
			
			float tot = sqrt(input.getTot0(hit) * input.getTot1(hit));
			int mod = input.getModuleID(hit);
			float tDiff = input.getTDiff(hit);
			
			// fill correlation plot showing ToT vs. tDiff
			hVecTotVsTDiff[mod]->Fill(tDiff, tot);
		}

	}


	// ---------------- Output ----------------
	TString pathOut = TString(trb3dir) + "/data/totVsTDiffHistograms/" + TString(subdir) + "/" + TString(filename);
	cout << "[fillTotVsTDiff] Writing file " << pathOut.Data() << endl;
	TFile *fileOut = new TFile(pathOut, "recreate");
	for(TH2F* h : hVecTotVsTDiff) h->Write();

	// quit root
	gROOT->ProcessLine(".q");
}