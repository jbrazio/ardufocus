/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017-2018 João Brázio [joao@brazio.org]
 *
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

#ifndef __LOOKUPTABLE_H__
#define __LOOKUPTABLE_H__

#include <avr/pgmspace.h>

namespace lookup
{
  const uint8_t full_step_table_sz = 4;
  const uint8_t full_step_table[full_step_table_sz] PROGMEM = {
        // #  DCBA
     9, // 1 B1001
     3, // 2 B0011
     6, // 3 B0110
    12  // 4 B1100
  };

  const uint8_t half_step_table_sz = 8;
  const uint8_t half_step_table[half_step_table_sz] PROGMEM = {
        // #  DCBA
     9, // 1 B1001
     1, // 2 B0001
     3, // 3 B0011
     2, // 4 B0010
     6, // 5 B0110
     4, // 6 B0100
    12, // 7 B1100
     8, // 8 B1000
  };

  const uint8_t  step_freq_table_sz = 8;
  const uint16_t step_freq_table[step_freq_table_sz] PROGMEM = {
      0x7D, // 500Hz
      0xFA, // 250Hz
     0x1F4, // 125Hz
     0x3E0, // 63Hz
     0x7A1, // 32Hz
     0xF42, // 16Hz
    0x1E84, // 8Hz
    0x3D09  // 4Hz
  };
}

#endif
