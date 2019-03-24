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

#ifndef __SERIAL_H__
#define __SERIAL_H__

#ifndef CMD_START_CHAR
#error Please check the protocol specification: CMD_START_CHAR missing
#endif

#ifndef CMD_END_CHAR
#error Please check the protocol specification: CMD_END_CHAR missing
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <avr/pgmspace.h>

#include "avr_usart.h"
#include "ringbuf.h"

#ifndef CMD_MAX_LEN
  #define CMD_MAX_LEN 9u
#endif

class serial {
  protected:
    void setup() {
      const uint8_t __SREG___ = SREG;
      cli();

      // Defines the speed at which the serial line will operate.
      // The default settings are: 8-bit, no parity, 1 stop bit.
      const uint16_t UBRR0_value = ((F_CPU / (8L * 9600)) - 1) /2;
      UCSR0A &= ~bit(U2X0); // baud doubler off

      // set baudrate
      UBRR0H = UBRR0_value >> 8;
      UBRR0L = UBRR0_value;

      // enable rx and tx
      UCSR0B |= bit(RXEN0);
      UCSR0B |= bit(TXEN0);

      // enable interrupt on complete reception of a byte
      UCSR0B |= bit(RXCIE0);

      SREG = __SREG___;
    }

    size_t write(const char& c) {
      // wait until there is space in the buffer
      while (!usart::buffer.tx.enqueue(c)) flush();

      // Enable Data Register Empty Interrupt
      // to make sure tx-streaming is running
      UCSR0B |= bit(UDRIE0);

      return 1;
    }

    size_t write(const char* str) {
      size_t n = 0;
      while (*str) {
        write(*str++);
        ++n;
      }
      return n;
    }

    size_t write_P(const char* str) {
      size_t n = 0;
      while (const char c = pgm_read_byte(str++)) {
        write(c);
        ++n;
      }
      return n;
    }

    inline size_t receive(char* const str) {
      static size_t pos = 0;
      static char buff[CMD_MAX_LEN];

      const uint8_t __SREG___ = SREG;
      cli();

      while(!usart::buffer.rx.empty()) {
        switch(const char c = usart::buffer.rx.dequeue()) {
          // process the data on the buffer
          case CMD_END_CHAR: {
            strcpy(str, buff);
            size_t sz = pos;

            memset(&buff, 0, sizeof(buff));
            pos = 0;

            SREG = __SREG___;
            return sz;
          }

          // clears the data on the buffer
          case CMD_START_CHAR: {
            memset(&buff, 0, sizeof(buff));
            pos = 0;
            break;
          }

          // ignore windows line ending
          case '\r': {
            break;
          }

          // adds data to the buffer
          default: {
            buff[pos++] = c;
            pos %= sizeof(buff);
            break;
          }
        }
      }

      SREG = __SREG___;
      return 0;
    }

    void flush() {
      const uint8_t __SREG___ = SREG;
      cli();

      while (bit_is_set(UCSR0B, UDRIE0) || bit_is_clear(UCSR0A, TXC0)) {
        if (bit_is_set(UCSR0B, UDRIE0) && bit_is_clear(SREG, SREG_I)) {
          if (bit_is_set(UCSR0A, UDRE0)) {
            // send a byte from the buffer
            UDR0 = usart::buffer.tx.dequeue();
            UCSR0A |= bit(TXC0);

            // turn off Data Register Empty Interrupt
            // to stop tx-streaming if this concludes the transfer
            if (usart::buffer.tx.empty()) { UCSR0B &= ~bit(UDRIE0); }
          }
        }
      }

      SREG = __SREG___;
    }
};

#endif
