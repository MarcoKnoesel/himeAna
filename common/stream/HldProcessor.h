/*
	This file was taken from the Stream framework and modified.

	----------------------------------------------------------------
		                  The Stream framework
	----------------------------------------------------------------
	Copyright (C) 2013 -
	GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
	Planckstr. 1, 64291 Darmstadt, Germany
	Contact:  https://github.com/gsi-ee/stream

	Modifications Copyright (C) 2023, 2024 Marco Knösel (mknoesel@ikp.tu-darmstadt.de)
	2023-07-05: Added ClassDef to create a dictionary from struct 
	HldMessage using rootcint and removed code that is not required
	for that purpose.	
	----------------------------------------------------------------
	DISCLAIMER:
	The Stream framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation, either version 3
	of the License, or (at your option) any later version.

	The Stream framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty
	of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
	the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with the Stream framework. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef HADAQ_HLDPROCESSOR_H
#define HADAQ_HLDPROCESSOR_H

#include "Rtypes.h"

namespace hadaq {

	struct HldMessage {
		uint8_t trig_type; ///< trigger type
		uint32_t seq_nr;   ///< event sequence number
		uint32_t run_nr;   ///< run number

		/** constructor */
		HldMessage() : trig_type(0), seq_nr(0), run_nr(0) {}

		/** copy constructor */
		HldMessage(const HldMessage& src) : trig_type(src.trig_type), seq_nr(src.seq_nr), run_nr(src.run_nr) {}

		ClassDef(hadaq::HldMessage,1)
	};
}

#endif
