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

#include "HistogramCollection.h"

#include <iostream>
using std::cout;
using std::endl;
using std::map;
using std::pair;



void HistogramCollection::addHistogram(const TObject& obj, const TString& className, const char* name, int run, bool verbose){

	if(verbose) cout << "[HistogramCollection] Found " << className.Data() << " " << name << endl;
	
	try{
		if( className.EqualTo("TH1F") ){
			addHistogram((const TH1F&)obj, name, run);
			return;
		}
		if( className.EqualTo("TH2F") ){
			addHistogram((const TH2F&)obj, name, run);
			return;
		}
	}
	catch(std::out_of_range){
		// This message is printed if a TObject is found which was not present in the *first* run
		cout << "\n[HistogramCollection] Cannot add " << className.Data() << name << endl;
	}
}



void HistogramCollection::addHistogram(const TH1F& h1, const char* name, int run){

	if(run){
		try{
			h1map.at(name).Add(&h1);
		}
		catch(std::out_of_range){
			cout << "[HistogramCollection] Cannot add TH1F " << name << endl;
		}
	}
	else h1map.insert(pair<TString, TH1F>(name, h1));
}



void HistogramCollection::addHistogram(const TH2F& h2, const char* name, int run){

	if(run){
		try{
			h2map.at(name).Add(&h2);
		}
		catch(std::out_of_range){
			cout << "[HistogramCollection] Cannot add TH2F " << name << endl;
		}
	}
	else h2map.insert(pair<TString, TH2F>(name, h2));
}



void HistogramCollection::write(TFile* file) const {

	file->cd();
	for(const pair<TString, TH1F>& pair: h1map) pair.second.Write();
	for(const pair<TString, TH2F>& pair: h2map) pair.second.Write();
}