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

#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include "hal.h"
#include "macro.h"

#define LOW 0
#define HIGH 255

class IO
{
  /**
   * Disable the creation of an instance of this object.
   * This class should be used as a static class.
   */
  private:
     IO() {;}
    ~IO() {;}

  public:
    static inline void set_as_input(const uint8_t &pin) {
      const uint8_t     mask = pgm_read_word(&pin_map[pin][PM_MASK]);
      volatile uint8_t *mode = (volatile uint8_t *)(pgm_read_word(&pin_map[pin][PM_DIR]));
      volatile uint8_t *port = (volatile uint8_t *)(pgm_read_word(&pin_map[pin][PM_OUTPUT]));

      CRITICAL_SECTION_START
      *mode &= ~mask;
      *port &= ~mask;
      CRITICAL_SECTION_END
    }

    static inline void set_as_output(const uint8_t &pin) {
      const uint8_t     mask = pgm_read_word(&pin_map[pin][PM_MASK]);
      volatile uint8_t *mode = (volatile uint8_t *)(pgm_read_word(&pin_map[pin][PM_DIR]));

      CRITICAL_SECTION_START
      *mode |= mask;
      CRITICAL_SECTION_END
    }

    static inline void write(const uint8_t &pin, const uint8_t &value) {
      const uint8_t timer = pgm_read_byte(&pin_map[pin][PM_TIMER]);

      // Deals with digital signals i.e. LOW and HIGH
      if(value == LOW || value == HIGH) {

        const uint8_t     mask = pgm_read_word(&pin_map[pin][PM_MASK]);
        volatile uint8_t *port = (volatile uint8_t *)( pgm_read_word(&pin_map[pin][PM_OUTPUT]) );

        // Turns off any active PWM output on the pin
        switch(timer)
        {
          case TIMER0A: cbi(TCCR0A, COM0A1); break; // timer 0, channel A
          case TIMER0B: cbi(TCCR0A, COM0B1); break; // timer 0, channel B
          case TIMER1A: cbi(TCCR1A, COM1A1); break; // timer 1, channel A
          case TIMER1B: cbi(TCCR1A, COM1B1); break; // timer 1, channel B
          case TIMER2A: cbi(TCCR2A, COM2A1); break; // timer 2, channel A
          case TIMER2B: cbi(TCCR2A, COM2B1); break; // timer 2, channel B
          default: ;
        }

        CRITICAL_SECTION_START
        if(value == LOW) { *port &= ~mask; }
        else { *port |= mask; }
        CRITICAL_SECTION_END
      }

      // Deals with analog signals i.e. PWM
      else {
        switch(timer)
        {
          case TIMER0A: sbi(TCCR0A, COM0A1); OCR0A = value; break; // timer 0, channel A
          case TIMER0B: sbi(TCCR0A, COM0B1); OCR0B = value; break; // timer 0, channel B
          case TIMER1A: sbi(TCCR1A, COM1A1); OCR1A = value; break; // timer 1, channel A
          case TIMER1B: sbi(TCCR1A, COM1B1); OCR1B = value; break; // timer 1, channel B
          case TIMER2A: sbi(TCCR2A, COM2A1); OCR2A = value; break; // timer 2, channel A
          case TIMER2B: sbi(TCCR2A, COM2B1); OCR2B = value; break; // timer 2, channel B
          default: write(pin, (value < 128) ? 0 : 255); // fallbacks to digital mode
        }
      }
    }

    static inline uint8_t read(const uint8_t &pin) {
        const uint8_t     mask  = pgm_read_word(&pin_map[pin][PM_MASK]);
        const uint8_t     timer = pgm_read_byte(&pin_map[pin][PM_TIMER]);
        volatile uint8_t *port  = (volatile uint8_t *)(pgm_read_word(&pin_map[pin][PM_INPUT]));

        // If the pin that support PWM output, we need to turn it off
        // before getting a digital reading.
        if (timer != NOTIMER) {
          switch(timer)
          {
            case TIMER0A: cbi(TCCR0A, COM0A1); break; // timer 0, channel A
            case TIMER0B: cbi(TCCR0A, COM0B1); break; // timer 0, channel B
            case TIMER1A: cbi(TCCR1A, COM1A1); break; // timer 1, channel A
            case TIMER1B: cbi(TCCR1A, COM1B1); break; // timer 1, channel B
            case TIMER2A: cbi(TCCR2A, COM2A1); break; // timer 2, channel A
            case TIMER2B: cbi(TCCR2A, COM2B1); break; // timer 2, channel B
            default: ;
          }
        }

        if (*port & mask) return HIGH;
        return LOW;
    }
};

#endif
