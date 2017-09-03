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

#ifndef __A4988_H__
#define __A4988_H__

#include "ardufocus.h"
#include "struct.h"
#include "stepper.h"

// total movement 37300

class a4988: public stepper
{
protected:
  const stepper_pin_t output;
  volatile bool idle;

public:
  inline a4988(uint8_t const& dir, uint8_t const& step, uint8_t const& sleep, uint8_t const& ms1)
    : output({ NOT_A_PIN, ms1, NOT_A_PIN, NOT_A_PIN, NOT_A_PIN, sleep, step, dir })
  {
    idle = true;

    pinMode(output.ms1,       OUTPUT);
    pinMode(output.sleep,     OUTPUT);
    pinMode(output.step,      OUTPUT);
    pinMode(output.direction, OUTPUT);

    digitalWrite(output.ms1,        LOW);
    digitalWrite(output.sleep,      LOW);
    digitalWrite(output.step,       LOW);
    digitalWrite(output.direction,  LOW);
  }

  inline a4988(uint8_t const& ena, uint8_t const& ms1, uint8_t const& ms2, uint8_t const& ms3,
               uint8_t const& reset, uint8_t const& sleep, uint8_t const& step, uint8_t const& dir)
    : output({ ena, ms1, ms2, ms3, reset, sleep, step, dir })
  {
    idle = true;

    pinMode(output.enable,    OUTPUT);
    pinMode(output.ms1,       OUTPUT);
    pinMode(output.ms2,       OUTPUT);
    pinMode(output.ms3,       OUTPUT);
    pinMode(output.reset,     OUTPUT);
    pinMode(output.sleep,     OUTPUT);
    pinMode(output.step,      OUTPUT);
    pinMode(output.direction, OUTPUT);

    digitalWrite(output.enable,     LOW);
    digitalWrite(output.ms1,        LOW);
    digitalWrite(output.ms2,        LOW);
    digitalWrite(output.ms3,        LOW);
    digitalWrite(output.reset,      HIGH);
    digitalWrite(output.sleep,      LOW);
    digitalWrite(output.step,       LOW);
    digitalWrite(output.direction,  LOW);
  }

  inline void halt()
  {
    idle = true;
    stepper::halt();
    digitalWrite(output.sleep, LOW);
  }

  inline void set_full_step()
  {
    digitalWrite(output.ms1, LOW);
    mode = 0x00;
  }

  inline void set_half_step()
  {
    digitalWrite(output.ms1, HIGH);
    mode = 0xFF;
  }

  inline void step_cw()
  {
    digitalWrite(output.direction, HIGH);

    __asm__ __volatile__ ("nop\n\t"); // 62.5ns wait
    __asm__ __volatile__ ("nop\n\t"); // 62.5ns wait
    __asm__ __volatile__ ("nop\n\t"); // 62.5ns wait
    __asm__ __volatile__ ("nop\n\t"); // 62.5ns wait

    step();
  }

  inline void step_ccw()
  {
    digitalWrite(output.direction, LOW);

    __asm__ __volatile__ ("nop\n\t"); // 62.5ns wait
    __asm__ __volatile__ ("nop\n\t"); // 62.5ns wait
    __asm__ __volatile__ ("nop\n\t"); // 62.5ns wait
    __asm__ __volatile__ ("nop\n\t"); // 62.5ns wait

    step();
  }

private:
  inline void step()
  {
    if (idle) {
      digitalWrite(output.sleep, HIGH);

      __asm__ __volatile__ ("nop\n\t"); // 62.5ns wait
      __asm__ __volatile__ ("nop\n\t"); // 62.5ns wait
      __asm__ __volatile__ ("nop\n\t"); // 62.5ns wait
      __asm__ __volatile__ ("nop\n\t"); // 62.5ns wait
    }

    digitalWrite(output.step, HIGH);
    delayMicroseconds(2);

    digitalWrite(output.step, LOW);
    delayMicroseconds(2);
  }
};

#endif
