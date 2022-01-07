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

#ifndef __MACRO_H__
#define __MACRO_H__

#undef NULL
#define NULL 0

#undef bit
#define bit(b) (1UL << (b))

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
