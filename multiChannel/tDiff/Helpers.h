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

#ifndef Helpers_h
#define Helpers_h

#include <vector>
#include "TString.h"
#include "TdcSubEvent.h"
#include "Detector.h"

namespace Helpers{
	bool isEarlier(hadaq::MessageFloat &mf1, hadaq::MessageFloat &mf2);
	TString getFilename(TString path);
	void print(const std::vector<hadaq::MessageFloat> &msgVec, int eventCounter = -1);
	void print(const Detector &d);
};

#endif