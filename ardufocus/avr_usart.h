/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017-2019 João Brázio [joao@brazio.org]
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

#ifndef __USART_H__
#define __USART_H__

#include "config.h"

#include "ringbuf.h"
#include "macro.h"

#define SERIAL_TXBUF_SZ 39u
#define SERIAL_RXBUF_SZ 78u

#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328PB__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__)
  #ifdef __AVR_ATmega328PB__
    #define USART_RX_VECT USART0_RX_vect
    #define USART_TX_VECT USART0_UDRE_vect
  #else
    #define USART_RX_VECT USART_RX_vect
    #define USART_TX_VECT USART_UDRE_vect
  #endif
#endif

namespace usart {
  struct buffer_t {
    Ringbuf<char, SERIAL_RXBUF_SZ> rx;
    Ringbuf<char, SERIAL_TXBUF_SZ> tx;
  };

  extern buffer_t buffer;
}

#endif
