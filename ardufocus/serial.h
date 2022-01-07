/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017-2022 João Brázio [joao@brazio.org]
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

#include "config.h"

#include <stdint.h>
#include <string.h>
#include <util/atomic.h>
#include "avr_usart.h"
#include "eeprom.h"

#ifndef CMD_MAX_LEN
#define CMD_MAX_LEN 9u
#endif

class serial {
protected:
	void setup() {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			// Defines the speed at which the serial line will operate.
			// The default settings are: 8-bit, no parity, 1 stop bit.
			const uint16_t baud_rate = ((F_CPU / (8L * 9600)) - 1) / 2;
			USART_CSRA &= ~bit(USART_BIT_U2X); // baud doubler off

			// set baudrate
			USART_BRRH = baud_rate >> 8;
			USART_BRRL = baud_rate;

			// enable rx and tx
			USART_CSRB |= bit(USART_BIT_RXEN);
			USART_CSRB |= bit(USART_BIT_TXEN);

			// enable interrupt on complete reception of a byte
			USART_CSRB |= bit(USART_BIT_RXCIE);
		}
	}

	size_t write(const char& c) {
		// wait until there is space in the buffer
		while (!usart::buffer.tx.enqueue(c)) flush();

		// Enable Data Register Empty Interrupt
		// to make sure tx-streaming is running
		USART_CSRB |= bit(USART_BIT_DRIE);

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

		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			while (!usart::buffer.rx.empty()) {
				switch (const char c = usart::buffer.rx.dequeue()) {
					// process the data on the buffer
				case CMD_END_CHAR: {
					strcpy(str, buff);
					size_t sz = pos;

					memset(&buff, 0, sizeof(buff));
					pos = 0;

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
		}
		return 0;
	}

	void flush() {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			while (bit_is_set(USART_CSRB, USART_BIT_DRIE) || bit_is_clear(USART_CSRA, USART_BIT_TXC)) {
				if (bit_is_set(USART_CSRB, USART_BIT_DRIE) && bit_is_clear(SREG, SREG_I)) {
					if (bit_is_set(USART_CSRA, USART_BIT_DRE)) {
						// send a byte from the buffer
						USART_DR = usart::buffer.tx.dequeue();
						USART_CSRA |= bit(USART_BIT_TXC);

						// turn off Data Register Empty Interrupt
						// to stop tx-streaming if this concludes the transfer
						if (usart::buffer.tx.empty()) { USART_CSRB &= ~bit(USART_BIT_DRIE); }
					}
				}
			}
		}
	}
};

#endif
