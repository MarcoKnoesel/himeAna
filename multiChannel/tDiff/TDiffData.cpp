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

#include <iostream>

using std::cout;
using std::endl;
using std::array;
using std::vector;
using std::to_string;

TDiffData::TDiffData(TString path){

	// create TFile and TTree
	cout << "[TDiffData] Writing file " << path.Data() << endl;
	file = new TFile(path, "recreate");
	tree = new TTree("tree", "tree");

	// create branches
	tree->Branch("tDiff", &tDiff);
	tree->Branch("tSum", &tSum);
	tree->Branch("tot0", &tot0);
	tree->Branch("tot1", &tot1);
	tree->Branch("moduleID", &moduleID);
	tree->Branch("nHits", &nHits, "nHits/I");
}



void TDiffData::fill(){
	tree->Fill();
}



void TDiffData::write(){
	file->cd();
	tree->Write();
}



void TDiffData::cdToFile(){
	file->cd();
}



void TDiffData::initVectors(int size){
	tDiff		= vector<float>(size);
	tSum		= vector<float>(size);
	tot0		= vector<float>(size);
	tot1		= vector<float>(size);
	moduleID	= vector<int>(size);
}