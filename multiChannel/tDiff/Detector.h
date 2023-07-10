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
 * This class represents a set of scintillator modules. Each module has two PMTs.
 * Each PMT has its own channel number in the range from 0 to 48 times the number of TDCs. 
 * A PMT object has a std::vector of Pulse objects, where the time stamps of rising and falling signals are stored in.
 * 
 * In the constructor of the class Detector, Module objects are created. Each one will get 2 PMTs.
 * Therefore, the constructor determines which channels are connected to each module.
 * 
 * 
 * Detector
 *    |
 *    |__ Module 0
 *    |      |__ PMT 0, channel 0
 *    |      |__ PMT 1, channel 1
 *    |
 *    |__ Module 1
 *    |      |__ PMT 0, channel 2
 *    |      |__ PMT 1, channel 3
 *    |
 *    |__ Module 2
 *    |      |__ PMT 0, channel 4
 *    |      |__ PMT 1, channel 5
 *    |
 *  etc...
 * 
 */ 
#ifndef Detector_h
#define Detector_h

#include "Module.h"

#include <iostream>

class Detector{
	public:
	Detector();
	Detector(const char *path_to_csv_file);
	void clearPulses();
	void addModule(int id, int ch0, int ch1);
	void write();
	PMT& getPMTAtCh(int ch);
	Module& getModuleWithID(int id);
	Module& getModuleWithCh(int ch);
	std::vector<Module> modules;
	std::vector<int> reverseSearchChannelToModule;	// Maps channels on modules. Needed for reverse search.
	std::vector<Pulse> pulsesFromInactiveChannels;	// All pulses, for which the reverse search fails, are put here.
};

#endif