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

Module::Module(){
	id = -1;
	channels[0].id = -1;
	channels[1].id = -1;
	channels[0].fired = false;
	channels[1].fired = false;
}

Module::Module(int id, int ch1, int ch2){
	this -> id = id;
	channels[0].id = ch1;
	channels[1].id = ch2;
	channels[0].fired = false;
	channels[1].fired = false;
}

void Module::initHistograms(int nBinsTDiff, float tDiffMin, float tDiffMax, int nBinsToT, float totMin, float totMax){

	TString tDiffTitle("t_{" + Convert::toStr(channels[0].id) + "}- t_{" + Convert::toStr(channels[1].id) + "}\\mbox{ (ns)}");
	TString moduleName("module_" + Convert::toStr(id));
	TString moduleTitle("Module " + Convert::toStr(id));

	for(int iHist = 0; iHist < 2; iHist++){

		TString coinStrName = iHist ? "_coin" : "";
		TString coinStrTitle = iHist ? " (coincidences only)" : "";

		hToT[iHist] = TH1F("hToT_" + moduleName + coinStrName, moduleTitle + ": Combined time over threshold" + coinStrTitle, nBinsToT, totMin, totMax);
		hToT[iHist].GetXaxis()->SetTitle("ToT (ns)");
		hToT[iHist].GetYaxis()->SetTitle("Counts");

		hTDiff[iHist] = TH1F("hDiff_" + moduleName + coinStrName, moduleTitle + ": Time difference" + coinStrTitle, nBinsTDiff, tDiffMin, tDiffMax);
		hTDiff[iHist].GetXaxis()->SetTitle(tDiffTitle);
		hTDiff[iHist].GetYaxis()->SetTitle("Counts");

		hTotVsTDiff[iHist] = TH2F("hToTvsTDiff_" + moduleName + coinStrName, moduleTitle + ": Combined time over threshold vs. time difference" + coinStrTitle, nBinsTDiff, tDiffMin, tDiffMax, nBinsToT, totMin, totMax);
		hTotVsTDiff[iHist].GetXaxis()->SetTitle(tDiffTitle);
		hTotVsTDiff[iHist].GetYaxis()->SetTitle("ToT (ns)");
	}
}



void Module::fillHistograms(bool coincidence){
	
	hToT[0].Fill(getCombinedToT());
	hTDiff[0].Fill(getTDiff());
	hTotVsTDiff[0].Fill(getTDiff(), getCombinedToT());

	if(!coincidence) return;

	hToT[1].Fill(getCombinedToT());
	hTDiff[1].Fill(getTDiff());
	hTotVsTDiff[1].Fill(getTDiff(), getCombinedToT());
}



void Module::write(TFile* fileOut) const {
	fileOut->cd();
	for(int iHist = 0; iHist < 2; iHist++){
		hToT[iHist].Write();
		hTDiff[iHist].Write();
		hTotVsTDiff[iHist].Write();
	}
}