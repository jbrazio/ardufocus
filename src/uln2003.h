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

// Some code was inspired bt the HotStepper Arduino library
// (c) Ben Pirt <https://github.com/bjpirt>

#ifndef __ULN2003_H__
#define __ULN2003_H__

#include "ardufocus.h"
#include "struct.h"
#include "lookuptables.h"

// total movement 37300

class uln2003: public stepper
{
protected:
  const uint8_t pinmask;
  uint8_t* const port;

  volatile int8_t phase = 0;
  volatile uint16_t speed = 2;
  volatile stepper_position_t position = { 0, 0, false };

public:
  inline uln2003(uint8_t m, volatile uint8_t* p)
    : pinmask(m), port((uint8_t*) p)
  {
    if      (port == &PORTB) DDRB |= pinmask;
    else if (port == &PORTC) DDRC |= pinmask;
    else if (port == &PORTD) DDRD |= pinmask;
  }

  inline void halt()
  {
    stepper::halt();

    uint8_t output = (uint8_t)* port;
    output &= ~pinmask;
    output |= pad(0x00, pinmask);
    *port = output;
  }

  inline void set_full_step()
  {
    // not implemented
  }

  inline void set_half_step()
  {
    // not implemented
  }

  inline void step_cw()
  {
    step();
    ++phase;
    if (phase > (lookup::full_step_table_sz -1)) { phase = 0; }
  }

  inline void step_ccw()
  {
    step();
    --phase;
    if (phase < 0) { phase = (lookup::full_step_table_sz -1); }
  }

private:
  inline void step()
  {
    const uint8_t next_step = pgm_read_byte_near(lookup::full_step_table + phase);
    uint8_t output = (uint8_t)* port;
    output &= ~pinmask;
    output |= pad(next_step, pinmask);
    *port = output;
  }

private:
  inline uint8_t pad(uint8_t input, uint8_t mask)
  {
    uint8_t output = 0;
    for (char i = 0; i < 8; i++) {
      if ((mask & 1)) {
        // We should use this pin
        if (input & 1) output |= 0b10000000;
        else           output &= 0b01111111;
        input >>= 1;
      }

      if (i < 7) {
        output >>= 1;
        mask >>= 1;
      }
    }
    return output;
  }
};

#endif
