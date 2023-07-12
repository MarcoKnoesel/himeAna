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

#ifndef HistogramCollection_h
#define HistogramCollection_h

#include <map>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

class HistogramCollection{
	
	public:
	HistogramCollection(){}
	void addHistogram(const TObject& obj, const TString& className, const char* name, int run, bool verbose);
	void write(TFile* file) const;

	private:
	void addHistogram(const TH1F& h1, const char* name, int run);
	void addHistogram(const TH2F& h2, const char* name, int run);
	std::map<TString, TH1F> h1map;
	std::map<TString, TH2F> h2map;
};

#endif