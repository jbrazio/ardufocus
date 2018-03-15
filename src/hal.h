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

#ifndef __HAL_H__
#define __HAL_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include "macro.h"

#ifdef __AVR_ATmega328P__

  enum hal_timer_t
  {
    NOTIMER = 0,
    TIMER0A,
    TIMER0B,
    TIMER1A,
    TIMER1B,
    TIMER2A,
    TIMER2B
  };

  enum hal_pin_map_t
  {
    PM_DIR = 0,
    PM_OUTPUT,
    PM_INPUT,
    PM_TIMER,
    PM_MASK,
  };

  enum hal_non_existent_t
  {
    NOT_A_PIN = 0,
    NOT_A_PORT = 0
  };

  const uint16_t pin_map[][5] PROGMEM =
  {
    /* pin,              PM_DIR,         PM_OUTPUT,         PM_INPUT, PM_TIMER, PM_MASK */
    /*  0 */ { (uint16_t) &DDRD, (uint16_t) &PORTD, (uint16_t) &PIND,  NOTIMER,  bit(0) },
    /*  1 */ { (uint16_t) &DDRD, (uint16_t) &PORTD, (uint16_t) &PIND,  NOTIMER,  bit(1) },
    /*  2 */ { (uint16_t) &DDRD, (uint16_t) &PORTD, (uint16_t) &PIND,  NOTIMER,  bit(2) },
    /*  3 */ { (uint16_t) &DDRD, (uint16_t) &PORTD, (uint16_t) &PIND,  TIMER2B,  bit(3) },
    /*  4 */ { (uint16_t) &DDRD, (uint16_t) &PORTD, (uint16_t) &PIND,  NOTIMER,  bit(4) },
    /*  5 */ { (uint16_t) &DDRD, (uint16_t) &PORTD, (uint16_t) &PIND,  TIMER0B,  bit(5) },
    /*  6 */ { (uint16_t) &DDRD, (uint16_t) &PORTD, (uint16_t) &PIND,  TIMER0A,  bit(6) },
    /*  7 */ { (uint16_t) &DDRD, (uint16_t) &PORTD, (uint16_t) &PIND,  NOTIMER,  bit(7) },

    /*  8 */ { (uint16_t) &DDRB, (uint16_t) &PORTB, (uint16_t) &PINB,  NOTIMER,  bit(0) },
    /*  9 */ { (uint16_t) &DDRB, (uint16_t) &PORTB, (uint16_t) &PINB,  TIMER1A,  bit(1) },
    /* 10 */ { (uint16_t) &DDRB, (uint16_t) &PORTB, (uint16_t) &PINB,  TIMER1B,  bit(2) },
    /* 11 */ { (uint16_t) &DDRB, (uint16_t) &PORTB, (uint16_t) &PINB,  TIMER2A,  bit(3) },
    /* 12 */ { (uint16_t) &DDRB, (uint16_t) &PORTB, (uint16_t) &PINB,  NOTIMER,  bit(4) },
    /* 13 */ { (uint16_t) &DDRB, (uint16_t) &PORTB, (uint16_t) &PINB,  NOTIMER,  bit(5) },

    /* 14 */ { (uint16_t) &DDRC, (uint16_t) &PORTC, (uint16_t) &PINC,  NOTIMER,  bit(0) },
    /* 15 */ { (uint16_t) &DDRC, (uint16_t) &PORTC, (uint16_t) &PINC,  NOTIMER,  bit(1) },
    /* 16 */ { (uint16_t) &DDRC, (uint16_t) &PORTC, (uint16_t) &PINC,  NOTIMER,  bit(2) },
    /* 17 */ { (uint16_t) &DDRC, (uint16_t) &PORTC, (uint16_t) &PINC,  NOTIMER,  bit(3) },
    /* 18 */ { (uint16_t) &DDRC, (uint16_t) &PORTC, (uint16_t) &PINC,  NOTIMER,  bit(4) },
    /* 19 */ { (uint16_t) &DDRC, (uint16_t) &PORTC, (uint16_t) &PINC,  NOTIMER,  bit(5) },
  };

#else
  #error AVR ATmega328P not found, aborting
#endif

#endif
