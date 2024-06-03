/*
	This file was taken from the Stream framework.

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

#ifndef BASE_SUBEVENT_H
#define BASE_SUBEVENT_H

#include <algorithm>
#include <vector>

namespace base {


	/** SubEvent - base class for all event structures
	 * Need for: virtual destructor - to be able delete any instance
	 *   Reset - to be able reset (clean) all collections */

	class SubEvent {
		public:
			/** default constructor */
			SubEvent() {}

			/** destructor */
			virtual ~SubEvent() {}

			/** clear sub event */
			virtual void Clear() {}

			/** sort data in sub event */
			virtual void Sort() {}

			/** Method returns event multiplicity - that ever it means */
			virtual unsigned Multiplicity() const { return 0; }

		ClassDef(base::SubEvent,1)
	};
}

#endif
