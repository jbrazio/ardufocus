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

#ifndef __KEYBD_H__
#define __KEYBD_H__

#include "config.h"
#include "analog.h"
#include "api.h"
#include "io.h"

class Keybd
{
	/**
   * Disable the creation of an instance of this object.
   * This class should be used as a static class.
   */
private:
	Keybd() { ; }
	~Keybd() { ; }

	typedef void (*callback_t)(const bool&);

private:
	static motor_t motor;
	static bool inited;

public:
	static void setup();
	static void tick();
	static void debounce(bool&, bool&, bool&, uint8_t&, const uint8_t&);

public:
	static callback_t event_fwd_pressed();
	static callback_t event_bwd_pressed();
};

#endif
