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

/* 
 * Analyze hadaq::MessageFloat objects for all modules to prepare the data for calibration.
 * The results are written to a TTree.
 */

#include "trb3Ana.h"

#include <vector>
#include <iostream>

#include "ProgressIndicator.h"
#include "Drawer.h"
#include "PulseAna.h"
#include "TRB3RawData.h"
#include "MessageAna.h"
#include "Constants.h"
#include "Helpers.h"
#include "TStopwatch.h"
#include "Convert.h"

using std::cout;
using std::endl;
using std::vector;
using MF = hadaq::MessageFloat;

//     _______________
//     | .       ~   |
//     |  HIME ~   . |
//     | .~  rocks  .|
//     |_____________|
//            |
//      |\/\  |
//      (^.^) |
//     ,/   \-o
//  ----------------------------  



void trb3Ana(const char *trb3dir, const char *dir, const char *filename, int trigger, bool write, bool plot){

	/*
	 * Set the number of TDCs and the number of channels per TDC in "Constants.h"!
	 * If set too small, undefined behavior might occur.
	 */

	// ---------------- Input ----------------

	TRB3RawData input(TString(trb3dir) + "/data/unpacked/" + TString(dir) + "/" + TString(filename));
	const int nEvents = input.getNEvents();
	//Detector hime;
	//Detector hime(TString(trb3dir) + "/data/channelMapping/2022-10-11.csv");
	Detector hime(TString(trb3dir) + "/data/channelMapping/2022-11-28.csv");

	// ---------------- Output ----------------

	TDiffData output(TString(trb3dir) + "/data/tDiff/" + TString(dir) + "/" + TString(filename));

	// ---------------- Loop over events ----------------

	ProgressIndicator pi(nEvents, "[trb3Ana] Processed events:");
	HistogramCollection hc(nEvents);
	TStopwatch stopwatch;

	
	for(int eventCounter = 0; eventCounter < nEvents; eventCounter++){

		pi.showProgress(eventCounter);
		hime.clearPulses();
		input.getEvent(eventCounter);

		// check trigger
		if(trigger > -1){
			if( trigger != input.getTrigger() ) continue;
		}
		hc.hTrigger->Fill(input.getTrigger());

		// loop over FPGAs
		int nMessages = 0;


		for(int iTdc = 0; iTdc < Constants::nTdcs; iTdc++){
		
			vector<MF> messages = input.getMessagesOfTdc(iTdc);
			std::sort(messages.begin(), messages.end(), Helpers::isEarlier);
			nMessages += messages.size();
			// fill histograms showing information on hadaq::MessageFloat instances
			MessageAna::fillHistograms(eventCounter, iTdc, messages, hc);
			// convert hadaq::MessageFloat objects to Pulse objects for each channel
			PulseAna::findPulses(input, hime);
		}
		// analyze pulses and write the results to the TTree inside "TDiffData output"
		// and to the ToT-vs.-tDiff histograms of all HIME modules
		PulseAna::evaluate(hime, output);

		hc.hNMessages->Fill(nMessages);
		output.fill();
	}

	cout << "[trb3Ana] Time elapsed:  " << Convert::toStr(stopwatch.RealTime()).Data() << " s" << endl;

	// ---------------- Write data and plot histograms ----------------

	if(write){
		output.write();	// do this at first, because it does "cd" to the output file
		hime.write();
		hc.write();
	}
	if(plot){
		Drawer dr;
		dr.draw1d(hc);
		dr.draw2d(hc);
	}
}
