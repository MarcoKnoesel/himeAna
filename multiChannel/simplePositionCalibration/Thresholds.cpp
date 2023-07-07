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

#include "Thresholds.h"

#include "CSVReader.h"
#include <array>

using std::array;
using std::vector;
using std::string;



Thresholds::Thresholds(TString pathtoCSVFile, int nModules){

	thresholds = vector<float>(nModules, -1.);
	vector<array<string,2>> dataFromCSVFile = CSVReader::read(pathtoCSVFile);

	for(array<string,2> &dataFromOneLine : dataFromCSVFile){

		int moduleID = std::stoi(dataFromOneLine[0]);
		float threshold = std::stof(dataFromOneLine[1]);
		thresholds[moduleID] = threshold;
	}
}