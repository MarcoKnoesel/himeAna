
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

headers = $(filter-out LinkDef.h, $(wildcard *.h))
rootcint_generated_source = stream.cpp
rootcint_generated_pcm = $(rootcint_generated_source:.cpp=_rdict.pcm)
target = ../lib/libStream.so

all: $(target)

$(target): $(rootcint_generated_source) $(headers) LinkDef.h
	g++ -shared -fPIC -O2 -o $@ $(rootcint_generated_source) $(shell root-config --cflags --libs) -I.

$(rootcint_generated_source): $(headers) LinkDef.h
	rootcint -f $@ $^ && mv $(rootcint_generated_pcm) ../lib/$(rootcint_generated_pcm)

clean: 
	rm -f $(rootcint_generated_source)
	rm -f ../lib/$(rootcint_generated_pcm)
	rm -f $(target)