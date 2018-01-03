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
      const uint8_t mask = pgm_read_word(&pin_map[pin][4]);
      volatile uint8_t *mode  = (volatile uint8_t *)( pgm_read_word(&pin_map[pin][0]) );
      volatile uint8_t *state = (volatile uint8_t *)( pgm_read_word(&pin_map[pin][1]) );

      CRITICAL_SECTION_START
      *mode  &= ~mask;
      *state &= ~mask;
      CRITICAL_SECTION_END
    }

    static inline void set_as_output(const uint8_t &pin) {
      const uint8_t mask = pgm_read_word(&pin_map[pin][4]);
      volatile uint8_t *mode = (volatile uint8_t *)( pgm_read_word(&pin_map[pin][0]) );

      CRITICAL_SECTION_START
      *mode |= mask;
      CRITICAL_SECTION_END
    }

    static inline void write(const uint8_t &pin, const uint8_t &value) {
      PORTD ^= bit(6);
      const uint8_t timer = pgm_read_byte(&pin_map[pin][3]);
      PORTD ^= bit(6);

      // Deals with digital signals i.e. LOW and HIGH
      if(value == LOW || value == HIGH) {

        const uint8_t mask = pgm_read_word(&pin_map[pin][4]);
        volatile uint8_t *state = (volatile uint8_t *)( pgm_read_word(&pin_map[pin][1]) );

        // Turns off any active PWM output on the pin
        switch(timer)
        {
          case TIMER0A: // timer 0, channel A
            cbi(TCCR0A, COM0A1);
            break;

          case TIMER0B: // timer 0, channel B
            cbi(TCCR0A, COM0B1);
            break;

          case TIMER1A: // timer 1, channel A
            cbi(TCCR1A, COM1A1);
            break;

          case TIMER1B: // timer 1, channel B
            cbi(TCCR1A, COM1B1);
            break;

          case TIMER2A: // timer 2, channel A
            cbi(TCCR2A, COM2A1);
            break;

          case TIMER2B: // timer 2, channel B
            cbi(TCCR2A, COM2B1);
            break;

          default:
            ;
        }

        CRITICAL_SECTION_START
        if(value == LOW) { *state &= ~mask; }
        else { *state |= mask; }
        CRITICAL_SECTION_END
      }

      // Deals with analog signals i.e. PWM
      else {
        switch(timer)
        {
          case TIMER0A: // timer 0, channel A
            sbi(TCCR0A, COM0A1);
            OCR0A = value;
            break;

          case TIMER0B: // timer 0, channel B
            sbi(TCCR0A, COM0B1);
            OCR0B = value;
            break;

          case TIMER1A: // timer 1, channel A
            sbi(TCCR1A, COM1A1);
            OCR1A = value;
            break;

          case TIMER1B: // timer 1, channel B
            sbi(TCCR1A, COM1B1);
            OCR1B = value;
            break;

          case TIMER2A: // timer 2, channel A
            sbi(TCCR2A, COM2A1);
            OCR2A = value;
            break;

          case TIMER2B: // timer 2, channel B
            sbi(TCCR2A, COM2B1);
            OCR2B = value;
            break;

          default:
            // fallbacks to digital mode when pin is not PWM capable
            write(pin, (value < 128) ? 0 : 255);
        }
      }
    }
};

#endif
