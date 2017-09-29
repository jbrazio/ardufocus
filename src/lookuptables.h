/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017 João Brázio [joao@brazio.org]
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

#ifndef __LOOKUPTABLES_H__
#define __LOOKUPTABLES_H__

namespace lookup
{
  const uint8_t full_step_table_sz = 4;
  const uint8_t full_step_table[full_step_table_sz] PROGMEM = {
    B0001, B0010, B0100, B1000
  };

  const uint8_t half_step_table_sz = 8;
  const uint8_t half_step_table[half_step_table_sz] PROGMEM = {
    B1001, B0001, B0010, B0011, B0010, B00110, B1100, B1000
  };

  const uint8_t  step_freq_table_sz = 8;
  const uint16_t step_freq_table[step_freq_table_sz] PROGMEM = {
    0x7D, 0xFA, 0x1F4, 0x3E0, 0x7A1, 0xF42, 0x1E84, 0x3D09
  // 500,  250,   125,    63,    32,    16       8       4Hz
  };
}

#endif
