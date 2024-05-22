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

#include "Module.h"
#include "Convert.h"
using std::vector;
using std::array;

#include <iostream>
using std::cout;
using std::endl;

Module::Module(){
	id = -1;
}

Module::Module(int id, int chLeft, int chRight, vector<float> voltages, bool linearGainFitModel){

	this->id = id;
	this->voltages = voltages;
	this->channels = {chLeft, chRight};
	hVecTotVsTDiff = vector<TH2F>(voltages.size()); 
	for(vector<TH1F>& hVec: hVecTot) hVec = vector<TH1F>(voltages.size());
	for(vector<TF1*>& fVec: fits) fVec = vector<TF1*>(voltages.size());

	// create Gaussian fits for the cosmic-muon peaks in the ToT spectra
	TString moduleID_str = Convert::toNdigit(id, 3);
	for(int i = 0; i < fits.size(); i++){
		for(int iVoltage = 0; iVoltage < voltages.size(); iVoltage++){
			TString voltage_str = Convert::toStr(voltages[iVoltage]);
			TString name;
			if(i < 2){
				name = "fit_channel" + Convert::toNdigit(channels[i],3) + "_" + voltage_str + "V";
			}
			else{
				name = "fit_module" + moduleID_str + "_" + voltage_str + "V";
			}
			fits[i][iVoltage] = new TF1(name, "[0] * exp( - ([1] - x)*([1] - x) / [2] / [2] )");
		}
	}

	// add name and axis titles for the TGraph object containing the (ToT, voltage) data points
	TString fitFunc_str;
	if(linearGainFitModel){
		fitFunc_str = "[0] + [1] * x";
	}
	else{
		fitFunc_str = "[0] + [1] * x + [2] * x * x";
	}

	// initialize gainGraphs and gainFits for the two PMTs
	for(int pmt = 0; pmt < 2; pmt++){
		TString channel_str(Convert::toNdigit(channels[pmt],3));
		gainGraphs[pmt].SetName("gainGraph_channel" + channel_str);
		gainGraphs[pmt].SetTitle("Gain for channel " + channel_str);
		gainFits[pmt] = new TF1("gainFit_channel" + channel_str, fitFunc_str, 0., 100.);
	}
	// initialize gainGraph and gainFit for the module
	gainFits[2] = new TF1("gainFit_module" + moduleID_str, fitFunc_str, 0., 100.);
	gainGraphs[2].SetName("gainGraph_module" + moduleID_str);
	gainGraphs[2].SetTitle("Gain for module " + moduleID_str);

	// set axes titles for the gainGraphs of the module and its PMTs
	for(int i = 0; i < gainGraphs.size(); i++){
		TGraphErrors& g = gainGraphs[i];
		g.GetXaxis()->SetTitle("ToT (ns)");
		g.GetYaxis()->SetTitle("Voltage (V)");
	}
}



void Module::normalizeToT(){
	for(vector<TH1F>& hVec: hVecTot){
		for(TH1F& h: hVec){
			h.GetYaxis()->SetTitle("Number of hits (normalized)");
			float integral = h.Integral();
			// if the number of entries in the current histogram is 0,
			// skip the normalization
			if(!integral) continue;
			h.Scale(1. / integral);
		}
	}		
}



