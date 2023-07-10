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

#include "Module.h"
#include "Convert.h"

Module::Module(int id, int ch_left_up, int ch_right_down){
	pmts = {PMT(ch_left_up), PMT(ch_right_down)};
	hTotVsTDiff = TH2F("hTotVsTDiff_module_" + Convert::toNdigit(id, 3), "ToT vs. tDiff for Module " + Convert::toStr(id), 240, -30, 30, 100, 0, 50);
}

void Module::writeIfFilled(){
	if(hTotVsTDiff.GetEntries()){
		hTotVsTDiff.Write();
	}
}