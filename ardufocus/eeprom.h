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

#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "config.h"

#include <stdint.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "macro.h"

#define EEPROM_MAGIC_HEADER  0xaa55
#define EEPROM_START_ADDRESS 0x0000

struct eeprom_map_t {
	uint16_t header;      // 00
	uint32_t position_m1; // 02
	uint32_t position_m2; // 06
	bool     dtr_reset;   // 10
};

extern eeprom_map_t g_config;

#ifdef USE_EEPROM
void eeprom_init(eeprom_map_t*);
void eeprom_load(eeprom_map_t*);
void eeprom_save(eeprom_map_t*);
#else
inline void eeprom_init(eeprom_map_t* ptr) { memset(ptr, 0, sizeof(eeprom_map_t)); }
inline void eeprom_load(eeprom_map_t*) { ; }
inline void eeprom_save(eeprom_map_t*) { ; }
#endif

#endif
