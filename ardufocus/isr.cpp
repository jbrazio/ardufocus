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

#include "isr.h"

/**
 * @brief Timer0 interrupt handler
 * @details
 *
 */
ISR(TIMER0_COMPA_vect)
{
  #ifdef DEBUG_ISR
    PORTB ^= bit(PB5);
  #endif

  #ifdef MOTOR1_HAS_DRIVER
    #ifdef DEBUG_ISR
      PORTC ^= bit(PC2);
    #endif

    //
    // This block takes ~50uS to execute when motor is stepping
    //

    g_motor1->tick();

    // previous motor state
    static bool pstate1 = g_motor1->is_moving();

    // current motor state
    bool cstate1 = g_motor1->is_moving();

    if(cstate1 != pstate1) {
      if(cstate1 == false) {
        g_config.position_m1 = g_motor1->get_current_position();
        eeprom_save(&g_config);
      }
      pstate1 = cstate1;
    }

    #ifdef DEBUG_ISR
      PORTC ^= bit(PC2);
    #endif
  #endif

  #ifdef MOTOR2_HAS_DRIVER
    #ifdef DEBUG_ISR
      PORTC ^= bit(PC3);
    #endif

    //
    // This block takes ~50uS to execute when motor is stepping
    //
    //
    g_motor2->tick();

    // previous motor state
    static bool pstate2 = g_motor2->is_moving();

    // current motor state
    bool cstate2 = g_motor2->is_moving();

    if(cstate2 != pstate2) {
      if(cstate2 == false) {
        g_config.position_m2 = g_motor2->get_current_position();
        eeprom_save(&g_config);
      }
      pstate2 = cstate2;
    }

    #ifdef DEBUG_ISR
      PORTC ^= bit(PC3);
    #endif
  #endif

  #ifdef DEBUG_ISR
    PORTB ^= bit(PB5);
  #endif
}

/**
 * @brief Timer2 interrupt handler
 * @details
 *
 */
ISR(TIMER2_COMPA_vect)
{
  static uint8_t counter = 0;

  switch(counter++)
  {
    case NTC_ADC_CHANNEL + 10:
      Analog::read_async(NTC_ADC_CHANNEL);
      break;

    #ifdef USE_UI_KAP
    case UI_KAP_ADC_CHANNEL + 30:
      Analog::read_async(UI_KAP_ADC_CHANNEL);
      break;
    #endif

    default:
      if(counter > 40) { counter = 0; }
  }
}
