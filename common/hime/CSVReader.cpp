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

#include "CSVReader.h"

#include <fstream>
using std::ifstream;
using std::string;
using std::vector;


vector<vector<string>> CSVReader::read(const char *path_to_csv_file, const int nEntries){

	// input
	ifstream ifs(path_to_csv_file);
	// a line in the csv file
	std::string line;
	// data that will be returned from this function
	vector<vector<string>> data;


	while(getline(ifs,line)){

		// skip empty lines
		if(!line.size()) continue;

		// remove leading whitespaces
		while(line[0] == ' ' || line[0] == '\t') line.erase(0);

		// After removing the whitespaces, the line might be empty.
		// If so, skip it.
		if(!line.size()) continue;

		// skip comments
		if(line[0] == '#') continue;
		
		// find delimiters
		vector<int> delimiters(nEntries+1);
		delimiters[0] = -1;
		delimiters.back() = line.size();
		int commaCounter = 1;
		for(int i = 0; i < line.size(); i++){
			if(line[i] == ',' || line[i] == ';'){
				delimiters[commaCounter] = i;
				commaCounter++;
			}
			if(commaCounter == nEntries+1) break;
		}
		
		// extract entries as std::string
		vector<string> entries(nEntries);
		bool goToNextLine = false;
		
		for(int i = 0; i < entries.size(); i++){
			
			int pos = delimiters[i]+1;
			int size = delimiters[i+1] - pos;
			
			entries[i] = line.substr(pos, size);
			
			// skip lines with empty entries
			if(!entries[i].size()){
				goToNextLine = true;
				break;
			}
		}
		if(goToNextLine) continue;

		data.push_back(entries);
	}

	return data;
}