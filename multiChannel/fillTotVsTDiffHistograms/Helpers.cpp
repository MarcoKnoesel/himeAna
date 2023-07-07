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

#include "Helpers.h"

#include <iostream>

using std::cout;
using std::endl;
using std::vector;



bool Helpers::coincidence(const TDiffData &input, int moduleID0, int moduleID1){
	bool mod0fired = false;
	bool mod1fired = false;
	for(int mod : *(input.moduleID)){
		if( mod == moduleID0 ) mod0fired = true;
		if( mod == moduleID1 ) mod1fired = true;
	}
	return mod0fired && mod1fired;
}