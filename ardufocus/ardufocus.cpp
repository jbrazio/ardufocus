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
  // Timer0 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  // Cleanup all the relevant registers
  TCCR0A = 0; TCCR0B = 0; TIMSK0 = 0;
  TIFR0  = 0; TCNT0  = 0; OCR0A  = 0; OCR0B = 0;

  // Normal port operation, no output
  //TCCR0A &= ~(bit(COM0A1) | bit(COM0A0) | bit(COM0B1) | bit(COM0B0));

  // set waveform generation mode to CTC, top OCR0A
  TCCR0A |= bit(WGM01);

  // set clock select to clk/8
  TCCR0B |= bit(CS01);

  // output Compare A Match Interrupt Enable
  TIMSK0 |= bit(OCIE0A);

  // sets the Output Compare Register values
  OCR0A = TIMER0_OCRA;


  // --------------------------------------------------------------------------
  // ADC init routine ---------------------------------------------------------
  // --------------------------------------------------------------------------
  Analog::setup();
  Analog::read(NTC_ADC_CHANNEL);


  // --------------------------------------------------------------------------
  // Motor #1 init routine ----------------------------------------------------
  // --------------------------------------------------------------------------
  g_motor1.set_invert_direction(MOTOR1_INVERT_DIRECTION);
  g_motor1.set_sleep_when_idle(MOTOR1_SLEEP_WHEN_IDLE);
  g_motor1.set_sleep_timeout(MOTOR1_SLEEP_TIMEOUT);
  g_motor1.set_compress_steps(MOTOR1_COMPRESS_STEPS);
  g_motor1.set_max_speed(MOTOR1_MAX_SPEED);
  g_motor1.set_min_speed(MOTOR1_MIN_SPEED);
  g_motor1.init();


  // --------------------------------------------------------------------------
  // Enable global interrupts -------------------------------------------------
  // --------------------------------------------------------------------------
  sei();


  // --------------------------------------------------------------------------
  // Banner -------------------------------------------------------------------
  // --------------------------------------------------------------------------
  LogLn::PGM(PSTR("Ardufocus " ARDUFOCUS_VERSION "-" ARDUFOCUS_BRANCH " ready."));
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
