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

#ifndef SpectrumAna_h
#define SpectrumAna_h

#include "Module.h"

namespace SpectrumAna{
	// Find the cosmic-muon peak. It should be the peak at the highest ToT value.
	// This function gives a start value for the fit procedure in SpectrumAna::fitLastPeak.
	int findLastPeak(const TH1F& h);
	// helper for SpectrumAna::findLastPeak
	bool threePreceedingBinsHaveSmallerContent(const TH1F& h, int currentBin);
	// apply a Gaussian fit to the cosmic-muon peak in the ToT spectrum
	std::array<std::array<double,2>,3> fitLastPeak(Module& m, int iVoltage);
};

#endif