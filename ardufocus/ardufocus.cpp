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

#include "ardufocus.h"

// --------------------------------------------------------------------------
// Globals ------------------------------------------------------------------
// --------------------------------------------------------------------------
eeprom_map_t  g_config;

#ifdef MOTOR1_HAS_DRIVER
  stepper* g_motor1 = &motor1drv;
#endif

#ifdef MOTOR2_HAS_DRIVER
  stepper* g_motor2 = &motor2drv;
#endif


int main(void)
{
  // --------------------------------------------------------------------------
  // DEBUG --------------------------------------------------------------------
  // --------------------------------------------------------------------------
  #ifdef DEBUG_ISR
  DDRB = bit(PB5);
  DDRC = bit(PC3) | bit(PC2);
  #endif

  // --------------------------------------------------------------------------
  // EEPROM -------------------------------------------------------------------
  // --------------------------------------------------------------------------
  eeprom_init(&g_config);


  // --------------------------------------------------------------------------
  // DTR Serial Reset ---------------------------------------------------------
  // --------------------------------------------------------------------------
  dtr_disable();


  // --------------------------------------------------------------------------
  // Disable Watchdog ---------------------------------------------------------
  // --------------------------------------------------------------------------
  wdt_disable();


  // --------------------------------------------------------------------------
  // Disable global interrupts ------------------------------------------------
  // --------------------------------------------------------------------------
  cli();


  // --------------------------------------------------------------------------
  // Load settings ------------------------------------------------------------
  // --------------------------------------------------------------------------
  #ifdef MOTOR1_HAS_DRIVER
    g_motor1->set_current_position(g_config.position_m1);
  #endif

  #ifdef MOTOR2_HAS_DRIVER
    g_motor2->set_current_position(g_config.position_m2);
  #endif


  // --------------------------------------------------------------------------
  // Timer0 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  // Cleanup all the relevant registers
  TCCR0A = 0; TCCR0B = 0; TIMSK0 = 0;
  TIFR0  = 0; TCNT0  = 0; OCR0A  = 0; OCR0B = 0;

  // set waveform generation mode to CTC, top OCR0A
  TCCR0A |= bit(WGM01);

  // set clock select to clk/64
  TCCR0B |= bit(CS01) | bit(CS00);

  // output Compare A Match Interrupt Enable
  TIMSK0 |= bit(OCIE0A);

  // sets the Output Compare Register values
  OCR0A = TIMER0_OCRA;


  // --------------------------------------------------------------------------
  // Timer2 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  // Cleanup all the relevant registers
  TCCR2A = 0; TCCR2B = 0; TIMSK2 = 0;
  TIFR2  = 0; TCNT2  = 0; OCR2A  = 0; OCR2B = 0;

  // set waveform generation mode to CTC, top OCR0A
  TCCR2A |= bit(WGM21);

  // set clock select to clk/64
  TCCR2B |= bit(CS22) | bit(CS21) | bit(CS20);

  // output Compare A Match Interrupt Enable
  TIMSK2 |= bit(OCIE2A);

  // sets the Output Compare Register values
  OCR2A = TIMER2_OCRA;


  // --------------------------------------------------------------------------
  // ADC init routine ---------------------------------------------------------
  // --------------------------------------------------------------------------
  Analog::setup();


  // --------------------------------------------------------------------------
  // User interface -----------------------------------------------------------
  // --------------------------------------------------------------------------
  UI::setup();


  // --------------------------------------------------------------------------
  // Motor #1 init routine ----------------------------------------------------
  // --------------------------------------------------------------------------
  #ifdef MOTOR1_HAS_DRIVER
    g_motor1->set_invert_direction(MOTOR1_INVERT_DIRECTION);
    g_motor1->set_sleep_when_idle(MOTOR1_SLEEP_WHEN_IDLE);
    g_motor1->set_sleep_timeout(MOTOR1_SLEEP_TIMEOUT);
    g_motor1->set_max_speed(MOTOR1_MAX_SPEED);
    g_motor1->set_min_speed(MOTOR1_MIN_SPEED);
    g_motor1->init();
    if (g_config.speed_m1 != 0) { g_motor1->set_speed(g_config.speed_m1); }
    if (g_config.half_step_m1) { g_motor1->set_half_step(); }
  #endif

  #ifdef MOTOR2_HAS_DRIVER
    g_motor2->set_invert_direction(MOTOR2_INVERT_DIRECTION);
    g_motor2->set_sleep_when_idle(MOTOR2_SLEEP_WHEN_IDLE);
    g_motor2->set_sleep_timeout(MOTOR2_SLEEP_TIMEOUT);
    g_motor2->set_max_speed(MOTOR2_MAX_SPEED);
    g_motor2->set_min_speed(MOTOR2_MIN_SPEED);
    g_motor2->init();
    if (g_config.speed_m2 != 0) { g_motor2->set_speed(g_config.speed_m2); }
    if (g_config.half_step_m2) { g_motor2->set_half_step(); }
  #endif


  // --------------------------------------------------------------------------
  // Enable global interrupts -------------------------------------------------
  // --------------------------------------------------------------------------
  sei();


  // --------------------------------------------------------------------------
  // Loop routine -------------------------------------------------------------
  // --------------------------------------------------------------------------
  for(;;)
  {
    comms.receive();

    UI::update_display();
    UI::fetch_key_state();
  }

  // Someone made an Opsie !
  // Code should not reach this
  return 0;
}
