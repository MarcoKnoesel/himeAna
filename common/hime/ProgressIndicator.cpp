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

#include "ProgressIndicator.h"

#include <iostream>
#include <iomanip>
#include <cmath>

using std::cout;
using std::endl;
using std::flush;
using std::setw;
using std::string;



ProgressIndicator::ProgressIndicator(){
	percCounter = -1;
	last = 0;
	invTotal = 1.;
	message = "";
}



ProgressIndicator::ProgressIndicator(long total, string message = ""){
	percCounter = -1;
	last = total - 1;
	invTotal = 1. / total;
	this->message = message;
}



int ProgressIndicator::showProgress(long current){

	float fraction = 100. * (current + 1) * invTotal;

	if( fraction > percCounter + 0.5 ){
		percCounter = std::roundl(fraction);
		cout << "\r" << message << setw(4) << percCounter << " % " << flush;
	}

	if( current == last ){
		cout << endl;
	}
	
	return percCounter;
}