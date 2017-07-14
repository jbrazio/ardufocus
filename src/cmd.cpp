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

void cmd::process(const char* buffer)
{
  char str[6];
  memset(&str, 0, sizeof(str));

  switch (buffer[0]) {
    case 'C': // Start temperature conversion
      ADMUX  = 0x00;
      ADCSRA |= bit(ADSC) | bit(ADIE);
      break;

    case 'F':
      switch(buffer[1]) {
        case 'G': // Start a Motor 1 move
          motor1.move();
          break;

        case 'Q': // Halt motor 1 move
          motor1.halt();
          break;

        default: break;
      }
      break;

    case 'G':
      switch(buffer[1]) {
        case 'B': // Get the backlight value
          serial::print::string("00#");
          break;

        case 'C': // Get temperature coefficient
          serial::print::string("02#");
          break;

        case 'D': // Get the motor 1 speed
          sprintf(str, "%02x#", motor1.get_speed());
          serial::print::string(str);
          break;

        case 'H': // Get the motor 1 step
          sprintf(str, "%02x#", motor1.get_step_mode());
          serial::print::string(str);
          break;

        case 'I': // Get the motor 1 status
          serial::print::string((motor1.is_moving()) ? "01#" : "00#");
          break;

        case 'N': // Get the new motor 1 position
          sprintf(str, "%04x#", motor1.get_target_position());
          serial::print::string(str);
          break;

        case 'P': // Get current motor 1 positon
          sprintf(str, "%04x#", motor1.get_current_position());
          serial::print::string(str);
          break;

        case 'T': // Get the current temperature
          sprintf(str, "%04x#", static_cast<int16_t>(utils::steinhart(temp())) << 1);
          serial::print::string(str);
          break;

        case 'V': // Get firmware version
          serial::print::string(ARDUFOCUS_VERSION "#");
          break;

        default: break;
      }
      break;

    case 'P':
      switch(buffer[1]) {
        case 'B': // Adjust Blue backlight brightness
        case 'C': // Adjust LCD contrast
        case 'G': // Adjust Green backlight brightness
        case 'H': // Find home for motor
        case 'O': // Adjust temperature offset
        case 'R': // Adjust Red backlight brightness
        case 'S': // Adjust temperature scale
        case 'X': // Adjust the scale for motor 1
        case 'Y': // Adjust the scale for motor 2
          // !!! NOT IMPLEMENTED
          break;

        default: break;
      }
      break;

    case 'S':
      switch(buffer[1]) {
        case 'D': // Set the motor 1 speed
          strncpy(str, buffer + 2, 2);
          motor1.set_speed(utils::hexstr2long(str));
          break;

        case 'F': // Set motor 1 to full step
          motor1.set_full_step();
          break;

        case 'H': // Set motor 1 to half step
          motor1.set_half_step();
          break;

        case 'N': // Set the new motor 1 position
          strncpy(str, buffer + 2, 4);
          motor1.set_target_position(utils::hexstr2long(str));
          break;

        case 'P': // Set current motor 1 positon
          strncpy(str, buffer + 2, 4);
          motor1.set_current_position(utils::hexstr2long(str));
          break;

        default: break;
      }
      break;

    case '+': // Enable temperature compensation focusing
    case '-': // Disable temperature compensation focusing
      // !!! NOT IMPLEMENTED
      break;

    default:
      // do nothing
      break;
  }
}
