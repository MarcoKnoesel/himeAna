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

#ifndef SVGPicture_h
#define SVGPicture_h

#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include "TText.h"

namespace SVGPicture{
	void drawTDiff(TH1F hTDiff);
	void drawTotVsTDiff(TH2F hTotVsTDiff);
	void removeStats();
	void copt(TCanvas *c, int pad, const char *opt);
	void setBasicFont(TH1& h);
	void setTH1Fmargins(TCanvas *can);
	void setTH2Fmargins(TCanvas *can);
	TText* scaleDownY(TH1& h, uint power, TCanvas *can, float labelOffset = 0.02);
	TString binning(const TAxis *axis);
};

#endif