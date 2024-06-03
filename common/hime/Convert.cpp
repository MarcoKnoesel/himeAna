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

#include "Convert.h"



TString Convert::toStr(int i){
	return TString(std::to_string(i).data());
}



TString Convert::toStr(double d){
	TString str =(std::to_string(d).data());
	if(str.Contains(".")){
		while(str.EndsWith("0")) str.Remove(str.Length()-1);
		if(str.EndsWith(".")) str.Remove(str.Length()-1);
	}
	return str;
}



TString Convert::toNdigit(int i, int n){
	TString iStr(toStr(i));
	while(iStr.Length() < n) iStr = "0" + iStr;
	return iStr;
}