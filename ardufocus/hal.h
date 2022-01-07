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

#ifndef __HAL_H__
#define __HAL_H__

#include <avr/pgmspace.h>
#include "macro.h"

#define hal_tbl_lookup(a, b) pgm_read_word(&pin_map[a][b])

enum hal_register_headers_t { IO_DIR, IO_DATA, IO_IN, IO_BIT };

#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328PB__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__)

const uint16_t pin_map[][4] PROGMEM = {
	// PIN                IO_DIR,           IO_DATA,            IO_IN, IO_BIT
	/*   0 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(0) },
	/*   1 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(1) },
	/*   2 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(2) },
	/*   3 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(3) },
	/*   4 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(4) },
	/*   5 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(5) },
	/*   6 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(6) },
	/*   7 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(7) },

	/*   8 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(0) },
	/*   9 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(1) },
	/*  10 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(2) },
	/*  11 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(3) },
	/*  12 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(4) },
	/*  13 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(5) },

	/*  14 */ { (uint16_t)&DDRC, (uint16_t)&PORTC, (uint16_t)&PINC, bit(0) },
	/*  15 */ { (uint16_t)&DDRC, (uint16_t)&PORTC, (uint16_t)&PINC, bit(1) },
	/*  16 */ { (uint16_t)&DDRC, (uint16_t)&PORTC, (uint16_t)&PINC, bit(2) },
	/*  17 */ { (uint16_t)&DDRC, (uint16_t)&PORTC, (uint16_t)&PINC, bit(3) },
	/*  18 */ { (uint16_t)&DDRC, (uint16_t)&PORTC, (uint16_t)&PINC, bit(4) },
	/*  19 */ { (uint16_t)&DDRC, (uint16_t)&PORTC, (uint16_t)&PINC, bit(5) },
};

#define USART_BRRH UBRR0H // Baud Rate Register High
#define USART_BRRL UBRR0L // Baud Rate Register Low
#define USART_CSRA UCSR0A // Control and Status Register A
#define USART_CSRB UCSR0B // Control and Status Register B
#define USART_DR   UDR0   // Data Register

#define USART_BIT_DRE   UDRE0  // Data Register Empty
#define USART_BIT_DRIE  UDRIE0 // Data Register Empty Interrupt Enable
#define USART_BIT_PE    UPE0   // Parity Error
#define USART_BIT_RXCIE RXCIE0 // Receive Complete Interrupt Enable
#define USART_BIT_RXEN  RXEN0  // Receive Enable
#define USART_BIT_TXC   TXC0   // Transmit Complete
#define USART_BIT_TXEN  TXEN0  // Transmit Enable
#define USART_BIT_U2X   U2X0   // Double Speed Operation

#ifdef __AVR_ATmega328PB__
#define USART_RX_VECT USART0_RX_vect
#define USART_TX_VECT USART0_UDRE_vect

#else
#define USART_RX_VECT USART_RX_vect
#define USART_TX_VECT USART_UDRE_vect
#endif

#elif defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)

const uint16_t pin_map[][4] PROGMEM = {
	// PIN                IO_DIR,           IO_DATA,            IO_IN, IO_BIT
	/*   0 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(2) },
	/*   1 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(3) },
	/*   2 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(1) },
	/*   3 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(0) },
	/*   4 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(4) },
	/*   5 */ { (uint16_t)&DDRC, (uint16_t)&PORTC, (uint16_t)&PINC, bit(6) },
	/*   6 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(7) },
	/*   7 */ { (uint16_t)&DDRE, (uint16_t)&PORTE, (uint16_t)&PINE, bit(6) },
	/*   8 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(4) },
	/*   9 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(5) },
	/*  10 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(6) },

	/*  11 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(7) },
	/*  12 */ { (uint16_t)&DDRD, (uint16_t)&PORTD, (uint16_t)&PIND, bit(6) },
	/*  13 */ { (uint16_t)&DDRC, (uint16_t)&PORTC, (uint16_t)&PINC, bit(7) },

	/*  14 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(3) },
	/*  15 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(1) },
	/*  16 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(2) },
	/*  17 */ { (uint16_t)&DDRB, (uint16_t)&PORTB, (uint16_t)&PINB, bit(0) },

	/*  18 */ { (uint16_t)&DDRF, (uint16_t)&PORTF, (uint16_t)&PINF, bit(7) },
	/*  19 */ { (uint16_t)&DDRF, (uint16_t)&PORTF, (uint16_t)&PINF, bit(6) },
	/*  20 */ { (uint16_t)&DDRF, (uint16_t)&PORTF, (uint16_t)&PINF, bit(5) },
	/*  21 */ { (uint16_t)&DDRF, (uint16_t)&PORTF, (uint16_t)&PINF, bit(4) },
	/*  22 */ { (uint16_t)&DDRF, (uint16_t)&PORTF, (uint16_t)&PINF, bit(1) },
	/*  23 */ { (uint16_t)&DDRF, (uint16_t)&PORTF, (uint16_t)&PINF, bit(0) },
};

#define USART_BRRH UBRR1H // Baud Rate Register High
#define USART_BRRL UBRR1L // Baud Rate Register Low
#define USART_CSRA UCSR1A // Control and Status Register A
#define USART_CSRB UCSR1B // Control and Status Register B
#define USART_DR   UDR1   // Data Register

#define USART_BIT_DRE   UDRE1  // Data Register Empty
#define USART_BIT_DRIE  UDRIE1 // Data Register Empty Interrupt Enable
#define USART_BIT_PE    UPE1   // Parity Error
#define USART_BIT_RXCIE RXCIE1 // Receive Complete Interrupt Enable
#define USART_BIT_RXEN  RXEN1  // Receive Enable
#define USART_BIT_TXC   TXC1   // Transmit Complete
#define USART_BIT_TXEN  TXEN1  // Transmit Enable
#define USART_BIT_U2X   U2X1   // Double Speed Operation

#define USART_RX_VECT USART1_RX_vect
#define USART_TX_VECT USART1_UDRE_vect

#else
const uint16_t pin_map[][0] PROGMEM = { {} };

#error No supported platform found
#error Please file a bug at https://github.com/jbrazio/ardufocus/issues
#endif

#endif
