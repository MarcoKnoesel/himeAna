
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

headers := $(wildcard *.h)
sources := $(wildcard *.cpp)

.PHONY: clean

libTRB3Ana.so: $(sources) $(headers)
	g++ -shared -fPIC -O2 -o $@ $(sources) $(shell root-config --cflags --libs) -I. -I../../common/hime -I../../common/stream -L../../common/lib -lCommon -lStream

clean:
	rm -f libTRB3Ana.so




#headers := $(filter-out Hadaq.h, $(wildcard *.h))
#sources := $(filter-out Hadaq.cpp, $(wildcard *.cpp))
#
#.PHONY: clean
#
#libTRB3Ana.so: $(sources) Hadaq.cpp $(headers)
#	g++ -shared -fPIC -O2 -o $@ $(sources) Hadaq.cpp $(shell root-config --cflags --libs) -I. -I../../common/ -L/home/mknoesel/himeAna/common -lCommon 
#
#Hadaq.cpp: Hadaq.h LinkDef.h
#	rootcint -f $@ $^
#
#clean:
#	rm -f libTRB3Ana.so Hadaq_rdict.pcm Hadaq.cpp