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

#include "Drawer.h"
#include "TCanvas.h"
#include "Convert.h"
#include "TStyle.h"
#include <cmath>
using std::vector;



Drawer::Drawer(){}



Drawer::Drawer(const std::vector<float>& voltages){
	this->voltages = voltages;
}



void Drawer::drawOverview(HistogramCollection& hc){

	gStyle->SetOptStat(0);
	
	setColors(hc.hVecNHits);
	
	// Number of hits vs. module ID
	// for each voltage
	TCanvas* can = new TCanvas("can_nhits", "", 1800, 1000);
	can->Divide(3,2);
	can->cd(1);
	setRangeToMax(hc.hVecNHits);
	hc.hVecNHits[0].DrawClone("hist");
	for(int i = 1; i < hc.hVecNHits.size(); i++){
		hc.hVecNHits[i].DrawClone("same hist");
	}
	TLegend lgd = createLegend(hc.hVecNHits);
	lgd.DrawClone();

	// Result of the gain-matching procedure: 
	// target voltages
	can->cd(2);
	hc.hTargetVoltages.GetYaxis()->SetRangeUser(500., 2000.);
	hc.hTargetVoltages.DrawClone("hist");

	// Fit parameter vs. module ID
	// for all fit parameters of the voltage-vs.-ToT fits
	can->cd(3);
	setColors(hc.histogramsGainFitPar);
	removeTitles(hc.histogramsGainFitPar);
	removeYAxisTitles(hc.histogramsGainFitPar);
	setRangeToMinAndMax(hc.histogramsGainFitPar);
	hc.histogramsGainFitPar[0].DrawClone("hist");

	TLegend gainFitParLgd(0.7,0.7,0.9,0.9);
	gainFitParLgd.AddEntry(&(hc.histogramsGainFitPar[0]), "Parameter 0");

	for(int i = 1; i < hc.histogramsGainFitPar.size(); i++){
		// skip empty histograms
		if(!hc.histogramsGainFitPar[i].GetEntries()) continue;
		gainFitParLgd.AddEntry(&(hc.histogramsGainFitPar[i]), "Parameter " + Convert::toStr(i));
		hc.histogramsGainFitPar[i].DrawClone("same hist");
	}
	gainFitParLgd.DrawClone();

	// 3 fit parameters of the Gaussian ToT fits
	for(int iPar = 0; iPar < hc.histogramsFitPar.size(); iPar++){

		can->cd(4 + iPar);
		
		setColors(hc.histogramsFitPar[iPar]);
		setRangeToMax(hc.histogramsFitPar[iPar]);
		removeTitles(hc.histogramsFitPar[iPar]);
		hc.histogramsFitPar[iPar][0].DrawClone("hist");

		for(int iVoltage = 0; iVoltage < hc.histogramsFitPar[iPar].size(); iVoltage++){

			hc.histogramsFitPar[iPar][iVoltage].DrawClone("same hist");
		}

		lgd.DrawClone();
	}

	can->Write();
}



void Drawer::drawModule(Module& m, bool normalize){
	
	gStyle->SetOptStat(0);
	
	TString name("can_module" + Convert::toNdigit(m.getID(), 3));
	TCanvas* can = new TCanvas(name, "", 1800, 1000);
	can->Divide(1, 3);

	// Draw all ToT spectra and the corresponding Gaussian fits for the cosmic-muon lines
	// in the same TPad.
	// -> one spectrum and one fit for each voltage
	TLegend lgd = createLegend(m.hVecTot[0]);

	can->cd(1);
	gPad->Divide(m.hVecTot.size(), 1);

	for(int i = 0; i < m.hVecTot.size(); i++){
		can->cd(1);
		gPad->cd(i+1);
		setColors(m.hVecTot[i]);
		setColorsPtr(m.fits[i]);
		setDashedLinePtr(m.fits[i]);
		// iterate over voltages
		for(int iVoltage = 0; iVoltage < m.hVecTot[i].size(); iVoltage++){
			m.hVecTot[i][iVoltage].GetYaxis()->SetRangeUser(0., 1.1 * findMaximum(m.hVecTot[i]));
			if(iVoltage == 0){
				m.hVecTot[i][iVoltage].DrawClone("hist");
			}
			else{
				m.hVecTot[i][iVoltage].DrawClone("same hist");
			}
			m.fits[i][iVoltage]->Draw("same");
		}
		lgd.DrawClone();
	}

	// Result of the gain-matching procedure:
	// Voltage vs. ToT of the cosmic-muon line.
	// -> TGraph (one data point for each measurement) and TF1 (fit)
	can->cd(2);
	gPad->Divide(m.gainGraphs.size(), 1);

	setColorsPtr(m.gainFits);
	setDashedLinePtr(m.gainFits);

	for(int i = 0; i < m.gainGraphs.size(); i++){
		can->cd(2);
		gPad->cd(i+1);
		m.gainGraphs[i].SetMarkerStyle(3);
		m.gainGraphs[i].GetXaxis()->SetLimits(0., 50.);
		m.gainGraphs[i].GetXaxis()->SetRangeUser(0., 50.);
		m.gainGraphs[i].DrawClone("ap");
		m.gainFits[i]->Draw("same");
	}

	// 2D correlation plots: ToT vs. tDiff
	// iterate over voltages
	can->cd(3);
	gPad->Divide(m.hVecTotVsTDiff.size(), 1);

	for(int i = 0; i < m.hVecTotVsTDiff.size(); i++){
		can->cd(3);
		gPad->cd(i+1);
		m.hVecTotVsTDiff[i].DrawClone("colz");
	}

	can->Write();
}



TLegend Drawer::createLegend(const std::vector<TH1F>& histograms){
	TLegend legend(0.7,0.7,0.9,0.9);
	for(int i = 0; i < voltages.size(); i++){
		legend.AddEntry(&(histograms[i]), Convert::toStr(voltages[i]) + " V");
	}
	return legend;
}