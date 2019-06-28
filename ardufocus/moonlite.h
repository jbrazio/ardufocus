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

#ifndef __MOONLITE_H__
#define __MOONLITE_H__

#define CMD_START_CHAR  0x3A // :
#define CMD_END_CHAR    0x23 // #
#define CMD_MAX_LEN      13u

#include "config.h"

#include <stdio.h>
#include <string.h>
#include "protocol.h"
#include "serial.h"
#include "version.h"

class moonlite: protected protocol, protected serial {
  public:
    moonlite() {
      setup();
    }

    void setup() {
      serial::setup();

      serial::write_P(PSTR("Ardufocus " ARDUFOCUS_VERSION "-" ARDUFOCUS_BRANCH " ready.\n"));
      serial::write_P(PSTR("Visit " ARDUFOCUS_URL " for updates.\n\n"));
    }

    void receive() {
      char str[CMD_MAX_LEN];
      if(serial::receive(str)) { parse(str); }
    }

    void reply(const char* str) {
      serial::write(str);
      serial::write(CMD_END_CHAR);
    }

    void reply_P(const char* str) {
      serial::write_P(str);
      serial::write(CMD_END_CHAR);
    }

    void parse(char* const str) {
      size_t offset = 0;
      motor_t motor = MOTOR_ONE;
      char buffer[CMD_MAX_LEN] = {0};

      if(str[0] == '2') {
        offset = 1;
        motor = MOTOR_TWO;
      }

      switch (str[0 + offset]) {
        case 'C':
          update_temperature();
          break;

        #ifdef ENABLE_REMOTE_RESET
        case 'Z':
          system_reset(0);
          break;
        #endif

        case 'F':
          switch(str[1] + offset) {
            case 'G':
                motor_start(motor);
              break;

            case 'Q':
                motor_stop(motor);
              break;
          } break;

        case 'G':
          switch(str[1 + offset]) {
            case 'D':
              sprintf_P(buffer, PSTR("%02X"), motor_get_speed(motor));
              break;

            case 'I':
              sprintf_P(buffer, PSTR("%02X"), motor_is_moving(motor));
              break;

            case 'N':
              #ifdef HIGH_RESOLUTION_MODE
              sprintf_P(buffer, PSTR("%08lX"), motor_get_target(motor));
              #else
              sprintf_P(buffer, PSTR("%04X"), motor_get_target(motor));
              #endif
              break;

            case 'P':
              #ifdef HIGH_RESOLUTION_MODE
              sprintf_P(buffer, PSTR("%08lX"), motor_get_position(motor));
              #else
              sprintf_P(buffer, PSTR("%04X"), motor_get_position(motor));
              #endif
              break;

            case 'T':
              #ifdef HIGH_RESOLUTION_MODE
              sprintf_P(buffer, PSTR("%08lX"), get_temperature());
              #else
              sprintf_P(buffer, PSTR("%04X"), ((int16_t)get_temperature()) << 1);
              #endif
              break;

            #ifdef ENABLE_DTR_RESET
            case 'Y':
              sprintf_P(buffer, PSTR("%02X"), get_dtr_reset());
              break;
            #endif

            default:
              reply_P(PSTR ("00"));
          }

          reply(buffer);
          break;

        case 'S':
          strncpy(buffer, str + 2 + offset, strlen(str) - (2 + offset));

          switch(str[1 + offset]) {
            case 'D':
              motor_set_speed(motor, util::hex2l(buffer));
              break;

            case 'N':
              #ifdef HIGH_RESOLUTION_MODE
              motor_set_target(motor, util::hex2ul(buffer));
              #else
              motor_set_target(motor, (uint32_t)util::hex2l(buffer));
              #endif
              break;

            case 'P':
              #ifdef HIGH_RESOLUTION_MODE
              motor_set_position(motor, util::hex2ul(buffer));
              #else
              motor_set_position(motor, (uint32_t)util::hex2l(buffer));
              #endif
              break;

            #ifdef ENABLE_DTR_RESET
            case 'Y':
              set_dtr_reset((buffer[0] == '1') ? true : false);
              break;
            #endif
          } break;

        default:
          break;
      }
    }
};

static moonlite comms;

#endif
