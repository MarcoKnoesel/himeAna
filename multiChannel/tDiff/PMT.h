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
 *
 * This class represents a photomultiplier tube which sends a sequence of pulses to its respective time-to-digital converter (TDC).
 * The sequence of pulses is respresented by the field "std::vector<Pulse> pulses".
 *
 */
#ifndef PMT_h
#define PMT_h

#include <vector>
#include "Pulse.h"

class PMT{
	public:
	PMT(){}
	PMT(int ch){ this->ch = ch; }
	int ch;
	std::vector<Pulse> pulses;
};

#endif