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

#ifndef Module_h
#define Module_h

#include "TH2F.h"
#include "TH1F.h"
#include "TFile.h"
#include "Channel.h"
#include <array>

class Module{
	public:
	Module();
	Module(int id, int ch1, int ch2);
	void initHistograms(int nBinsTDiff, float tDiffMin, float tDiffMax, int nBinsToT, float totMin, float totMax);
	bool fired() const { return channels[0].fired && channels[1].fired; }
	float getCombinedToT() const { return std::sqrt(channels[0].tot * channels[1].tot); }
	float getTDiff() const { return channels[0].time - channels[1].time; }
	void fillHistograms(bool coincidence);
	void write(TFile* fileOut) const;
	// first histogram is for all events, second one for coincidences only
	std::array<TH2F,2> hTotVsTDiff;
	std::array<TH1F,2> hTDiff;
	std::array<TH1F,2> hToT;
	int id;
	std::array<Channel,2> channels;
};

#endif