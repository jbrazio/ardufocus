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

#include <stdint.h>
#include <stdlib.h>

#include "stepper.h"
#include "lookuptable.h"

class uln2003: public stepper
{
protected:
  const uint8_t m_pinmask;
  uint8_t* const m_port;
  volatile int8_t m_phase = 0;

public:
  inline uln2003(uint8_t m, volatile uint8_t* p)
    : m_pinmask(m), m_port((uint8_t*) p)
  {
    if      (m_port == &PORTB) DDRB |= m_pinmask;
    else if (m_port == &PORTC) DDRC |= m_pinmask;
    else if (m_port == &PORTD) DDRD |= m_pinmask;
  }

  inline void halt()
  {
    stepper::halt();

    uint8_t output = (uint8_t)* m_port;
    output &= ~m_pinmask;
    output |= pad(0x00, m_pinmask);
    *m_port = output;
  }

  inline void set_full_step()
  {
    // not implemented
  }

  inline void set_half_step()
  {
    // not implemented
  }

  inline bool step_cw()
  {
    step();
    ++m_phase;
    if (m_phase > (lookup::full_step_table_sz -1)) { m_phase = 0; }
    return true;
  }

  inline bool step_ccw()
  {
    step();
    --m_phase;
    if (m_phase < 0) { m_phase = (lookup::full_step_table_sz -1); }
    return true;
  }

private:
  inline void step()
  {
    const uint8_t next_step = pgm_read_byte_near(lookup::full_step_table + m_phase);
    uint8_t output = (uint8_t)* m_port;
    output &= ~m_pinmask;
    output |= pad(next_step, m_pinmask);
    *m_port = output;
  }

private:
  inline uint8_t pad(uint8_t input, uint8_t mask)
  {
    uint8_t output = 0;
    for (char i = 0; i < 8; i++) {
      if ((mask & 1)) {
        if (input & 1) output |= 0x80;
        else output           &= 0x7f;
        input >>= 1;
      }

      if (i < 7) {
        output >>= 1;
        mask   >>= 1;
      }
    }
    return output;
  }
};

#endif
