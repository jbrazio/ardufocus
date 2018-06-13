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

#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "version.h"
#include "config.h"

#include "macro.h"

namespace util
{
  float steinhart(const uint16_t&);
  uint16_t hexstr2long(const char*);

  inline void delay_1us()
  {
    // Delay 16 cycles: 1us at 16 MHz
    asm volatile (
      "    ldi  r18, 5" "\n"
      "1:  dec  r18"  "\n"
      "    brne 1b" "\n"
      "    nop" "\n"
    );
  }

  inline void delay_2us()
  {
    // Delay 32 cycles: 2us at 16 MHz
    asm volatile (
      "    ldi  r18, 10"  "\n"
      "1:  dec  r18"  "\n"
      "    brne 1b" "\n"
      "    rjmp 1f" "\n"
      "1:"  "\n"
    );
  }

  inline void delay_250us()
  {
    // Delay 4 cycles: 250 ns at 16 MHz
    asm volatile (
      "    rjmp 1f" "\n"
      "1:  rjmp 2f" "\n"
      "2:"  "\n"
    );
  }

  #ifdef HAS_ACCELERATION
    float lerp(float const&, float const&, float);
    float clamp(float, float const&, float const&);

    #ifdef USE_SMOOTHSTEP_ACCEL
      float smoothstep(float const&, float const&, float);
      float smootherstep(float const&, float const&, float);
      float smootheststep(float const&, float const&, float);
    #endif
  #endif
};

#endif
