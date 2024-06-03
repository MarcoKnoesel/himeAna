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

#include "Analysis.h"
using MF = hadaq::MessageFloat;
using std::vector;



bool Analysis::isEarlier(MF& msg1, MF& msg2){
	return msg1.getStamp() < msg2.getStamp();
}



void Analysis::sortMessagesByChannel(vector<MF>& messages, vector<Module>& modules){
	
	// loop over all messages
	// sort all messages by channel number
	for(MF& msg : messages){
		for(Module& mod : modules){
			for(Channel& ch : mod.channels){
				if(msg.getCh() - 1 == ch.id){
					ch.messages.push_back(&msg);
				}
			}
		}
	}
}



void Analysis::findSignal(Channel& ch){

	if(ch.messages.size() < 2){
		ch.fired = false;
		return;
	}

	// loop over all messages
	for(int iMsg = 0; iMsg < ch.messages.size() - 1; iMsg++){
		
		const MF& firstMsg = *(ch.messages[iMsg]);
		const MF& secondMsg = *(ch.messages[iMsg+1]);

		// check if there is enough information to calculate the time over threshold
		if(firstMsg.isRising() && secondMsg.isFalling()){
			
			// the module fired if a rising edge was found that is followed by a falling edge
			ch.tot = secondMsg.getStamp() - firstMsg.getStamp();
			ch.time = firstMsg.getStamp();
			ch.fired = true;
			return;
		}
	}
}



bool Analysis::coincidence(const vector<Module>& modules){
	for(const Module& mod : modules) if(!mod.fired()) return false;
	return true;
}
