/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017-2018 João Brázio [joao@brazio.org]
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

#ifndef __MACRO_H__
#define __MACRO_H__

#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

#ifndef CRITICAL_SECTION_START
  #define CRITICAL_SECTION_START  const uint8_t __SREG___ = SREG; cli();
  #define CRITICAL_SECTION_END    SREG = __SREG___;
#endif

#ifndef bit
  #define bit(b) (1UL << (b))
#endif

#ifndef cbi
  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#ifndef min
  #define min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef max
  #define max(a,b) ((a)>(b)?(a):(b))
#endif

/*
#ifndef abs
  #define abs(x) ((x)>0?(x):-(x))
#endif
*/

#ifndef constrain
  #define constrain(n, low, high) ((n)<(low)?(low):((n)>(high)?(high):(n)))
#endif

#ifndef asizeof
  #define asizeof(a) (sizeof(a) / sizeof(*a))
#endif

#endif
