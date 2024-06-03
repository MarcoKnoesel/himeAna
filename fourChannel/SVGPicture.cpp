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

#include "SVGPicture.h"
#include "TStyle.h"



void SVGPicture::drawTDiff(TH1F hTDiff){

	TCanvas *can = new TCanvas("can0", "", 900, 650);
	setTH1Fmargins(can);
	can->SetTicks();

	setBasicFont(hTDiff);
	hTDiff.GetXaxis()->SetNdivisions(505);
	hTDiff.GetXaxis()->SetLabelOffset(0.025);
	hTDiff.GetYaxis()->SetLabelOffset(0.01);
	hTDiff.GetXaxis()->SetTitleOffset(1.8);
	hTDiff.GetYaxis()->SetTitleOffset(1.8);
	//hTDiff.GetYaxis()->SetMaxDigits(3);
	//hTDiff.GetYaxis()->SetLabelSize(0.07);
	hTDiff.GetXaxis()->SetRangeUser(-0.52, -0.32);
	hTDiff.GetXaxis()->SetTitle("$\\Delta t$ (\\si{\\nano\\second})");
	//hTDiff.GetYaxis()->SetTitle("Counts per \\SI{" + binning(h->GetXaxis()) + "}{\\nano\\second}");
	hTDiff.GetYaxis()->SetTitle("Counts per \\SI{2}{\\pico\\second}");
	hTDiff.SetTitle("");
	TText *label = scaleDownY(hTDiff, 3, can);

	can->cd();
	can->cd();
	hTDiff.DrawClone("hist");
	can->cd();
	label->Draw();
	can->cd();

	can->Print("timePrecisionMeasurement.svg");
}



void SVGPicture::drawTotVsTDiff(TH2F hTotVsTDiff){

	TCanvas *can = new TCanvas("can1","",900,650);
	setTH2Fmargins(can);
	can->SetLogz();

	setBasicFont(hTotVsTDiff);
	hTotVsTDiff.SetContour(256);
	hTotVsTDiff.SetTitle("");
	hTotVsTDiff.GetXaxis()->SetTitle("$\\Delta t$ (\\si{\\nano\\second})");
	hTotVsTDiff.GetYaxis()->SetTitle("$T$ (\\si{\\nano\\second})");
	hTotVsTDiff.GetXaxis()->SetTitleOffset(1.8);
	hTotVsTDiff.GetYaxis()->SetTitleOffset(1.8);
	hTotVsTDiff.GetXaxis()->SetLabelOffset(0.02);
	hTotVsTDiff.GetYaxis()->SetLabelOffset(0.01);
	hTotVsTDiff.GetZaxis()->SetLabelSize(0.06);
	hTotVsTDiff.DrawClone("colz");

	can->Print("totVsTDiff.svg");
}



void SVGPicture::removeStats(){
	gStyle->SetOptStat(0);
}



void SVGPicture::copt(TCanvas *c, int pad, const char *opt){
	c->cd(pad);
	TString optStr = TString(opt);
	if( optStr.Contains("g")) gPad->SetGrid();
	if( optStr.Contains("t")) gPad->SetTicks();
	if( optStr.Contains("lx")) gPad->SetLogy();
	if( optStr.Contains("ly")) gPad->SetLogy();
	if( optStr.Contains("lz")) gPad->SetLogz();
}



void SVGPicture::setBasicFont(TH1& h){
	h.GetXaxis()->SetTitleFont(40);
	h.GetYaxis()->SetTitleFont(40);
	h.GetYaxis()->SetTitleFont(40);
	h.GetXaxis()->SetLabelFont(40);
	h.GetYaxis()->SetLabelFont(40);
	h.GetZaxis()->SetLabelFont(40);
	h.SetTitleFont(40);
	h.SetLabelFont(40);
}



void SVGPicture::setTH1Fmargins(TCanvas *can){
	can->SetTopMargin(0.1);
	can->SetRightMargin(0.05);
	can->SetLeftMargin(0.15);
	can->SetBottomMargin(0.15);
}



void SVGPicture::setTH2Fmargins(TCanvas *can){
	can->SetTopMargin(0.05);
	can->SetRightMargin(0.15);
	can->SetLeftMargin(0.15);
	can->SetBottomMargin(0.15);
}



TText* SVGPicture::scaleDownY(TH1& h, uint power, TCanvas *can, float labelOffset){
	
	if(!power) return new TText();
	
	for(uint p = 0; p < power; p++) h.Scale(1./10.);
	TString label("$\\times 10");

	if(power >= 2){
		label += "^{";
		label += TString(std::to_string(power).data());
		label += "}";
	}

	label += "$";

	TText* t = new TText(can->GetLeftMargin(), 1. - can->GetTopMargin() + labelOffset, label);
	t->SetNDC();
	t->SetTextFont(40);
	
	return t;
}



TString SVGPicture::binning(const TAxis *axis){
	
	float binSize = (axis->GetXmax() - axis->GetXmin()) / axis->GetNbins();
	
	TString binningStr(std::to_string(binSize).data());
	
	while( binningStr.EndsWith("0") ){
		binningStr.Remove(binningStr.Length() - 1);
	}
	
	if(binningStr.EndsWith(".")){
		binningStr.Remove(binningStr.Length() - 1);
	}

	return binningStr;
}