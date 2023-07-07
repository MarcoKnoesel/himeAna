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
 * Create and analyze objects of type Pulse
 */
#ifndef PulseAna_h
#define PulseAna_h

#include <vector>

#include "Detector.h"
#include "TRB3RawData.h"
#include "Pulse.h"
#include "TDiffData.h"

namespace PulseAna{

	/*
		Search for leading and trailing edges among the 'MessageFloat' instances 'messages'
		and group them to instances of type 'Pulse'
	*/
	void findPulses(const TRB3RawData &input, Detector &d);

	/*
		Append the information from a MessageFloat object to a sequence of pulses of a certain channel
		Helper for the function above.
	*/
	void appendMessageToPulseSequence(const hadaq::MessageFloat &m, std::vector<Pulse> &pulses);

	/*
		Calculate tDiff for all modules and ToT for all PMTs.
	*/
	void evaluate(const Detector &d, TDiffData &output);

	/*
		Check if a certain module registered a hit
	*/
	bool moduleRegisteredHit(const Module &m);
};


#endif