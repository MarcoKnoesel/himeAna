/*
	This file was taken from the Stream framework and modified.

	----------------------------------------------------------------
		                  The Stream framework
	----------------------------------------------------------------
	Copyright (C) 2013 -
	GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
	Planckstr. 1, 64291 Darmstadt, Germany
	Contact:  https://github.com/gsi-ee/stream

	Modifications Copyright (C) 2023 Marco Knösel (mknoesel@ikp.tu-darmstadt.de)
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

#ifndef HADAQ_TRBPROCESSOR_H
#define HADAQ_TRBPROCESSOR_H

#include "Rtypes.h"

namespace hadaq {

	struct TrbMessage {
		bool fTrigSyncIdFound;              ///<  is sync id found
		unsigned fTrigSyncId;               ///<  sync id
		unsigned fTrigSyncIdStatus;         ///<  sync id status
		uint64_t fTrigTm;                   ///<  trigger time
		unsigned fSyncPulsePeriod;          ///<  sync pulse period
		unsigned fSyncPulseLength;          ///<  sync pulse length

		/** reset */
		void Reset()
		{
			fTrigSyncIdFound = false;
			fTrigSyncId = 0;
			fTrigTm = 0;
			fTrigSyncIdStatus = 0;
			fSyncPulsePeriod = 0;
			fSyncPulseLength = 0;
		}

		ClassDef(hadaq::TrbMessage,1)
	};
}

#endif
