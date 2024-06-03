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

#include "Input.h"
#include <iostream>
using std::vector;
using std::cout;
using std::endl;



Input::Input(std::vector<const char*> filenames, const char* subdir, const char* path, const char* tdc){
	chain = new TChain("T");
	if(TString(path).EndsWith(".root")){
		cout << "[Input] Reading single file " << path << endl;
		chain->AddFile(path);
	}
	else{
		cout << "[Input] Number of files found: " << filenames.size() << "\n";
		for(const TString f : filenames){
			TString completePath("../data/unpacked/");
			completePath += subdir + TString("/") + f;
			cout << "[Input] Reading file " << completePath.Data() << "\n";
			chain->AddFile(completePath);
		}
		cout << std::flush;
	}
	int nEvents = chain->GetEntries();
	cout << "[Input] " << nEvents << " events" << endl;
	messages = nullptr;
	chain->SetBranchAddress(tdc, &messages);
}



void Input::getEvent(long event){
	chain->GetEvent(event);
}



long Input::getEntries() const {
	return chain->GetEntries();
}



std::vector<hadaq::MessageFloat>& Input::getMessages(){
	return *messages;
}