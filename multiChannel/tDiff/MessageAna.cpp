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

#include "MessageAna.h"

#include "Constants.h"
#include "HistogramCollection.h"

using std::vector;
using MF = hadaq::MessageFloat;

void MessageAna::fillHistograms(int eventCounter, int iTdc, const vector<MF> &messages, HistogramCollection &hc){

	vector<int> nMessagesForEachChannel(Constants::nChPerTdc,0);
	
	for(int i = 0; i < (int) messages.size(); i++){

		int subCh = ((int) messages[i].getCh()) - 1;
		int ch = subCh + iTdc * Constants::nChPerTdc;

		hc.hChannels->Fill(ch);
		hc.hTimeStamps->Fill(messages[i].getStamp());

		if(messages[i].isRising()){
			hc.hChVsStampRising->Fill(messages[i].getStamp(), ch);
		}
		else{
			hc.hChVsStampFalling->Fill(messages[i].getStamp(), ch);
		}

		// count how often each channel saw a signal in the current event
		nMessagesForEachChannel[subCh]++;
	}

	for(int subCh = 0; subCh < nMessagesForEachChannel.size(); subCh++){
		int ch = subCh + iTdc * Constants::nChPerTdc;
		hc.hChVsEvtNr->Fill(eventCounter, ch, nMessagesForEachChannel[subCh]);
	}
}