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

#include "ardufocus.h"

// --------------------------------------------------------------------------
// Globals ------------------------------------------------------------------
// --------------------------------------------------------------------------
float g_ambient = 0.0F;
MOTOR_DRIVER g_motor1({ MOTOR1_PINOUT });

int main(void)
{
  // --------------------------------------------------------------------------
  // Disable Watchdog ---------------------------------------------------------
  // --------------------------------------------------------------------------
  wdt_disable();


  // --------------------------------------------------------------------------
  // Disable global interrupts ------------------------------------------------
  // --------------------------------------------------------------------------
  cli();


  // --------------------------------------------------------------------------
  // Serial port init routine -------------------------------------------------
  // --------------------------------------------------------------------------
  Serial::setup();


  // --------------------------------------------------------------------------
  // Timer1 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  TCCR1B = 0; TIMSK1 = 0;

  // set waveform generation mode to CTC, top OCR1A (D9, D10)
  TCCR1B |= bit(WGM12);

  // set clock select to clk/64
  TCCR1B |= bit(CS11) | bit(CS10);

  // output Compare A Match Interrupt Enable
  TIMSK1 |= bit(OCIE1A);

  // sets the Output Compare Register values
  //OCR1A = 0x32; // (5kHz)


  // --------------------------------------------------------------------------
  // ADC init routine ---------------------------------------------------------
  // --------------------------------------------------------------------------
  Analog::setup();
  Analog::read(THERMISTOR_ADC_CHANNEL);


  // --------------------------------------------------------------------------
  // Enable global interrupts -------------------------------------------------
  // --------------------------------------------------------------------------
  sei();


  // --------------------------------------------------------------------------
  // Banner -------------------------------------------------------------------
  // --------------------------------------------------------------------------
  LogLn::PGM(PSTR("Ardufocus " ARDUFOCUS_VERSION " ready."));
  LogLn::PGM(PSTR("Visit " ARDUFOCUS_URL " for updates."));
  Log::eol();


  // --------------------------------------------------------------------------
  // Loop routine -------------------------------------------------------------
  // --------------------------------------------------------------------------
  for(;;) {
    Serial::process(&protocol::process);
  }

  // We should not reach this
  return 0;
}
