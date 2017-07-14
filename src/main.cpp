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

// Global control structures
movingmean<uint16_t, 10> temp;

#ifdef USE_ULN2003_DRIVER
  uln2003 motor1(0x0f, &PORTB);
#else
  a4988 motor1(12, 11, 10, 9, 8, 7, 6, 5);
#endif

int main(void)
{
  // --------------------------------------------------------------------------
  // Disable global interrupts ------------------------------------------------
  // --------------------------------------------------------------------------
  cli();


  // --------------------------------------------------------------------------
  // Serial port init routine -------------------------------------------------
  // --------------------------------------------------------------------------
  #if SERIAL_BAUDRATE < 57600
    const uint16_t UBRR0_value = ((F_CPU / (8L * SERIAL_BAUDRATE)) - 1) /2;
    UCSR0A &= ~bit(U2X0); // baud doubler off
  #else
    const uint16_t UBRR0_value = ((F_CPU / (4L * SERIAL_BAUDRATE)) - 1) /2;
    UCSR0A |= bit(U2X0);  // baud doubler on for high baud rates
  #endif

  // set baudrate
  UBRR0H = UBRR0_value >> 8;
  UBRR0L = UBRR0_value;

  // enable rx and tx
  UCSR0B |= bit(RXEN0);
  UCSR0B |= bit(TXEN0);

  // enable interrupt on complete reception of a byte
  UCSR0B |= bit(RXCIE0);

  // --------------------------------------------------------------------------
  // Timer0 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  // set waveform generation mode to Fast PWM
  TCCR0A |= bit(WGM01) | bit(WGM00);

  // set clock select to 64 (from prescaler)
  TCCR0B |= bit(CS01) | bit(CS00);

  // set overflow interrupt enable
  TIMSK0 |= bit(TOIE0);


  // --------------------------------------------------------------------------
  // Timer1 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  // set output compare register A to 250 Hz
  OCR1A = 0xFA;

  // set waveform generation mode to CTC
  TCCR1B |= bit(WGM12);

  // set clock select to 256 (from prescaler)
  TCCR1B |= bit(CS12);

  // set output compare A match interrupt enable
  TIMSK1 |= bit(OCIE1A);


  // --------------------------------------------------------------------------
  // Timer2 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  // set waveform generation mode to PWM Phase Correct
  TCCR2A |= bit(WGM20);

  // set clock select to 64 (from prescaler)
  TCCR2B |= bit(CS22);


  // --------------------------------------------------------------------------
  // ADC init routine ---------------------------------------------------------
  // --------------------------------------------------------------------------
  // set ADC prescaler select to 128
  ADCSRA |= bit(ADPS2) | bit(ADPS1) | bit(ADPS0);

  // set ADC Enable
  ADCSRA |= bit(ADEN);

  // set ADC multiplexer selection to external AREF
  ADMUX = 0x00;

  // set digital input disable register to A1-A5
  DIDR0 |= bit(ADC5D) | bit(ADC4D) | bit(ADC3D) | bit(ADC2D) | bit(ADC1D);


  // --------------------------------------------------------------------------
  // Enable global interrupts -------------------------------------------------
  // --------------------------------------------------------------------------
  sei();


  // --------------------------------------------------------------------------
  // Output init --------------------------------------------------------------
  // --------------------------------------------------------------------------
  DDRB |= 0x20; // Enable D13 as output
                // PORTB ^= 1 << 5;


  // --------------------------------------------------------------------------
  // Loop routine -------------------------------------------------------------
  // --------------------------------------------------------------------------
  for(;;) {
    wdt_reset();
    serial::process();
  }

  // We should not reach this
  return 0;
}
