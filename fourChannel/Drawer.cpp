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
#include "TCanvas.h"
#include "Convert.h"



void Drawer::drawOverview(const TRBMessageHistograms& trbHist) const {
	
	TCanvas* can = new TCanvas("can", "Overview", 1200, 900);
	can->Divide(2, 2);
	
	int padCounter = 1;
	can->cd(padCounter++);
	trbHist.hCh.DrawClone();
	can->cd(padCounter++);
	trbHist.hChVsEvt.DrawClone("colz");
	can->cd(padCounter++);
	trbHist.hStamps.DrawClone();
	can->cd(padCounter++);
	trbHist.hMult.DrawClone();
}



void Drawer::drawModule(const Module& mod) const {
	
	TString canName("canvas_for_module_");
	canName += Convert::toStr(mod.id);
	TString canTitle("Module ");
	canTitle += Convert::toStr(mod.id);

	TCanvas* canMod = new TCanvas(canName, canTitle, 1200, 900);
	canMod->Divide(3, 2);

	int padCounter = 1;

	for(int iHist = 0; iHist < 2; iHist++){
		canMod->cd(padCounter++);
		mod.hToT[iHist].DrawClone();
		canMod->cd(padCounter++);
		mod.hTDiff[iHist].DrawClone();
		canMod->cd(padCounter++);
		mod.hTotVsTDiff[iHist].DrawClone("colz");
	}
}