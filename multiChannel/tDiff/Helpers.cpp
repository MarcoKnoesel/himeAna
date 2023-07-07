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

#include "Helpers.h"

#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using MF = hadaq::MessageFloat;



bool Helpers::isEarlier(MF &mf1, MF &mf2){
	return mf1.getStamp() < mf2.getStamp();
}



void Helpers::print(const vector<MF> &msgVec, int eventCounter){
	if(eventCounter > - 1){
		cout << "*** Event " << eventCounter << " ***\n";
	}
	cout << "messages:\n";
	for(int i = 0; i < msgVec.size(); i++){
		cout << "channel " << ((uint) msgVec[i].getCh()) - 1 << "    ";
		if(msgVec[i].isFalling()) cout << "falling    ";
		else cout << "rising    ";
		cout << msgVec[i].getStamp() << endl;
	}
}



void Helpers::print(const Detector &d){
	cout << "pulses:\n";
	for(int i = 0; i < d.modules.size(); i++){
		for(int j = 0; j < 2; j++){
			const std::vector<Pulse> &pulses = d.modules[i].pmts[j].pulses;
			if(pulses.size()) cout << "---- channel " << d.modules[i].pmts[j].ch << " ----" << endl;
			for(const Pulse &p : pulses){
				if(p.hasRising()) cout << "    rising: " << p.getStampRising();
				if(p.hasFalling()) cout << "    falling: " << p.getStampFalling();
				if(p.isComplete()) cout << "    ToT: " << p.getTot();
				cout << endl;
			}
		}
	}
}



TString Helpers::getFilename(TString path){
	while(path.Contains("/")){
		int pos = path.First("/");
		int length = path.Length();
		int diff = length - pos;
		path = path(pos+1,diff);
	}
	return path;
}