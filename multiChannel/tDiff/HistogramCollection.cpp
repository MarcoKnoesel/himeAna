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

#include "HistogramCollection.h"

#include <iostream>
#include "Constants.h"
#include "TFile.h"

using std::cout;
using std::endl;

HistogramCollection::HistogramCollection(int nEvents){

	const int nCh = Constants::nChPerTdc * Constants::nTdcs;

	hChannels 			= new TH1F("hChannels", "TDC channels", nCh, 0, nCh);
	hTimeStamps			= new TH1F("hTimeStamps", "Time stamps", 600, -300, 300);
	hTrigger  			= new TH1F("hTrigger", "Trigger signals", 10, 0, 10);
	hChVsStampRising	= new TH2F("hChVsStampRising", "Rising signals", 200, -100, 100, nCh, 0, nCh);
	hChVsStampFalling	= new TH2F("hChVsStampFalling", "Falling signals", 200, -100, 100, nCh, 0, nCh);
	hChVsEvtNr 			= new TH2F("hChVsEvtNr", "Number of messages from each channel in dependence on event number", 100, 0, nEvents, nCh, 0, nCh);
	hNMessages 			= new TH1F("hNMessages", "Number of messages per event", 50, 0, 50);
	hChannels->GetXaxis()->SetTitle("Channel number");
	hTimeStamps->GetXaxis()->SetTitle("Time (ns)");
	hTrigger->GetXaxis()->SetTitle("Tigger (TrbNet Type)");
	hChVsStampRising->GetXaxis()->SetTitle("Time (ns)");
	hChVsStampRising->GetYaxis()->SetTitle("Channel number");
	hChVsStampFalling->GetXaxis()->SetTitle("Time (ns)");
	hChVsStampFalling->GetYaxis()->SetTitle("Channel number");
	hChVsEvtNr->GetXaxis()->SetTitle("Event number");
	hChVsEvtNr->GetYaxis()->SetTitle("Channel number");
	hNMessages->GetXaxis()->SetTitle("Number of messages");
}



void HistogramCollection::write(){

	hChannels->Write();
	hTimeStamps->Write();
	hTrigger->Write();
	hChVsStampRising->Write();
	hChVsStampFalling->Write();
	hChVsEvtNr->Write();
	hNMessages->Write();
}