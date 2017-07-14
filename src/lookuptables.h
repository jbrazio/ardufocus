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
}

#endif
