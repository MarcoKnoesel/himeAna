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

using std::vector;
using MF = hadaq::MessageFloat;

HistogramCollection::HistogramCollection(const vector<int>& activeChannels, int nEvents){

	this->fActiveChannels = activeChannels;
	fInvNEvents = 1. / nEvents;
	const int nCh = Constants::nChTot;

	hChannels   = TH1F("hChannels", "TDC channels", nCh, 0, nCh);
	hTimeStamps = TH1F("hTimeStamps", "Time stamps", 600, -300, 300);
	hTrigger    = TH1F("hTrigger", "Trigger signals", 10, 0, 10);
	hChVsEvtNr  = TH2F("hChVsEvtNr", "Number of messages from each channel in dependence on event number", 100, 0, 100, nCh, 0, nCh);
	hNMessages  = TH1F("hNMessages", "Number of messages per event", 50, 0, 50);
	hChCorr     = TH2F("hChCorr", "Number of hits in each pair of channels", Constants::nChTot, 0, Constants::nChTot, Constants::nChTot, 0, Constants::nChTot);
	hChannels.GetXaxis()->SetTitle("Channel number");
	hTimeStamps.GetXaxis()->SetTitle("Time (ns)");
	hTrigger.GetXaxis()->SetTitle("Tigger (TrbNet Type)");
	hChVsEvtNr.GetXaxis()->SetTitle("Event number (%)");
	hChVsEvtNr.GetYaxis()->SetTitle("Channel number");
	hNMessages.GetXaxis()->SetTitle("Number of messages");
	hChCorr.GetXaxis()->SetTitle("Channel number");
	hChCorr.GetYaxis()->SetTitle("Channel number");
}



void HistogramCollection::fill(const vector<vector<MF*>>& messagesSortedByChannel, int trigger, int eventCounter){
	// fill histograms showing general information about the hadaq::MessageFloat objects
	// that were recorded in this run
	int nMessages = 0;
	for(int ch: fActiveChannels){
		const vector<MF*>& messages = messagesSortedByChannel[ch];
		nMessages += messages.size();
		hChannels.Fill(ch, messages.size());
		hChVsEvtNr.Fill(100. * eventCounter * fInvNEvents, ch, messages.size());
		for(const MF* mf: messages){
			hTimeStamps.Fill(mf->getStamp());
		}
	}
	hNMessages.Fill(nMessages);
	hTrigger.Fill(trigger);

	// iterate over all pairs of active channels
	for(int i = 0; i < fActiveChannels.size(); i++){
		for(int j = i; j < fActiveChannels.size(); j++){
			int ch0 = fActiveChannels[i];
			int ch1 = fActiveChannels[j];
			// fill the histogram, if there was at least one hit in both channels
			if(messagesSortedByChannel[ch0].size() > 0 && messagesSortedByChannel[ch1].size() > 0){
				hChCorr.Fill(ch0, ch1);
			}
		}
	}
}




void HistogramCollection::write(TFile* f){

	f->cd();
	hChannels.Write();
	hTimeStamps.Write();
	hTrigger.Write();
	hChVsEvtNr.Write();
	hNMessages.Write();
	hChCorr.Write();
}