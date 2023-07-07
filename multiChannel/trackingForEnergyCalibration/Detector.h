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

#ifndef Detector_h
#define Detector_h

#include <array>
#include "Constants.h"
#include "Module.h"
#include "TString.h"

class Detector{
	public: 
	Detector(){};
	Detector(TString trb3dir, TString geometryFile);
	bool layerIsHorizontal(int layerID) const;
	bool isHorizontal(int moduleID) const;
	int getLayer(int moduleID) const;
	void write(TString path);
	std::array<Module,Constants::nModules> modules;
};

#endif