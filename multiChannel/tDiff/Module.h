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
 * This class represents a module composed of a scintillator bar and two PMTs.
 * Each PMT is connected to a specific channel.
 */

#ifndef Module_h
#define Module_h

#include <array>
#include "PMT.h"
#include "TH2F.h"


class Module{
	public:
	Module(){}
	Module(int id, int ch_left_up, int ch_right_down);
	std::array<PMT,2> pmts;
	inline PMT pmt_left_up() const { return pmts[0]; }
	inline PMT pmt_right_down() const { return pmts[1]; }
	inline void fill(float tDiff, float tot) { hTotVsTDiff.Fill(tDiff, tot); }
	void writeIfFilled();

	private:
	TH2F hTotVsTDiff;
};

#endif