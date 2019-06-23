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
  // Motor #1 init routine ----------------------------------------------------
  // --------------------------------------------------------------------------
  #ifdef MOTOR1_HAS_DRIVER
    g_motor1->set_invert_direction(MOTOR1_INVERT_DIRECTION);
    g_motor1->set_sleep_when_idle(MOTOR1_SLEEP_WHEN_IDLE);
    g_motor1->set_sleep_timeout(MOTOR1_SLEEP_TIMEOUT);
    g_motor1->set_max_speed(MOTOR1_MAX_SPEED);
    g_motor1->set_min_speed(MOTOR1_MIN_SPEED);
    g_motor1->init();
  #endif

  #ifdef MOTOR2_HAS_DRIVER
    g_motor2->set_invert_direction(MOTOR2_INVERT_DIRECTION);
    g_motor2->set_sleep_when_idle(MOTOR2_SLEEP_WHEN_IDLE);
    g_motor2->set_sleep_timeout(MOTOR2_SLEEP_TIMEOUT);
    g_motor2->set_max_speed(MOTOR2_MAX_SPEED);
    g_motor2->set_min_speed(MOTOR2_MIN_SPEED);
    g_motor2->init();
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

    #ifdef USE_UI_KAP
      static motor_t motor = MOTOR_ONE;
      static uint8_t old_motor_speed = 0;
      static bool fixed_init_has_run = false;

      if(!fixed_init_has_run)
      {
        // FIXME
        // Allows the IO to be correctly setup for the user interface
        // This should be removed from here when refactoring the class
        fixed_init_has_run = true;

        IO::set_as_input(UI_KAP_FWD_BUTTON_PIN);
        IO::set_as_input(UI_KAP_BWD_BUTTON_PIN);

        #if !defined(UI_KAP_INVERT_BUTTON_LOGIC)
        // Activates the internal pullup resistor
        IO::write(UI_KAP_FWD_BUTTON_PIN, HIGH);
        IO::write(UI_KAP_BWD_BUTTON_PIN, HIGH);
        #endif

        #ifdef UI_KAP_FWD_BUTTON_LED_PIN
        IO::set_as_output(UI_KAP_FWD_BUTTON_LED_PIN);
        IO::write(UI_KAP_FWD_BUTTON_LED_PIN, LOW);
        #endif

        #ifdef UI_KAP_BWD_BUTTON_LED_PIN
        IO::set_as_output(UI_KAP_BWD_BUTTON_LED_PIN);
        IO::write(UI_KAP_BWD_BUTTON_LED_PIN, LOW);
        #endif
      }

      // Get button state
      bool button_fwd_state = (bool)(IO::read(UI_KAP_FWD_BUTTON_PIN) == LOW); // fwd
      bool button_bwd_state = (bool)(IO::read(UI_KAP_BWD_BUTTON_PIN) == LOW); // back

      #ifdef UI_KAP_INVERT_BUTTON_LOGIC
      button_fwd_state ^= true;
      button_bwd_state ^= true;
      #endif

      #ifdef UI_KAP_FWD_BUTTON_LED_PIN
      // Visual feedback when the forward button is pressed
      IO::write(UI_KAP_FWD_BUTTON_LED_PIN, (button_fwd_state) ? HIGH : LOW);
      #endif

      #ifdef UI_KAP_BWD_BUTTON_LED_PIN
      // Visual feedback when the backward button is pressed
      IO::write(UI_KAP_BWD_BUTTON_LED_PIN, (button_bwd_state) ? HIGH : LOW);
      #endif

      //
      // TODO maybe add a button debounce routine here
      //

      // Previous button sate
      static uint8_t button_fwd_old_state = button_fwd_state;
      static uint8_t button_bwd_old_state = button_bwd_state;

      if(!button_bwd_state)
      {
        if(button_fwd_state)
        {
          if(!button_fwd_old_state) // Initial button push
          {
            old_motor_speed = api::motor_get_speed(motor);
          }

          else // Button already pushed
          {
            uint16_t potValue = Analog::read(UI_KAP_ADC_CHANNEL);

            // It's important that the "new_motor_speed" value not to
            // be lower than 2, because at the stepper tick routine it
            // will be divivded by two.
            uint8_t new_motor_speed = map(potValue, 0, 1023, 2, 64);
            api::motor_set_speed(motor, new_motor_speed);

            if(! api::motor_is_moving(motor))
            {
              // Use an overflow hack to make to motor move forever
              api::motor_set_target(motor, -1);
              api::motor_start(motor);
            }
          }

          button_fwd_old_state = button_fwd_state;
        }
        else
        {
          if(button_fwd_old_state) // Button released
          {
            api::motor_stop(motor);
            api::motor_set_speed(motor, old_motor_speed);
            button_fwd_old_state = button_fwd_state;
          }
        }
      }

      if(!button_fwd_state)
      {
        if(button_bwd_state)
        {

          if(!button_bwd_old_state) // Initial button push
          {
            old_motor_speed = api::motor_get_speed(motor);
          }

          else // Button already pushed
          {
            uint16_t potValue = Analog::read(UI_KAP_ADC_CHANNEL);

            // It's important that the "new_motor_speed" value not to
            // be lower than 2, because at the stepper tick routine it
            // will be divivded by two.
            uint8_t new_motor_speed = map(potValue, 0, 1023, 2, 64);
            api::motor_set_speed(motor, new_motor_speed);

            if(! api::motor_is_moving(motor))
            {
              api::motor_set_target(motor, 0);
              api::motor_start(motor);
            }
          }

          button_bwd_old_state = button_bwd_state;
        }
        else
        {
          if(button_bwd_old_state) // Button released
          {
            api::motor_stop(motor);
            api::motor_set_speed(motor, old_motor_speed);
            button_bwd_old_state = button_bwd_state;
          }
        }
      }
    #endif
  }

  // Someone made an Opsie !
  // Code should not reach this
  return 0;
}
