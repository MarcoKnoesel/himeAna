#!/bin/bash

#	HIMEana: Analyze HIME data.
#	
#	Copyright (C) 2023, 2024 Marco Knösel (mknoesel@ikp.tu-darmstadt.de)
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

# ------------------------------------------------------------------------------
#
#   S E T T I N G S
#
# enter the path to the root file which your data has been written to
unset path
# or leave "path" empty and read all files from the following directory:
subdir="2023-01-03"
#
# enter the name of the TDC you want to look at
tdc="TDC_2480"
#
# select here the channels you want to look at
# (called "channels of interest" in the following)
ch0=0
ch1=8
# set the following two channels to "-1" if you only want to look at two channels
ch2=4
ch3=12
#
# ------------------------------------------------------------------------------
# *** How to set which files are read ***
# 
# ** 1st possibility **
# Set the variable "path" to the complete path to a root file, such as
#     ~/my_file.root
#
# ** 2nd possibility **
# Leave "path" empty and set the directory "subdir".
# Then, all ROOT files inside that directory are read.
# ------------------------------------------------------------------------------

source ../common/common.sh

make

if [ $? -eq 0 ]; then

	get_all_files unpacked "$subdir"

	echo $ALL_FILES

	$ROOT_CALL "trb3Ana(${ALL_FILES}, \"${subdir}\", \"${path}\", \"${tdc}\", ${ch0}, ${ch1}, ${ch2}, ${ch3})"
fi;
