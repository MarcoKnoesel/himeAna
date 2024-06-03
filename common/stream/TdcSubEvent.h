/*
	This file was taken from the Stream framework and modified.

	----------------------------------------------------------------
		                  The Stream framework
	----------------------------------------------------------------
	Copyright (C) 2013 -
	GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
	Planckstr. 1, 64291 Darmstadt, Germany
	Contact:  https://github.com/gsi-ee/stream
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

#ifndef HADAQ_TDCSUBEVENT_H
#define HADAQ_TDCSUBEVENT_H

#include "Rtypes.h"
#include "SubEvent.h"

namespace hadaq {

	struct MessageFloat {
		uint8_t ch;    ///< channel and edge
		float   stamp; ///< time stamp minus channel0 time, ns

		/**  stamp */
		float getStamp() const { return stamp; }
		/**  channel */
		uint8_t getCh() const { return ch & 0x7F; }
		/**  edge 0 - rising, 1 - falling */
		uint8_t getEdge() const { return ch >> 7; } // 0 - rising, 1 - falling
		/**  is rising */
		bool isRising() const { return getEdge() == 0; }
		/**  is falling */
		bool isFalling() const { return getEdge() == 1; }

		/** constructor */
		MessageFloat() : ch(0), stamp(0.) {}
		/** constructor */
		MessageFloat(const MessageFloat& src) : ch(src.ch), stamp(src.stamp) {}
		/** constructor */
		MessageFloat(unsigned _ch, bool _rising, float _stamp) :
			ch(_ch | (_rising ? 0x00 : 0x80)),
			stamp(_stamp)
		{
		}

		/** compare operator - used for time sorting */
		bool operator<(const MessageFloat &rhs) const
			{ return (stamp < rhs.stamp); }

		ClassDef(hadaq::MessageFloat,1)
   };
}

#endif
