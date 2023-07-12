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
#include <iostream>
using std::cout;
using std::endl;
using std::vector;



Input::Input(TString directory, const std::vector<const char*>& filenames){
	if( !directory.EndsWith("/") ) directory.Append("/");
	cout << "[Input] Reading directory " << directory.Data() << " ..." << endl;
	this->directory = directory;
	this->filenames = filenames;
	files = vector<TFile*>(filenames.size());
}



TFile* Input::getFile(int run, bool verbose){
	if(verbose) cout << "[Input] Reading file " << filenames[run] << endl;
	files[run] = new TFile(directory + filenames[run], "read");
	return files[run];
}



void Input::closeFile(int run){
	files[run]->Close();
}