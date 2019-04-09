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
 * @brief Timer1 interrupt handler
 * @details
 *
 */
ISR(TIMER0_COMPA_vect)
{
  #if defined(MOTOR1_HAS_DRIVER) && defined(MOTOR2_HAS_DRIVER)
    g_motor1->tick();
    g_motor2->tick();

    if (!g_motor1->is_moving() && !g_motor2->is_moving()) {
      g_config.position_m1 = g_motor1->get_current_position();
      g_config.position_m2 = g_motor2->get_current_position();
      eeprom_save(&g_config);
    }
  #elif defined(MOTOR1_HAS_DRIVER)
    g_motor1->tick();

    if (!g_motor1->is_moving()) {
      g_config.position_m1 = g_motor1->get_current_position();
      eeprom_save(&g_config);
    }
  #elif defined(MOTOR2_HAS_DRIVER)
    g_motor2->tick();

    if (!g_motor2->is_moving()) {
      g_config.position_m2 = g_motor2->get_current_position();
      eeprom_save(&g_config);
    }
  #endif
}
