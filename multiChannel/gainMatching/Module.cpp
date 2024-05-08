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

#include <iostream>
using std::cout;
using std::endl;

Module::Module(){
	id = -1;
	targetVoltage = 0.;
}

Module::Module(int id, vector<float> voltages, bool linearGainFitModel){

	this->id = id;
	targetVoltage = 0.;
	this->voltages = voltages;
	hVecTotVsTDiff = vector<TH2F>(voltages.size()); 
	hVecTot = vector<TH1F>(voltages.size()); 
	fits = vector<TF1>(voltages.size());

	// create Gaussian fits for the cosmic-muon peaks in the ToT spectra
	TString moduleID_str = Convert::toNdigit(id, 3);
	for(int iVoltage = 0; iVoltage < voltages.size(); iVoltage++){
		TString voltage_str = Convert::toStr(voltages[iVoltage]);
		TString name = "fit" + moduleID_str + "_" + voltage_str + "V";
		fits[iVoltage] = TF1("fit", "[0] * exp( - ([1] - x)*([1] - x) / [2] / [2] )");
	}

	// add name and axis titles for the TGraph object containing the (ToT, voltage) data points
	gainGraph.SetName("gainGraph" + moduleID_str);
	gainGraph.GetXaxis()->SetTitle("ToT (ns)");
	gainGraph.GetYaxis()->SetTitle("Voltage (V)");
	if(linearGainFitModel){
		gainFit = TF1("gainFit" + moduleID_str, "[0] + [1] * x", 0., 100.);
	}
	else{
		gainFit = TF1("gainFit" + moduleID_str, "[0] + [1] * x + [2] * x * x", 0., 100.);
	}
}



void Module::normalizeToT(){
	for(TH1F& h: hVecTot){
		h.GetYaxis()->SetTitle("Number of hits (normalized)");
		float integral = h.Integral();
		// if the number of entries in the current histogram is 0,
		// skip the normalization
		if(!integral) continue;
		h.Scale(1. / integral);
	}
}




void Module::fill(int iVoltage, const TH2F* histogramFromFile){

	if(!histogramFromFile->GetEntries()) return;

	int lastBin = hVecTotVsTDiff[iVoltage].GetXaxis()->GetNbins();
	TH1D* projection = hVecTotVsTDiff[iVoltage].ProjectionY("", 1, lastBin);

	// Add data to already existing and non-empty histograms
	if(hVecTotVsTDiff[iVoltage].GetEntries()){
		hVecTotVsTDiff[iVoltage].Add(histogramFromFile);
		hVecTot[iVoltage].Add(projection);
	}
	// When the first histograms are read (from the tDiff part of himeAna),
	// adapt the binning and the axis ranges.
	// Otherwise, the bin contents could not just be copied.
	else{	
		TString voltage_str = Convert::toStr(voltages[iVoltage]);
		TString moduleID_str = Convert::toNdigit(id, 3);
		int nBinsToT = histogramFromFile->GetYaxis()->GetNbins();
		int nBinsTDiff = histogramFromFile->GetXaxis()->GetNbins();
		double totMin  = histogramFromFile->GetYaxis()->GetXmin();
		double totMax = histogramFromFile->GetYaxis()->GetXmax();
		double tDiffMin = histogramFromFile->GetXaxis()->GetXmin();
		double tDiffMax = histogramFromFile->GetXaxis()->GetXmax();
		TString name, title;

		name = "hTotVsTDiff" + moduleID_str + "_" + voltage_str + "V";
		title = "ToT vs. TDiff for module " + moduleID_str + " at " + voltage_str + " V";
		hVecTotVsTDiff[iVoltage] = TH2F(name, title, nBinsTDiff, tDiffMin, tDiffMax, nBinsToT, totMin, totMax);
		hVecTotVsTDiff[iVoltage].GetXaxis()->SetTitle("\\Delta t \\;\\mbox{(ns)}");
		hVecTotVsTDiff[iVoltage].GetYaxis()->SetTitle("ToT (ns)");

		name = "hTot" + moduleID_str + "_" + voltage_str + "V";
		title = "ToT for module " + moduleID_str + " at " + voltage_str + " V";
		hVecTot[iVoltage] = TH1F(name, title, nBinsToT, totMin, totMax);
		hVecTot[iVoltage].GetXaxis()->SetTitle("ToT (ns)");
		hVecTot[iVoltage].GetYaxis()->SetTitle("Number of hits");
		
		copy(histogramFromFile, hVecTotVsTDiff[iVoltage]);
		copy(projection, hVecTot[iVoltage]);
	}
}



void Module::addPoint(int iVoltage, const std::array<double,2>& peakPositionAndUncertainty){
	// The ID of the next point is given by the number of point that are already in the TGraph.
	// This number is not used anywhere in the gain-matching procedure, 
	// but it's required as a parameter in TGraph::SetPoint
	int point = gainGraph.GetN();
	gainGraph.SetPoint(point, peakPositionAndUncertainty[0], voltages[iVoltage]);
	gainGraph.SetPointError(point, peakPositionAndUncertainty[1], 0.);
}



void Module::fitGainGraph(float desiredToT){
	// to make ROOT happy:
	//  -> clone TF1 fit, create a pointer
	//  -> perform fit with the pointer
	//  -> copy result to the original TF1 object
	//  -> delete pointer
	TF1* fitptr = (TF1*) gainFit.Clone();
	fitptr->SetParameter(0, gainGraph.GetPointY(0));
	fitptr->SetParameter(1, 50.);
	fitptr->SetParameter(2, 0.);
	gainGraph.Fit(fitptr, "rq0");
	for(int i = 0; i < gainFit.GetNpar(); i++){
		gainFit.SetParameter(i, fitptr->GetParameter(i));
		gainFit.SetParError(i, fitptr->GetParError(i));
	}
	fitptr->Delete();
	targetVoltage = gainFit.Eval(desiredToT);
	if(targetVoltage < 500 || targetVoltage > 1500){
		cout << "[Module] \e[1m\e[31mError for module " << id << "!\e[0m\n";
		cout << "[Module] The calculated voltage " << Convert::toStr(targetVoltage) << " V is outside of the allowed range [500 V, 1500 V]!" << endl;
		targetVoltage = 0.;
	}
}



bool Module::gotHits() const{
	for(const TH1F& h: hVecTot){
		if(h.GetEntries()){
			return true;
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



void Module::copy(const TH1D* hSource, TH1F& hTarget) const {
	for(int binX= 1; binX <= hSource->GetXaxis()->GetNbins(); binX++){
		hTarget.SetBinContent(binX, hSource->GetBinContent(binX));
	}
}