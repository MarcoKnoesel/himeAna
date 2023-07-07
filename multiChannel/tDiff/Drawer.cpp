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

#include "Drawer.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TImage.h"

using std::vector;
using std::to_string;



Drawer::Drawer(){
	gStyle->SetOptStat(1111111111);
}



void Drawer::draw1d(HistogramCollection &hc){

	TCanvas *can = new TCanvas("can1d","",900,900);
	
	can->Divide(2,2);
	fPadCounter = 1;
	copt(can,"t");
	hc.hChannels->Draw();
	copt(can,"t");
	hc.hTimeStamps->Draw();
	copt(can,"tly");
	hc.hNMessages->Draw();
	copt(can,"tly");
	hc.hTrigger->Draw();
}



void Drawer::draw2d(HistogramCollection &hc){

	TCanvas *can = new TCanvas("can2d","",900,900);
	
	can->Divide(2,2);

	fPadCounter = 1;
	copt(can,"t");
	hc.hChVsStampRising->Draw("colz");
	copt(can,"t");
	hc.hChVsEvtNr->Draw("colz");
	copt(can,"t");
	hc.hChVsStampFalling->Draw("colz");
}



void Drawer::copt(TCanvas *c, const char *opt){
	c->cd(fPadCounter);
	fPadCounter++;
	TString optStr = TString(opt);
	if( optStr.Contains("g")) gPad->SetGrid();
	if( optStr.Contains("t")) gPad->SetTicks();
	if( optStr.Contains("lx")) gPad->SetLogy();
	if( optStr.Contains("ly")) gPad->SetLogy();
	if( optStr.Contains("lz")) gPad->SetLogz();
}



void Drawer::img(TCanvas *c, const char str[]){

	TImage *img = TImage::Create();
	img	-> FromPad(c);
	img	-> WriteImage(str);
}
