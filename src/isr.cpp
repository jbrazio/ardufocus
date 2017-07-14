/**
 * Ardufocus - Moonlite compatible focuser
 * Copyright (C) 2017 João Brázio [joao@brazio.org]
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

#include "ardufocus.h"

// Timer1 interrupt handler
ISR(TIMER1_COMPA_vect)
{
  motor1.tick();
}

// Analog to Digital Converter interrupt handler
ISR(ADC_vect)
{
  temp += static_cast<int16_t>(ADCW);
}

// Serial RX interrupt handler
ISR(USART_RX_vect)
{
  uint8_t c = UDR0; // read a byte
  // check for parity errors and filter for ASCII chars
  if (bit_is_clear(UCSR0A, UPE0)) {
    //TODO: trigger an alarm on buffer overflow
    serial::buffer.rx.enqueue(c);
  } /* else { discard byte } */
}

// Serial TX interrupt handler
ISR(USART_UDRE_vect)
{
  // send a byte from the buffer
  UDR0 = serial::buffer.tx.dequeue();
  UCSR0A |= bit(TXC0);

  // turn off Data Register Empty Interrupt
  // to stop tx-streaming if this concludes the transfer
  if (serial::buffer.tx.empty()) { UCSR0B &= ~bit(UDRIE0); }
}
