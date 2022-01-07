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

#ifndef __ADC_H__
#define __ADC_H__

#include "config.h"

#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/io.h>
#include "macro.h"
#include "utility.h"

class Analog
{
private:
	/**
	 * Disable the creation of an instance of this object.
	 * This class should be used as a static class.
	 */
	Analog() { ; }
	~Analog() { ; }

public:
	static struct buffer_t
	{
		uint8_t  n;
		uint8_t  chan;
		uint16_t raw[128];
	} s_buffer;

	static uint16_t s_cache[4];

public:
	static void setup();
	static void read_async(const uint8_t&);
	static uint16_t read(const uint8_t& channel) { return ((channel > 3) ? 0 : s_cache[channel]); }
};

#endif
