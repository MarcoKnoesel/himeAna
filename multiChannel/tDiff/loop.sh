
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
subdirs=(2024-04-11 2024-03-22 2024-03-18 2024-03-19)
trigger=-1
# ------------------------------

source ../../common/common.sh

make

if [ $? -eq 0 ]; then
	for subdir in "${subdirs[@]}"; do
		create_directory tDiff "$subdir"

		fileCounter=0

		# loop over all files
		for filename in "$HIME_ANA_DIRECTORY"/data/unpacked/"$subdir"/*.root; do
			check_threads "$fileCounter"
			filenameBase=$(basename "$filename")
			$ROOT_CALL "tDiff(\"${HIME_ANA_DIRECTORY}\",\"${subdir}\",\"${filenameBase}\",${trigger},true,false)" > /dev/null &
			fileCounter=`expr ${fileCounter} + 1`
		done
	done
	wait

	echo -e "\nloop.sh done."
fi;
