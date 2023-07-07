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

#include "PulseAna.h"

#include "Constants.h"

using std::vector;
using MF = hadaq::MessageFloat;



void PulseAna::findPulses(const TRB3RawData &input, Detector &d){

	d.clearPulses();

	for(int iTdc = 0; iTdc < Constants::nTdcs; iTdc++){
	
		const vector<MF> &messages = input.getMessagesOfTdc(iTdc);
		int chOffs = iTdc * Constants::nChPerTdc;

		// iterate over all messages of the current FPGA
		for(int i = 0; i < (int) messages.size(); i++){

			int ch = ((int) messages[i].getCh()) - 1 + chOffs;
			
			try{
				appendMessageToPulseSequence(messages[i], d.getPMTAtCh(ch).pulses);
			}
			catch (const char *message) {
				appendMessageToPulseSequence(messages[i], d.pulsesFromInactiveChannels);
			}
		}
	}
}



void PulseAna::appendMessageToPulseSequence(const MF &m, vector<Pulse> &pulses){

	/*  
	If ...
	- we have a MessageFloat representing a falling edge
	- the latest pulse in the same channel already has a rising edge with an earlier time stamp
	- but no falling edge
	then add the stamp of the current MessageFloat object to that pulse.
	*/ 
	if(pulses.size()){

		Pulse &mostRecentPulse = pulses.back();
		
		if(m.isFalling() && mostRecentPulse.hasRising() && !mostRecentPulse.hasFalling()){
			mostRecentPulse.setStampFalling(m.stamp);
			return;
		}
	}
	// Otherwise, create a new pulse and set the pointer 'latestPulses[ch]' to the address of that new pulse.
	pulses.push_back( Pulse(m) );
}



void PulseAna::evaluate(const Detector &d, TDiffData &output){
	
	// IDs of modules that registered a hit in the current event
	vector<int> m;

	for(int iMod = 0; iMod < d.modules.size(); iMod++){

		if(moduleRegisteredHit(d.modules[iMod])) m.push_back(iMod);
	}

	// inform "TDiffData ouput" about the number of hits in the current event
	output.initVectors(m.size());
	output.nHits = m.size();

	// loop over modules that fired in the current event
	for(int iHit = 0; iHit < m.size(); iHit++){

		const vector<Pulse> &pulses_left_up = d.modules[m[iHit]].pmt_left_up().pulses;
		const vector<Pulse> &pulses_right_down = d.modules[m[iHit]].pmt_right_down().pulses;

		output.tDiff[iHit] = pulses_right_down[0].getStampRising() - pulses_left_up[0].getStampRising();
		output.tSum[iHit] = pulses_right_down[0].getStampRising() + pulses_left_up[0].getStampRising();
		output.tot0[iHit] = pulses_left_up[0].getTot();
		output.tot1[iHit] = pulses_right_down[0].getTot();
		output.moduleID[iHit] = m[iHit];
	}
}



// *** here it is defined what it means that a certain module registered a hit ***
bool PulseAna::moduleRegisteredHit(const Module &m){

	const vector<Pulse> &pulses_left_up = m.pmt_left_up().pulses;
	const vector<Pulse> &pulses_right_down = m.pmt_right_down().pulses;

	if( !pulses_left_up.size() ) return false;
	if( !pulses_right_down.size() ) return false;
	if( !pulses_left_up[0].isComplete() ) return false;
	if( !pulses_right_down[0].isComplete() ) return false;

	return true;
}