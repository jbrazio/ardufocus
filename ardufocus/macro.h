/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017-2019 João Brázio [joao@brazio.org]
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

#undef CRITICAL_SECTION_START
#define CRITICAL_SECTION_START const uint8_t __SREG___ = SREG; cli();

#undef CRITICAL_SECTION_END
#define CRITICAL_SECTION_END   SREG = __SREG___;

#undef bit
#define bit(b) (1UL << (b))

#undef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

#undef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#undef constrain
#define constrain(n, l, h) ((n)<(l)?(l):((n)>(h)?(h):(n)))

#undef map
#define map(n, a, b, x, y) ((n-a)*(y-x)/(b-a)+x)

#undef asizeof
#define asizeof(a) (sizeof(a) / sizeof(*a))

#define force_inline __attribute__((always_inline)) inline
#define silence      __attribute__((unused))
#define speed        __attribute__((optimize("O3")))
#define diet         __attribute__((optimize("Os")))

#endif
