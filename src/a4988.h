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

#ifndef __A4988_H__
#define __A4988_H__

#include <stdint.h>
#include <stdlib.h>

#include "stepper.h"
#include "io.h"

class a4988: public stepper
{
protected:
  const stepper_pin_t output;

  uint8_t m_step      = HIGH,
          m_sleep     = LOW,
          m_direction = LOW;

public:
  inline a4988(uint8_t const& dir, uint8_t const& step, uint8_t const& sleep, uint8_t const& ms1)
    : output({ NOT_A_PIN, ms1, NOT_A_PIN, NOT_A_PIN, NOT_A_PIN, sleep, step, dir })
  {
    IO::set_as_output(output.ms1);
    IO::set_as_output(output.sleep);
    IO::set_as_output(output.step);
    IO::set_as_output(output.direction);

    IO::write(output.ms1,        LOW);
    IO::write(output.step,       LOW);
    IO::write(output.direction,  LOW);

    #ifdef MOTOR_SLEEP_WHEN_IDLE
      IO::write(output.sleep, LOW);
    #else
      m_sleep = HIGH;
      IO::write(output.sleep, HIGH);
    #endif
  }

  inline a4988(uint8_t const& ms1, uint8_t const& ms2, uint8_t const& ms3,
               uint8_t const& sleep, uint8_t const& step, uint8_t const& dir)
    : output({ NOT_A_PIN, ms1, ms2, ms3, NOT_A_PIN, sleep, step, dir })
  {
    IO::set_as_output(output.ms1);
    IO::set_as_output(output.ms2);
    IO::set_as_output(output.ms3);
    IO::set_as_output(output.sleep);
    IO::set_as_output(output.step);
    IO::set_as_output(output.direction);

    IO::write(output.ms1,        LOW);
    IO::write(output.ms2,        LOW);
    IO::write(output.ms3,        LOW);
    IO::write(output.step,       LOW);
    IO::write(output.direction,  LOW);

    #ifdef MOTOR_SLEEP_WHEN_IDLE
      IO::write(output.sleep, LOW);
    #else
      m_sleep = HIGH;
      IO::write(output.sleep, HIGH);
    #endif
  }

  inline a4988(uint8_t const& ena, uint8_t const& ms1, uint8_t const& ms2, uint8_t const& ms3,
               uint8_t const& reset, uint8_t const& sleep, uint8_t const& step, uint8_t const& dir)
    : output({ ena, ms1, ms2, ms3, reset, sleep, step, dir })
  {
    IO::set_as_output(output.enable);
    IO::set_as_output(output.ms1);
    IO::set_as_output(output.ms2);
    IO::set_as_output(output.ms3);
    IO::set_as_output(output.reset);
    IO::set_as_output(output.sleep);
    IO::set_as_output(output.step);
    IO::set_as_output(output.direction);

    IO::write(output.enable,     LOW);
    IO::write(output.ms1,        LOW);
    IO::write(output.ms2,        LOW);
    IO::write(output.ms3,        LOW);
    IO::write(output.reset,      HIGH);
    IO::write(output.step,       LOW);
    IO::write(output.direction,  LOW);

    #ifdef MOTOR_SLEEP_WHEN_IDLE
      IO::write(output.sleep, LOW);
    #else
      m_sleep = HIGH;
      IO::write(output.sleep, HIGH);
    #endif
  }

  inline void halt()
  {
    stepper::halt();

    m_step = LOW;
    IO::write(output.step, LOW);

    #ifdef MOTOR_SLEEP_WHEN_IDLE
      m_sleep = LOW;
      IO::write(output.sleep, LOW);
    #endif
  }

  inline void set_full_step()
  {
    m_mode = 0x00;
    IO::write(output.ms1, LOW);
    IO::write(output.ms2, LOW);
    IO::write(output.ms3, LOW);

    // Delay 4 cycles: 250 ns at 16 MHz
    asm volatile (
        "    rjmp 1f" "\n"
        "1:  rjmp 2f" "\n"
        "2:"  "\n"
    );
  }

  inline void set_half_step()
  {
    m_mode = 0xFF;
    IO::write(output.ms1, HIGH);
    IO::write(output.ms2, LOW);
    IO::write(output.ms3, LOW);

    // Delay 4 cycles: 250 ns at 16 MHz
    asm volatile (
        "    rjmp 1f" "\n"
        "1:  rjmp 2f" "\n"
        "2:"  "\n"
    );
  }

  inline bool step_cw()
  {
    if (m_direction == LOW)
    {
      m_direction = HIGH;
      IO::write(output.direction, HIGH);

      // Delay 4 cycles: 250 ns at 16 MHz
      asm volatile (
          "    rjmp 1f" "\n"
          "1:  rjmp 2f" "\n"
          "2:"  "\n"
      );
    }

    return step();
  }

  inline bool step_ccw()
  {
    if (m_direction == HIGH)
    {
      m_direction = LOW;
      IO::write(output.direction, LOW);

      // Delay 4 cycles: 250 ns at 16 MHz
      asm volatile (
          "    rjmp 1f" "\n"
          "1:  rjmp 2f" "\n"
          "2:"  "\n"
      );
    }

    return step();
  }

private:
  inline bool step()
  {
    #ifdef MOTOR_SLEEP_WHEN_IDLE
    if (! m_sleep) {
      m_sleep = HIGH;
      IO::write(output.sleep, HIGH);

      // Delay 4 cycles: 250 ns at 16 MHz
      asm volatile (
          "    rjmp 1f" "\n"
          "1:  rjmp 2f" "\n"
          "2:"  "\n"
      );
    }
    #endif

    /*
     * The A4988 driver will physically step the motor when
     * transitioning from high to low thus the boolean return
     * value represents if the step "happen" and only then the
     * internal position counter will be updated.
     */

    if (m_step) {
      m_step = LOW;
      IO::write(output.step, LOW);

      // Delay 16 cycles: 1us at 16 MHz
      asm volatile (
          "    ldi  r18, 5" "\n"
          "1:  dec  r18"  "\n"
          "    brne 1b" "\n"
          "    nop" "\n"
      );

      return true;
    }

    else {
      m_step = HIGH;
      IO::write(output.step, HIGH);

      // Delay 16 cycles: 1us at 16 MHz
      asm volatile (
          "    ldi  r18, 5" "\n"
          "1:  dec  r18"  "\n"
          "    brne 1b" "\n"
          "    nop" "\n"
      );

      return false;
    }
  }
};

#endif
