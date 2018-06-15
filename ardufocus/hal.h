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

#include <avr/pgmspace.h>

#include "version.h"
#include "config.h"

#include "macro.h"

enum hal_register_headers_t { HALDIR, HALOUT, HALIN, HALTMR, HALPIN };
enum hal_non_existent_t     { NOT_A_PIN = 0, NOT_A_PORT = 0 };

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168P__)

  enum hal_timer_t { NOTIMER = 0, TIMER0A, TIMER0B, TIMER1A, TIMER1B, TIMER2A, TIMER2B };

  const uint16_t pin_map[][5] PROGMEM = {
    /* pin,              HALDIR,         HALOUT,         HALIN, HALTMR, HALPIN */
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

  static const uint16_t timer_prescaler[6][2] PROGMEM = {
    { 0b000, 0  }, { 0b001, 1   }, { 0b010, 8 },
    { 0b011, 64 }, { 0b100, 256 }, { 0b101, 1024 }
  };

#elif defined (__AVR_ATmega328PB__)
  #error FIXME
#else
  #error No supported platform found
  #error Please file a bug at https://github.com/jbrazio/ardufocus/issues
#endif

#endif
