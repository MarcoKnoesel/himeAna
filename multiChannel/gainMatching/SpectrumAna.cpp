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

#include "SpectrumAna.h"
#include "TF1.h"



int SpectrumAna::findLastPeak(const TH1F& h){

	float maximum = h.GetMaximum();
	
	for(int bin = h.GetXaxis()->GetNbins(); bin >=3; bin--){

		// ignore noise and empty bins
		float binContent = h.GetBinContent(bin);
		if(binContent < 0.1 * maximum) continue;

		// this defines what a peak should look like
		if(threePreceedingBinsHaveSmallerContent(h, bin)){
			return bin;
		}
	}

	return 0;
}



bool SpectrumAna::threePreceedingBinsHaveSmallerContent(const TH1F& h, int currentBin){

	float currentBinContent = h.GetBinContent(currentBin);

	for(int compareBin = currentBin - 1; compareBin >= currentBin - 3; compareBin--){

		float compareBinContent = h.GetBinContent(compareBin);
		
		if(compareBinContent >= currentBinContent * 0.99){
			return false;
		}
	}

	return true;
}



std::array<double,2> SpectrumAna::fitLastPeak(Module& m, int iVoltage){
	
	TH1F& h = m.hVecTot[iVoltage];	

	// skip empty histograms
	if(!h.GetEntries()) return {0., 0.};

	// find the start parameters for the fit
	int peakBin = findLastPeak(h);
	float height_estimate = h.GetBinContent(peakBin);
	float position_estimate = h.GetBinCenter(peakBin);
	float width_estimate = 1.;

	// set the start parameters for the fit
	TF1& fit = m.fits[iVoltage];
	fit.SetRange(0.9 * position_estimate, h.GetXaxis()->GetBinUpEdge(h.GetXaxis()->GetNbins()));
	fit.SetParameter(0, height_estimate);
	fit.SetParLimits(0, 0.1 * height_estimate, 10. * height_estimate);
	fit.SetParameter(1, position_estimate);
	fit.SetParameter(2, width_estimate);

	// to make ROOT happy:
	//  -> clone TF1 fit, create a pointer
	//  -> perform fit with the pointer
	//  -> copy result to the original TF1 object
	//  -> delete pointer
	TF1* fitptr = (TF1*) fit.Clone();
	h.Fit(fitptr, "rq0");
	for(int i = 0; i < fit.GetNpar(); i++){
		fit.SetParameter(i, fitptr->GetParameter(i));
		fit.SetParError(i, fitptr->GetParError(i));
	}
	fitptr->Delete();
	// --------------------------------

	// return the position of the peak in the ToT spectrum and its uncertainty
	return {fit.GetParameter(1), fit.GetParError(1)};
}