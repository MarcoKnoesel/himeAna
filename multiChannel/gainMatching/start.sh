
#	HIMEana: Analyze HIME data.
#	
#	Copyright (C) 2023 Marco Knösel (mknoesel@ikp.tu-darmstadt.de)
#
#	This file is part of HIMEana.
#	
#	HIMEana is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	HIMEana is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with HIMEana.  If not, see <https://www.gnu.org/licenses/>.

#!/bin/bash

# ---------- settings ----------

# Set here the name of the directory where the output 
# of the gain matching procedure will be written
outputSubdir=2024-05-07

# provide that directory names of all measurements with different voltages
subdirs=(2024-04-11 2024-03-22 2024-03-18 2024-03-19) 

# this is required to tell the program which channels belong to each module
channelMapping=2024-03-17.csv

# list here the corresponding voltages in the right order
voltages=(1000 1100 1200 1300)

# Set here the ToT you want to have for the cosmic-muon line.
# The voltage for each module will be calculated according to this setting.
desiredToT=20

# set this to true in order to fit a linear model to the (ToT, voltage) data points
# or to false for a quadratic model
linearGainFitModel=true
# ------------------------------

source ../../common/common.sh

make

if [ $? -eq 0 ]; then

	# create a list of all root files in all subdirs
	files_of_all_subdirs="{"
	vector_of_all_subdirs="{"
	vector_of_all_voltages="{"
	for subdir in "${subdirs[@]}"; do
		get_all_files tDiff "$subdir"
		echo "$subdir"
		echo "$ALL_FILES"
		files_of_all_subdirs="$files_of_all_subdirs""$ALL_FILES",
		vector_of_all_subdirs="$vector_of_all_subdirs""\"$subdir\"",
	done
	for voltage in "${voltages[@]}"; do
		vector_of_all_voltages="$vector_of_all_voltages""$voltage",
	done
	files_of_all_subdirs=${files_of_all_subdirs%?}
	files_of_all_subdirs="$files_of_all_subdirs""}"
	vector_of_all_subdirs=${vector_of_all_subdirs%?}
	vector_of_all_subdirs="$vector_of_all_subdirs""}"
	vector_of_all_voltages=${vector_of_all_voltages%?}
	vector_of_all_voltages="$vector_of_all_voltages""}"

	# create directory for output
	create_directory gainMatching "$outputSubdir"

	# start 
	$ROOT_CALL "gainMatching(\"${HIME_ANA_DIRECTORY}\",${vector_of_all_voltages},${vector_of_all_subdirs},${files_of_all_subdirs},${desiredToT},${linearGainFitModel},\"${outputSubdir}\",\"${channelMapping}\")"
	
	wait
	echo -e "\nstart.sh done."
fi;
