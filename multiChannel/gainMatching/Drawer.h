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

#ifndef Drawer_h
#define Drawer_h

#include <vector>
#include "TH1F.h"
#include "HistogramCollection.h"
#include "Module.h"
#include "TLegend.h"
#include <iostream>
using std::cout;
using std::endl;



class Drawer{
	
	public:
	Drawer();
	Drawer(const std::vector<float>& voltages);
	// show general information about all modules
	void drawOverview(HistogramCollection& hc);
	// show detailed information about a specific module
	void drawModule(Module& m, bool normalize);

	private:

	// the legend shows the voltages of the measurements
	TLegend createLegend(const std::vector<TH1F>& histograms);

	// find the largest bin content in a vector of histograms
	template <typename T>
	float findMaximum(const T& histograms){
		float max = 0.;
		for(int i = 0; i < histograms.size(); i++){
			if(histograms[i].GetMaximum() > max) max = histograms[i].GetMaximum();
		}
		return max;
	}
	// find the largest bin content in a vector of histograms
	template <typename T>
	float findMinimum(const T& histograms){
		float min = 0.;
		for(int i = 0; i < histograms.size(); i++){
			if(histograms[i].GetMinimum() < min) min = histograms[i].GetMinimum();
		}
		return min;
	}
	// set a proper y-axis range for histograms that are drawn in the same TPad
	template <typename T>
	void setRangeToMax(T& histograms){
		// the y-axis range should be a little larger than the largest bin content
		// -> take 110% of the maximum
		float max = findMaximum(histograms);
		if(max <= 0.) return;
		float upperLimit = 1.1 * max;
		for(TH1F& h: histograms){
			h.GetYaxis()->SetRangeUser(0., upperLimit);
		}
	}
	// set a proper y-axis range for histograms that are drawn in the same TPad
	template <typename T>
	void setRangeToMinAndMax(T& histograms){
		// the y-axis range should be a little larger than the largest bin content
		float max = findMaximum(histograms);
		float min = findMinimum(histograms);
		float upperLimit = max > 0 ? 1.1 * max : 0.9 * max;
		float lowerLimit = min > 0 ? 0.9 * min : 1.1 * min;
		for(TH1F& h: histograms){
			h.GetYaxis()->SetRangeUser(lowerLimit, upperLimit);
		}
	}
	// set a different line color for each TH1F*/TGraph*/...
	template <typename T>
	void setColorsPtr(T& drawables){
		for(int i = 0; i < drawables.size(); i++){	
			drawables[i]->SetLineColor(colors[i%colors.size()]);
		}
	}
	// set a different line color for each TH1F/TGraph/...
	template <typename T>
	void setColors(T& drawables){
		for(int i = 0; i < drawables.size(); i++){	
			drawables[i].SetLineColor(colors[i%colors.size()]);
		}
	}
	// set the same line style for each TH1F*/TGraph*/...
	template <typename T>
	void setDashedLinePtr(T& drawables){
		for(int i = 0; i < drawables.size(); i++){	
			drawables[i]->SetLineStyle(9);
		}
	}
	// set the same line style for each TH1F/TGraph/...
	template <typename T>
	void setDashedLine(T& drawables){
		for(int i = 0; i < drawables.size(); i++){	
			drawables[i].SetLineStyle(9);
		}
	}
	// remove the titles of histograms
	template <typename T>
	void removeTitles(T& drawables){
		for(int i = 0; i < drawables.size(); i++){	
			drawables[i].SetTitle("");
		}
	}
	// remove the y-axis titles of histograms
	template <typename T>
	void removeYAxisTitles(T& drawables){
		for(int i = 0; i < drawables.size(); i++){	
			drawables[i].GetYaxis()->SetTitle("");
		}
	}

	// line colors
	std::vector<int> colors = {kRed+1, kOrange+1, kGreen+2, kBlue+2, kMagenta+2};
	// voltages in the measurements
	std::vector<float> voltages;
};

#endif