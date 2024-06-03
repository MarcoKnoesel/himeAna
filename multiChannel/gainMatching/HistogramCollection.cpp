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
#include "Convert.h"
#include "Constants.h"
#include <iostream>
using std::cout;
using std::endl;
using std::array;
using std::vector;



HistogramCollection::HistogramCollection(const vector<float>& voltages, float desiredToT){

	// initialize histograms showing general information about the gain-matching procedure 
	// for all modules
	hVecNHits = vector<TH1F>(voltages.size());
	histogramsFitPar.fill(vector<TH1F>(Constants::nChTot));

	vector<TString> parameter_str = {"height", "position", "width"};
	vector<TString> parameter_unit = {"", "(ns)", "(ns)"};
	vector<TString> gainFit_parameter_unit = {"(V)", "(V/ns)", "\\mbox{(V/ns}^2\\mbox{)}"};

	// create histogram names and axis titles containing the module IDs
	// and the voltages that have been set in the measurements
	for(int iVoltage = 0; iVoltage < voltages.size(); iVoltage++){

			TString voltage_str = Convert::toStr(voltages[iVoltage]);
			TString hNHitsName("hNHits" + voltage_str + "V");
			hVecNHits[iVoltage] = TH1F(hNHitsName, "", Constants::nChTot, 0, Constants::nChTot);
			hVecNHits[iVoltage].GetXaxis()->SetTitle("Module ID");
			hVecNHits[iVoltage].GetYaxis()->SetTitle("Number of hits");


		for(int iPar = 0; iPar < 3; iPar++){
			// parameters of the Gaussian muon-peak fits
			TString name("h_" + voltage_str + "V_" + parameter_str[iPar]);
			TString title("Fit parameter " + parameter_str[iPar] + " for " + voltage_str + " V");
			TString yAxisTitle("Fit parameter " + parameter_str[iPar] + " " + parameter_unit[iPar]);

			histogramsFitPar[iPar][iVoltage] = TH1F(name, title, Constants::nChTot, 0, Constants::nChTot);
			histogramsFitPar[iPar][iVoltage].GetXaxis()->SetTitle("Module ID");
			histogramsFitPar[iPar][iVoltage].GetYaxis()->SetTitle(yAxisTitle);
		}
	}

	for(int iPar = 0; iPar < 3; iPar++){
		// parameters of the fits for voltage vs. ToT
		TString name("hGainFitParameter" + Convert::toStr(iPar));
		TString title("Gain-fit parameter " + Convert::toStr(iPar));
		TString yAxisTitle("Gain-fit parameter " + Convert::toStr(iPar) + " " + gainFit_parameter_unit[iPar]);

		histogramsGainFitPar[iPar] = TH1F(name, title, Constants::nChTot, 0, Constants::nChTot);
		histogramsGainFitPar[iPar].GetXaxis()->SetTitle("Module ID");
		histogramsGainFitPar[iPar].GetYaxis()->SetTitle(yAxisTitle);
	}

	hTargetVoltages = TH1F("hTargetVoltages", "Voltages required for a ToT of " + Convert::toStr(desiredToT) + " ns", Constants::nChTot, 0, Constants::nChTot);
	hTargetVoltages.GetXaxis()->SetTitle("Module ID");
	hTargetVoltages.GetYaxis()->SetTitle("Voltage (V)");
}



void HistogramCollection::fill(const vector<Module>& modules){

	for(const Module& m: modules){

		// get target voltages from the fit result of the gain-matching TGraph objects

		for(int pmt = 0; pmt < 2; pmt++){

			int bin = m.getChannels()[pmt] + 1;
			
			hTargetVoltages.SetBinContent(bin, m.getTargetVoltages()[pmt]);

			// iterate over the voltages that have been set for the different measurements
			for(int iVoltage = 0; iVoltage < m.hVecTot[pmt].size(); iVoltage++){

				double nEntries = m.hVecTot[pmt][iVoltage].GetEntries();

				// skip ToT spectra without hits
				if(!nEntries) continue;
			
				hVecNHits[iVoltage].SetBinContent(bin, nEntries);

				// 3 fit parameters of the Gaussian ToT fits
				for(int iPar = 0; iPar < 3; iPar++){

					histogramsFitPar[iPar][iVoltage].SetBinContent(bin, m.fits[pmt][iVoltage]->GetParameter(iPar));
				}
			}

			// parameters of the fit for (ToT, voltage) data points
			for(int iPar = 0; iPar < 3; iPar++){

				if(iPar >= m.gainFits[pmt]->GetNpar()) break;

				double parameterValue = m.gainFits[pmt]->GetParameter(iPar);

				if(parameterValue < -1000. || parameterValue > 1000.){
					cout << "[HisogramCollection] \e[1m\e[33mWarning:\e[0m The value " + Convert::toStr(parameterValue);
					cout << " of parameter " + Convert::toStr(iPar) + " of channel " << m.getChannels()[pmt];
					cout << " is outside of the range [-1000., 1000] and will be set to 0 in the corresponding histogram." << endl;
					parameterValue = 0.;
				}
				histogramsGainFitPar[iPar].SetBinContent(bin + 1, parameterValue);
			}
		}
	}
}



void HistogramCollection::write(TFile* f) const {
	for(const TH1F& h: hVecNHits) h.Write();
	for(const TH1F& h: histogramsGainFitPar) h.Write();
	for(const vector<TH1F>& hVec: histogramsFitPar){
		for(const TH1F& h: hVec) h.Write();
	}
	hTargetVoltages.Write();
}