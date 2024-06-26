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

#include "TRBMessageHistograms.h"

using MF = hadaq::MessageFloat;
using std::vector;



TRBMessageHistograms::TRBMessageHistograms(long nEvents){
	hCh = TH1F("hCh", "Channels", 48, 0, 48);
	hMult = TH1F("hMult", "Number of messages per event", 10, 0, 10);
	hMult.GetXaxis()->SetTitle("Multiplicity");
	hStamps = TH1F("hStamps", "Time stamps", 1200, -600, 600);
	hStamps.GetXaxis()->SetTitle("Time (ns)");
	hChVsEvt = TH2F("hChVsEvt", "Channel stability", 100, 0, nEvents, 16, 0, 16);
	hChVsEvt.GetXaxis()->SetTitle("Event number");
	hChVsEvt.GetYaxis()->SetTitle("Channel number");
}



void TRBMessageHistograms::fill(vector<MF>& messages, long event){

	hMult.Fill(messages.size());
	
	for(const MF& mf : messages){
		hStamps.Fill(mf.getStamp());
		hCh.Fill(mf.getCh() - 1);
		hChVsEvt.Fill(event, mf.getCh() - 1);
	}
}



void TRBMessageHistograms::write(TFile* fileOut) const {
	fileOut->cd();
	hMult.Write();
	hStamps.Write();
	hCh.Write();
	hChVsEvt.Write();
}