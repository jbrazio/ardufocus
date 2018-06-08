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
  const uint8_t uln2003_unipolar_full_classic_sz = 4;
  const uint8_t uln2003_unipolar_full_classic[uln2003_unipolar_full_classic_sz] PROGMEM = {
    //ABCD
    0b1000, // 1
    0b0100, // 2
    0b0010, // 3
    0b0001, // 4
  };

  const uint8_t uln2003_unipolar_full_sz = 4;
  const uint8_t uln2003_unipolar_full[uln2003_unipolar_full_sz] PROGMEM = {
    //ABCD
    0b1001, // 1
    0b1100, // 2
    0b0110, // 3
    0b0011, // 4
  };

  const uint8_t uln2003_unipolar_half_sz = 8;
  const uint8_t uln2003_unipolar_half[uln2003_unipolar_half_sz] PROGMEM = {
    //ABCD
    0b1000, // 1
    0b1100, // 2
    0b0100, // 3
    0b0110, // 4
    0b0010, // 5
    0b0011, // 6
    0b0001, // 7
    0b1001, // 8
  };
}

#endif
