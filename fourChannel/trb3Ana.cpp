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

#include "trb3Ana.h"
#include <algorithm>
#include <iostream>
#include "Analysis.h"
#include "TFile.h"
#include "SVGPicture.h"
#include "Drawer.h"
#include "ProgressIndicator.h"
#include "TRBMessageHistograms.h"
#include "Input.h"

using std::cout;
using std::endl;
using std::array;
using std::vector;
using std::to_string;
using MF = hadaq::MessageFloat;



// "main" function
void trb3Ana(vector<const char*> filenames, const char* subdir, const char* path, const char* tdc, const int ch0, const int ch1, const int ch2, const int ch3){

	Input input(filenames, subdir, path, tdc);
	
	vector<Module> modules(2);
	modules[0] = Module(0, ch0, ch1);
	modules[1] = Module(1, ch2, ch3);

	for(Module &m : modules){
		// time difference & ToT vs. time difference
		m.initHistograms(160, -20, 20, 200, 0, 50);
	}


	long nEvents = input.getEntries();
	ProgressIndicator pi(nEvents, "Processed events:");
	TRBMessageHistograms trbHist(nEvents);
	vector<MF>& messages = input.getMessages();

	// loop over events
	for(long eventCounter = 0; eventCounter < nEvents; eventCounter++){

		input.getEvent(eventCounter);
		pi.showProgress(eventCounter);

		// sort messages by time (smaller time stamp comes first)
		std::sort(messages.begin(), messages.end(), Analysis::isEarlier);


		// fill basic histograms
		trbHist.fill(messages, eventCounter);
		
		// clear the vector of pointers referring to the hadaq::MessageFloat objects of the current event
		for(Module& mod : modules){
			for(Channel& ch : mod.channels){
				ch.messages.clear();
				ch.fired = false;
			}
		}

		// sort messages by channel
		Analysis::sortMessagesByChannel(messages, modules);

		for(Module& mod : modules){
			for(Channel& ch : mod.channels){
				// This function will check if both PMTs have seen a signal, 
				// i.e. there is a time stamp for a rising edge which is followed by a time stamp for a falling edge
				// in each of the two channels.
				// Then, the three fields "float time", "float tot" and "bool fired" of the objects of the class "Channel" will be set.
				Analysis::findSignal(ch);
			}
		}

		for(Module& mod : modules){
			if(mod.fired()){
				// If a valid signal was found, fill the histograms for ToT, tDiff and correlations between the two
				// Check if there is a coincidence between modules and if so, 
				// fill the same types of histograms again, but this time showing only coincident events.
				mod.fillHistograms(Analysis::coincidence(modules));
			}
		}
	}

	TFile* fileOut = new TFile("histograms.root", "recreate");
	trbHist.write(fileOut);
	for(const Module& mod : modules) mod.write(fileOut);

	Drawer dr;
	dr.drawOverview(trbHist);
	for(const Module& mod : modules){
		dr.drawModule(mod);
	}

/*
	SVGPicture::removeStats();
	SVGPicture::drawTDiff(modules[0].hTDiff[1]);
	SVGPicture::drawTotVsTDiff(modules[0].hTotVsTDiff[1]);
*/
}
	
	