void Module::fill(int iVoltage, const TH2F* moduleHistogramFromFile, const array<TH1F*,2>& pmtHistogramFromFile){

	// skip modules that didn't record hits
	if(!moduleHistogramFromFile->GetEntries()) return;

	int lastBin = hVecTotVsTDiff[iVoltage].GetXaxis()->GetNbins();
	TH1D* projection = hVecTotVsTDiff[iVoltage].ProjectionY("", 1, lastBin);

	// Add data to already existing and non-empty histograms
	if(hVecTotVsTDiff[iVoltage].GetEntries()){
		hVecTotVsTDiff[iVoltage].Add(moduleHistogramFromFile);
		hVecTot[2][iVoltage].Add(projection);
	}
	// When the first histograms are read (from the tDiff part of himeAna),
	// adapt the binning and the axis ranges.
	// Otherwise, the bin contents could not just be copied.
	else{	
		TString voltage_str = Convert::toStr(voltages[iVoltage]);
		TString moduleID_str = Convert::toNdigit(id, 3);
		int nBinsToT = moduleHistogramFromFile->GetYaxis()->GetNbins();
		int nBinsTDiff = moduleHistogramFromFile->GetXaxis()->GetNbins();
		double totMin  = moduleHistogramFromFile->GetYaxis()->GetXmin();
		double totMax = moduleHistogramFromFile->GetYaxis()->GetXmax();
		double tDiffMin = moduleHistogramFromFile->GetXaxis()->GetXmin();
		double tDiffMax = moduleHistogramFromFile->GetXaxis()->GetXmax();
		TString name, title;

		name = "hTotVsTDiff" + moduleID_str + "_" + voltage_str + "V";
		title = "ToT vs. TDiff for module " + moduleID_str + " at " + voltage_str + " V";
		hVecTotVsTDiff[iVoltage] = TH2F(name, title, nBinsTDiff, tDiffMin, tDiffMax, nBinsToT, totMin, totMax);
		hVecTotVsTDiff[iVoltage].GetXaxis()->SetTitle("\\Delta t \\;\\mbox{(ns)}");
		hVecTotVsTDiff[iVoltage].GetYaxis()->SetTitle("ToT (ns)");

		name = "hTot" + moduleID_str + "_" + voltage_str + "V";
		title = "ToT for module " + moduleID_str;
		hVecTot[2][iVoltage] = TH1F(name, title, nBinsToT, totMin, totMax);
		hVecTot[2][iVoltage].GetXaxis()->SetTitle("ToT (ns)");
		hVecTot[2][iVoltage].GetYaxis()->SetTitle("Number of hits");
		
		copy(moduleHistogramFromFile, hVecTotVsTDiff[iVoltage]);
		copy(projection, hVecTot[2][iVoltage]);
	} 

	// iterate over left and right PMT of this module
	for(int pmt = 0; pmt < 2; pmt++){
		// Same as above: check if the current histogram has been created already.
		// If so, add counts to the existing ones;
		// otherwise, initialze the histogram at first.
		if(hVecTot[pmt][iVoltage].GetEntries()){
			hVecTot[pmt][iVoltage].Add(pmtHistogramFromFile[pmt]);
		}
		else{
			TString voltage_str = Convert::toStr(voltages[iVoltage]);
			TString channel_str = Convert::toNdigit(channels[pmt], 3);
			int nBinsToT = pmtHistogramFromFile[pmt]->GetXaxis()->GetNbins();
			double totMin  = pmtHistogramFromFile[pmt]->GetXaxis()->GetXmin();
			double totMax = pmtHistogramFromFile[pmt]->GetXaxis()->GetXmax();
			TString name, title;

			name = "hTotCh" + channel_str + "_" + voltage_str + "V";
			title = "ToT for channel " + channel_str;
			hVecTot[pmt][iVoltage] = TH1F(name, title, nBinsToT, totMin, totMax);
			hVecTot[pmt][iVoltage].GetXaxis()->SetTitle("ToT (ns)");
			hVecTot[pmt][iVoltage].GetYaxis()->SetTitle("Number of hits");
			
			copy(pmtHistogramFromFile[pmt], hVecTot[pmt][iVoltage]);
		}
	}
}



void Module::addPoint(int iVoltage, const std::array<std::array<double,2>,3>& peakPositionsAndUncertainties){
	// The ID of the next point is given by the number of point that are already in the TGraph.
	// This number is not used anywhere in the gain-matching procedure, 
	// but it's required as a parameter in TGraph::SetPoint
	for(int i = 0; i < gainGraphs.size(); i++){
		int point = gainGraphs[i].GetN();
		gainGraphs[i].SetPoint(point, peakPositionsAndUncertainties[i][0], voltages[iVoltage]);
		gainGraphs[i].SetPointError(point, peakPositionsAndUncertainties[i][1], 0.);
	}
}



bool Module::gotHits() const{
	for(const vector<TH1F>& hVec: hVecTot){
		for(const TH1F& h: hVec){
			if(h.GetEntries()){
				return true;
			}
		}
	}
	return false;
}




float Module::findLargestEntry(const std::vector<float>& v) const {
	
	if(!v.size()) return 0.;
	
	float max = v[0];
	
	for(int i = 1; i < v.size(); i++){
		if(v[i] > max) max = v[i];
	}

	return max;
}



float Module::findSmallestEntry(const std::vector<float>& v) const {
	
	if(!v.size()) return 0.;
	
	float min = v[0];
	
	for(int i = 1; i < v.size(); i++){
		if(v[i] < min) min = v[i];
	}

	return min;
}



void Module::copy(const TH2F* hSource, TH2F& hTarget) const {
	for(int binX= 1; binX <= hSource->GetXaxis()->GetNbins(); binX++){
		for(int binY = 1; binY <= hSource->GetYaxis()->GetNbins(); binY++){
			hTarget.SetBinContent(binX, binY, hSource->GetBinContent(binX, binY));
		}
	}
}