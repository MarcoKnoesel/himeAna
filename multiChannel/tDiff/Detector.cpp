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

#include "Detector.h"

#include "CSVReader.h"
#include "Constants.h"

using std::array;
using std::vector;

#include <iostream>
using std::cout;
using std::endl;



Detector::Detector(){
	modules = vector<Module>(Constants::nModules);
	reverseSearchChannelToModule = vector<int>(2 * modules.size(), -1);

	// *** Here, the individual channels/PMTs are assigned to modules ***

	/*for(int i = 0; i < modules.size(); i++){
		addModule(i, 2 * i, 2 * i + 1);
	}*/
	
	addModule(0, 0, 8);
	addModule(1, 4, 12);
	
	/*addModule(2, 4, 5);
	addModule(3, 1, 7);//addModule(3, 6, 7);
	addModule(4, 8, 9);
	addModule(5, 2, 11);//addModule(5, 10, 11);
	addModule(6, 12, 13);
	addModule(7, 14, 3);//addModule(7, 14, 15);

	for(int i = 8; i < modules.size(); i++){
		addModule(i, 2 * i, 2 * i + 1);
	}
	*/	
}



Detector::Detector(const char *path_to_csv_file){

	modules = vector<Module>(Constants::nModules);
	reverseSearchChannelToModule = vector<int>(2 * modules.size(), -1);

	const int nColumns = 7;

	vector<array<int,nColumns>> csvData = CSVReader::read(path_to_csv_file);
	
	// combine information and add modules to the detector
	for(const array<int,nColumns> &entries : csvData){

		int wall 		= entries[0];
		int ch_r_d_raw 	= entries[1];
		int ch_l_u_raw 	= entries[2];
		int layer 		= entries[3];
		int mod_raw 	= entries[4];
		int chain 		= entries[5];
		int tdc  		= entries[6];

		int channelOffset = chain * 16 + tdc * 48 + wall * 100;

		addModule(
			layer * 24 + 7 + mod_raw, 
			ch_l_u_raw + channelOffset,
			ch_r_d_raw + channelOffset 
		);
	}
}



void Detector::clearPulses(){
	
	for(int i = 0; i < modules.size(); i++){

		for(int j = 0; j < 2; j++){

			modules[i].pmts[j].pulses.clear();
		}
	}

	pulsesFromInactiveChannels.clear();
}



void Detector::addModule(int id, int ch0, int ch1){
	modules[id] = Module(id, ch0, ch1);
	reverseSearchChannelToModule[ch0] = id;
	reverseSearchChannelToModule[ch1] = id;
}



void Detector::write(){
	for(Module& m : modules) m.writeIfFilled();
}



Module& Detector::getModuleWithID(int id){
	return modules[id]; 
}



Module& Detector::getModuleWithCh(int ch){
	int id = reverseSearchChannelToModule[ch];
	if(id < 0) throw "Signal detected in inactive channel";
	return modules[id]; 
}



PMT& Detector::getPMTAtCh(int ch){
	Module &m = getModuleWithCh(ch);
	if(m.pmts[0].ch == ch) return m.pmts[0];
	return m.pmts[1];
}