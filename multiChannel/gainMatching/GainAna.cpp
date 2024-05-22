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


#include "GainAna.h"
#include "Convert.h"
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
using std::cout;
using std::endl;
using std::vector;
using std::array;



void GainAna::removeDeviatingPoints(Module& m){

	for(int pmt = 0; pmt < 2; pmt++){

		vector<array<double,2>> points = graphToSortedVector(m.gainGraphs[pmt]);

		if(!checkMonotonouslyIncreasing(points, 0, points.size())){

			bool firstIncr = checkMonotonouslyIncreasing(points, 0, points.size() - 1);
			bool lastIncr = checkMonotonouslyIncreasing(points, 1, points.size());

			// remove the first point, if the last ones were monotonously increasing without it
			if(firstIncr && !lastIncr){
				replacePoints(points, m.gainGraphs[pmt], 0, points.size()-1);
			}
			// remove the last point, if the last ones were monotonously increasing without it
			if(lastIncr && !firstIncr){
				replacePoints(points, m.gainGraphs[pmt], 1, points.size());
			}
		}
	}
}



void GainAna::replacePoints(const vector<array<double,2>>& points, TGraphErrors& g, int pointStart, int pointStop){
	int nPoints = g.GetN();
	for(int i = nPoints - 1; i >= 0; i--){
		g.RemovePoint(i);
	}
	for(int i = pointStart; i < pointStop; i++){
		g.SetPoint(g.GetN(), points[i][0], points[i][1]);
	}
}



vector<array<double,2>> GainAna::graphToSortedVector(const TGraphErrors& g){

	vector<array<double,2>> points(g.GetN());

	for(int i = 0; i < g.GetN(); i++){
		g.GetPoint(i, points[i][0], points[i][1]);
	}

	// sort points by *voltage* in increasing order,
	// which is the second entry
	std::sort(points.begin(), points.end(), [](const array<double,2>& left, const array<double,2>& right) { return left[1] < right[1]; });

	return points;
}



bool GainAna::checkMonotonouslyIncreasing(const vector<array<double,2>> points, int pointStart, int pointStop){
	
	for(int i = pointStart + 1; i < pointStop; i++){
		if(points[i-1][0] > points[i][0]) return false;
	}
	return true;
}



void GainAna::fit(Module& m, float desiredToT){

	std::array<float,2> targetVoltages;
	// loop over the ToT spectra of the two individual PMTs and the combined ToT spectrum
	for(int i = 0; i < m.gainFits.size(); i++){
		TF1* fit = (TF1*) m.gainFits[i];
		fit->SetParameter(0, m.gainGraphs[i].GetPointY(0));
		fit->SetParameter(1, 50.);
		fit->SetParameter(2, 0.);
		m.gainGraphs[i].Fit(fit, "rqn");

		// for the two PMTs only (not for the combined ToT of the whiole module):
		// determine the targetVoltage that is required to reach the desiredToT
		// by evaluating the best fit
		if(i > 1) continue;
		
		targetVoltages[i] = m.gainFits[i]->Eval(desiredToT);

		// Print an error message if the targetVoltage is outside of the allowed range
		if(targetVoltages[i] < 900 || targetVoltages[i] > 1500){
			cout << "[GainAna] \e[1m\e[31mError for module " << m.getID() << " channel " << m.getChannels()[i] << "!\e[0m\n";
			cout << "[GainAna] The calculated voltage " << Convert::toStr(targetVoltages[i]) << " V is outside of the allowed range [900 V, 1500 V]!" << endl;
			targetVoltages[i] = 0.;
		}
	}
	
	m.setTargetVoltages(targetVoltages);
}