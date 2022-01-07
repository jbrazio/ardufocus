/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017-2022 João Brázio [joao@brazio.org]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __IO_H__
#define __IO_H__

#define LOW 0
#define HIGH 255

#include <util/atomic.h>
#include <avr/pgmspace.h>
#include "hal.h"

typedef uint8_t pin_t;

class IO
{
private:
	/**
	 * Disable the creation of an instance of this object.
	 * This class should be used as a static class.
	 */
	IO() { ; }
	~IO() { ; }

public:
	static inline void set_as_input(const uint8_t& pin) {
		const uint8_t     mask = hal_tbl_lookup(pin, IO_BIT);
		volatile uint8_t* mode = (volatile uint8_t*)(hal_tbl_lookup(pin, IO_DIR));
		volatile uint8_t* port = (volatile uint8_t*)(hal_tbl_lookup(pin, IO_DATA));

		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			*mode &= ~mask;
			*port &= ~mask;
		}
	}

	static inline void set_as_output(const uint8_t& pin) {
		const uint8_t     mask = hal_tbl_lookup(pin, IO_BIT);
		volatile uint8_t* mode = (volatile uint8_t*)(hal_tbl_lookup(pin, IO_DIR));

		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			*mode |= mask;
		}
	}

	static inline void write(const uint8_t& pin, const uint8_t& value) {
		const uint8_t     mask = hal_tbl_lookup(pin, IO_BIT);
		volatile uint8_t* port = (volatile uint8_t*)(hal_tbl_lookup(pin, IO_DATA));

		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			if (value == LOW) { *port &= ~mask; }
			else { *port |= mask; }
		}
	}

	static inline uint8_t read(const uint8_t& pin) {
		const uint8_t     mask = hal_tbl_lookup(pin, IO_BIT);
		volatile uint8_t* port = (volatile uint8_t*)(hal_tbl_lookup(pin, IO_IN));

		if (*port & mask) return HIGH;
		return LOW;
	}
};

#endif
