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

#ifndef Pulse_h
#define Pulse_h

#include "TdcSubEvent.h"

class Pulse{

	public:

	Pulse(double stampRising, double stampFalling);
	Pulse(const hadaq::MessageFloat &mf);
	Pulse();
	void clear();
	void setStampRising(double stamp);
	void setStampFalling(double stamp);
	double getTot() const;
	double getStampFalling() const { return fStampFalling; }
	double getStampRising() const { return fStampRising; }
	bool isComplete() const { return fIsComplete; }
	bool hasRising() const { return fHasRising; }
	bool hasFalling() const { return fHasFalling; }
	void printInfo() const;
	bool operator>(const Pulse &other);
	bool operator<(const Pulse &other);
	bool operator>=(const Pulse &other);
	bool operator<=(const Pulse &other);
	
	private:
	void calculateTot();
	double fStampRising;
	double fStampFalling;
	double fTot;
	bool fIsComplete;
	bool fHasRising;
	bool fHasFalling;
};

#endif