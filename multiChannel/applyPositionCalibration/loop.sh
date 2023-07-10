
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
subdir=2022-11-30b
geometryFile=2022-10-11.csv
# ------------------------------

source ../../common/common.sh

make

if [ $? -eq 0 ]; then
	
	fileCounter=0
	
	create_directory applyPositionCalibration "$subdir"

	for filename in "$HIME_ANA_DIRECTORY"/data/tDiff/"$subdir"/*.root; do
		check_threads "$fileCounter"
		filename=$(basename "$filename")
		$ROOT_CALL "applyCalibration( \"${HIME_ANA_DIRECTORY}\", \"${subdir}\", \"${filename}\", \"${geometryFile}\")" > /dev/null &
		fileCounter=`expr ${fileCounter} + 1`
	done

	wait
	echo -e "\nloop.sh done."
fi;