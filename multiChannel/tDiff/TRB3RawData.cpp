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

#include "TRB3RawData.h"

#include "Constants.h"

#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using MF = hadaq::MessageFloat;
using HM = hadaq::HldMessage;



TRB3RawData::TRB3RawData(TString path){

	// open file
	cout << "[TRB3RawData] Reading file " << path << endl;

	fFile = new TFile(path, "read");

	if( ! fFile ){
		cout << "file not found" << endl;
		return;
	}

	// extract TTree from opened file
	fTree = (TTree*) fFile->Get("T");

	if( ! fTree ){
		cout << "tree not found" << endl;
		return;
	}

	// find number of events
	fNEvents = fTree->GetEntries();
	cout << "[TRB3RawData] " << fNEvents << " events" << endl;

	// Set branch addresses for messages of all peripheral TDCs
	fTdcNames = vector<TString>(Constants::nTdcs);
	fTdcs = vector<vector<MF>*>(Constants::nTdcs, nullptr);

	for(int i = 0; i < fTdcNames.size(); i++){
		fTdcNames[i] = (*fTree->GetListOfBranches())[i+2]->GetName();
		fTree->SetBranchAddress( fTdcNames[i], &(fTdcs[i]) );
		cout << "[TRB3RawData] Found TDC \"" << fTdcNames[i] << "\"" << endl;
	}

	// Set branch address for run information
	runInfo = nullptr;
	fTree->SetBranchAddress("HLD", &runInfo);
}



int TRB3RawData::getTrigger() const {
	return (int) runInfo->trig_type;
}



void TRB3RawData::getEvent(long i){
	fTree->GetEvent(i);
	for(vector<MF> *messages : fTdcs){
		std::sort(messages->begin(), messages->end(), Helpers::isEarlier);
	}
}



int TRB3RawData::getNEvents() const {
	return fNEvents;
}



vector<MF> TRB3RawData::getMessagesOfTdc(int iTdc) const {
	return *(fTdcs[iTdc]);
}