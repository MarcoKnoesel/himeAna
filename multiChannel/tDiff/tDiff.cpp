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

#include "tDiff.h"

#include <vector>
#include <iostream>

#include "ProgressIndicator.h"
#include "TRB3RawData.h"
#include "HistogramCollection.h"
#include "TdcSubEvent.h"
#include "TDiffData.h"
#include "Detector.h"
#include "Module.h"
#include "PulseAna.h"

using std::cout;
using std::endl;
using std::array;
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



void tDiff(const char *trb3dir, const char *dir, const char *filename, int trigger, bool write, bool plot){

	// ---------------- Input ----------------

	TString pathInput(TString(trb3dir) + "/data/unpacked/" + TString(dir) + "/" + TString(filename));
	TRB3RawData input(pathInput);

	// ---------------- Output ----------------

	TString pathOutput(TString(trb3dir) + "/data/tDiff/" + TString(dir) + "/" + TString(filename));
	TDiffData output(pathOutput);

	// ---------------- Channel mapping ----------------

	// Create a std::vector of Module objects, where each Module has an ID and two channels.
	// In each channel, there will be sequences of hadaq::MessageFloat objects,
	// representing the signals of a PMT.
	vector<Module> modules = Detector::build("../../data/channelMapping/2024-03-17.csv");
	vector<int> activeChannels = Detector::getActiveChannels(modules);
	std::sort(activeChannels.begin(), activeChannels.end());

	// ---------------- Loop over events ----------------

	const int nEvents = input.getNEvents();
	HistogramCollection hc(activeChannels, nEvents);
	ProgressIndicator pi(nEvents, "[tDiff] Processed events:");

	for(int eventCounter = 0; eventCounter < nEvents; eventCounter++){

		pi.showProgress(eventCounter);
		output.reset();
		vector<vector<MF*>> messagesSortedByChannel = input.getMessagesSortedByChannel(eventCounter);

		// check trigger
		if(trigger > -1){
			if( trigger != input.getTrigger() ) continue;
		}

		hc.fill(messagesSortedByChannel, input.getTrigger(), eventCounter);

		// loop over all modules of the detector
		for(Module& m: modules){
			// get all MessageFloat objects of each PMT
			vector<MF*>& messages_left_up = messagesSortedByChannel[m.getChLeftUp()];
			vector<MF*>& messages_right_down = messagesSortedByChannel[m.getChRightDown()];

			// if a rising signal is found that is followed by a falling signal,
			// the respective time stamps are written to these arrays
			array<float,2> timeStamps_left_up;
			array<float,2> timeStamps_right_down;

			// if both PMTs have a rising signal that is followed by a falling one,
			// we have a complete hit in the current module,
			// which has information about position and energy deposition
			// of the particle that has hit the detector
			if(PulseAna::findPulse(messages_left_up, timeStamps_left_up) &&	PulseAna::findPulse(messages_right_down, timeStamps_right_down)){
				// time difference of the two rising edges
				float tDiff = timeStamps_right_down[0] - timeStamps_left_up[0];
				output.tDiff.push_back(tDiff);
				// sum of the time stamps of the two rising edges
				output.tSum.push_back(timeStamps_right_down[0] + timeStamps_left_up[0]);
				// ToT of the PMT on the left/top side
				float tot_left_up = timeStamps_left_up[1] - timeStamps_left_up[0];
				output.tot0.push_back(tot_left_up);
				// ToT of the PMT on the right/bottom side
				float tot_right_down = timeStamps_right_down[1] - timeStamps_right_down[0];
				output.tot1.push_back(tot_right_down);
				// ID of the module that was hit
				output.moduleID.push_back(m.getID());
				// count the number of hits in this event
				output.nHits++;
				// fill histograms that are required for the further analysis,
				// such as position calibration and gain matching
				m.fillHistograms(tDiff, tot_left_up, tot_right_down);
			}
		}

		output.fill();
	}
	
	output.write();
	hc.write(output.getFile());
	for(const Module& m: modules) m.write(output.getFile());
}
