# HIMEana - Analyze HIME Data

HIMEana serves to analyze data from the Neutron Detector 
"HIgh-resolution detector array for Multi-neutron Events" (HIME).

Copyright (C) 2023 Marco Knösel (mknoesel@ikp.tu-darmstadt.de)

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

---

## Contents
- [HIMEana - Analyze HIME Data](#himeana---analyze-hime-data)
  - [Contents](#contents)
  - [Initial Setup](#initial-setup)
  - [Structure of the HIMEana Tools](#structure-of-the-himeana-tools)
    - [Configure Details about the Setup](#configure-details-about-the-setup)


## Initial Setup
1. Go to directory `data` and create a symbolic link called `unpacked` that points towards your unpacked data:
    `ln -s [path] unpacked`.
    Inside `unpacked`, create a directory for each **experiment**, in which arbitrarily many ROOT files can be located, each one representing a **run**.
2. Go to directory `common` and modify the settings section: 
   1. Choose the root version you want to source.
   2. (optional) Modify the command that is used to call ROOT. This might be useful if you are using an older version of ROOT that does not yet include a web server to display all your instances of TCanvas, TBrowser, etc. in your web browser.
   3. Enter the complete absolute path to your `himeAna` directory.
3. In the same directory `common`, type `./compileCommons.sh`. This will create shared libraries that 


## Structure of the HIMEana Tools
The two main parts of HIMEana are `fourChannel` and `multiChannel`. The former is used to have a look on uncalibrated data from up to 4 channels and allows to cut on coincidences between two modules. The latter is used to calibrate all modules of HIME in time, position and energy, which is done is multiple steps. For each step, there is a subdirectory in `multichannel`, which reads and writes from/to the corresponding subdirectories of `data`. 

To some of the subdirectories of `data`, CSV files have to be added manually. See below.

The reason for the full analysis being divided in small steps is that the output should be checked carefully after each part of the calibration. Besides, there are two different approaches available for the position calibration.

In `fourChannel` and each of the individual analysis steps of `multiChannel`, there are shell scripts that compile the source code and start the program if the compilation was successful. To recompile from scratch, type `make clean` in the respective analysis directory.

### Configure Details about the Setup
In the directories `data/thresholds`, `data/geometry/` and `data/channelMapping/`, CSV files have to be added manually in order to correctly configure the calibration procedure:
1. `data/channelMapping/`: Tells which channels belong to the same module.
2. `data/thresholds`: ToT values for all modules above which the muon hits are located. Used to cut away low-energetic terrestrial background.
3. `data/geometry/`: Lists the positions of all modules.