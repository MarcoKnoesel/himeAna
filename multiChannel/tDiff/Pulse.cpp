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

#include "Pulse.h"

#include <iostream>
using std::cout;
using std::endl;

Pulse::Pulse(){
	clear();
}

Pulse::Pulse(double stampRising, double stampFalling){
	setStampRising(stampRising);
	setStampFalling(stampFalling);
}

Pulse::Pulse(const hadaq::MessageFloat &mf){

	clear();
	
	if(mf.isRising()){
		setStampRising(mf.stamp);
	}
	else if(mf.isFalling()){
		setStampFalling(mf.stamp);
	}
}

void Pulse::clear(){
	fStampRising 	= 0.;
	fStampFalling 	= 0.;
	fTot 			= -1.;
	fIsComplete 	= false;
	fHasRising 		= false;
	fHasFalling 	= false;
}

void Pulse::setStampRising(double stamp){ 
	fStampRising = stamp; 
	fHasRising = true;
	if(fHasFalling){ 
		fIsComplete = true; 
		calculateTot(); 
	} 
};

void Pulse::setStampFalling(double stamp){ 
	fStampFalling = stamp; 
	fHasFalling = true;
	if(fHasRising){ 
		fIsComplete = true; 
		calculateTot(); 
	} 
};

double Pulse::getTot() const {
	if(fIsComplete){
		return fTot;
	}
	return -1.;
}

void Pulse::calculateTot(){
	fTot = fStampFalling - fStampRising;
}

void Pulse::printInfo() const {
	cout << "Rising edge: " << fStampRising << " ns;    Falling edge: " << fStampFalling << " ns\n";
	cout << "Complete? -> " << fIsComplete;
	if(fIsComplete) cout << ";    ToT: " << fTot << " ns";
	cout << endl;
}

bool Pulse::operator>(const Pulse &other){ 
	if(fHasRising && other.hasRising()){
		return getStampRising() > other.getStampRising();
	}
	if(fHasFalling && other.hasFalling()){
		return getStampFalling() > other.getStampFalling();
	}
	return false;
}

bool Pulse::operator<(const Pulse &other){ 
	if(fHasRising && other.hasRising()){
		return getStampRising() < other.getStampRising();
	}
	if(fHasFalling && other.hasFalling()){
		return getStampFalling() < other.getStampFalling();
	}
	return false;
}

bool Pulse::operator>=(const Pulse &other){ 
	if(fHasRising && other.hasRising()){
		return getStampRising() >= other.getStampRising();
	}
	if(fHasFalling && other.hasFalling()){
		return getStampFalling() >= other.getStampFalling();
	}
	return false;
}

bool Pulse::operator<=(const Pulse &other){ 
	if(fHasRising && other.hasRising()){
		return getStampRising() <= other.getStampRising();
	}
	if(fHasFalling && other.hasFalling()){
		return getStampFalling() <= other.getStampFalling();
	}
	return false;
}