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

#include "avr_usart.h"

usart::buffer_t usart::buffer;

ISR(USART_RX_VECT) {
  // read a byte from the incoming stream
  // check for parity error and buffer it
  if (bit_is_clear(USART_CSRA, USART_BIT_PE)) { usart::buffer.rx.enqueue(USART_DR); }
}

ISR(USART_TX_VECT) {
  // transmit a byte from the buffer
  // disable USART TX ISR when buffer is empty
  if (! usart::buffer.tx.empty()) {
    USART_DR = usart::buffer.tx.dequeue();
    USART_CSRA |= bit(USART_BIT_TXC);
  } else { USART_CSRB &= ~bit(USART_BIT_DRIE); }
}
