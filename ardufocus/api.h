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

#ifndef __API_H__
#define __API_H__

#include "config.h"

#include <stdint.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "analog.h"
#include "stepper.h"
#include "dtr.h"

//TODO https://stackoverflow.com/questions/553682/when-can-i-use-a-forward-declaration

#ifdef MOTOR1_HAS_DRIVER
extern stepper* g_motor1;
#endif

#ifdef MOTOR2_HAS_DRIVER
extern stepper* g_motor2;
#endif

enum motor_t {
  MOTOR_ONE,
  MOTOR_TWO
};

class api {
  public:
     api() {;}
    ~api() {;}

    static void update_temperature() {
      Analog::read_async(NTC_ADC_CHANNEL);
    }

    static float get_temperature() {
      #ifdef START_TEMP_CONVERSION_ON_EVERY_GET
      update_temperature();
      #endif

      return util::steinhart(Analog::read(NTC_ADC_CHANNEL));
    }

    static void motor_start(const motor_t& idx) {
      switch(idx) {
        case MOTOR_ONE:
          #ifdef MOTOR1_HAS_DRIVER
          g_motor1->move();
          #endif
          break;

        case MOTOR_TWO:
          #ifdef MOTOR2_HAS_DRIVER
          g_motor2->move();
          #endif
          break;

        default: break;
      }
    }

    static void motor_stop(const motor_t& idx) {
      switch(idx) {
        case MOTOR_ONE:
          #ifdef MOTOR1_HAS_DRIVER
          g_motor1->halt();
          #endif
          break;

        case MOTOR_TWO:
          #ifdef MOTOR2_HAS_DRIVER
          g_motor2->halt();
          #endif
          break;

        default: break;
      }
    }

    static uint8_t motor_get_speed(const motor_t& idx) {
      switch(idx) {
        case MOTOR_ONE:
          #ifdef MOTOR1_HAS_DRIVER
          return g_motor1->get_speed();
          #endif
          break;

        case MOTOR_TWO:
          #ifdef MOTOR2_HAS_DRIVER
          return g_motor2->get_speed();
          #endif
          break;
      }

      return 0;
    }

    static uint8_t motor_get_mode(const motor_t& idx) {
      switch(idx) {
        case MOTOR_ONE:
          #ifdef MOTOR1_HAS_DRIVER
          return g_motor1->get_step_mode();
          #endif
          break;

        case MOTOR_TWO:
          #ifdef MOTOR2_HAS_DRIVER
          return g_motor2->get_step_mode();
          #endif
          break;
      }

      return 0;
    }

    static uint8_t motor_is_moving(const motor_t& idx) {
      switch(idx) {
        case MOTOR_ONE:
          #ifdef MOTOR1_HAS_DRIVER
          return ((g_motor1->is_moving()) ? 1 : 0);
          #endif
          break;

        case MOTOR_TWO:
          #ifdef MOTOR2_HAS_DRIVER
          return ((g_motor2->is_moving()) ? 1 : 0);
          #endif
          break;
      }

      return false;
    }

    static uint32_t motor_get_target(const motor_t& idx) {
      switch(idx) {
        case MOTOR_ONE:
          #ifdef MOTOR1_HAS_DRIVER
          return g_motor1->get_target_position();
          #endif
          break;

        case MOTOR_TWO:
          #ifdef MOTOR2_HAS_DRIVER
          return g_motor2->get_target_position();
          #endif
          break;
      }

      return 0;
    }

    static uint32_t motor_get_position(const motor_t& idx) {
      switch(idx) {
        case MOTOR_ONE:
          #ifdef MOTOR1_HAS_DRIVER
          return g_motor1->get_current_position();
          #endif
          break;

        case MOTOR_TWO:
          #ifdef MOTOR2_HAS_DRIVER
          return g_motor2->get_current_position();
          #endif
          break;

      }

      return 0;
    }

    static void motor_set_speed(const motor_t& idx, const uint32_t& value) {
      switch(idx) {
        case MOTOR_ONE:
          #ifdef MOTOR1_HAS_DRIVER
          g_motor1->set_speed(value);
          #endif
          break;

        case MOTOR_TWO:
          #ifdef MOTOR2_HAS_DRIVER
          g_motor2->set_speed(value);
          #endif
          break;

        default:
          return;
      }
    }

    static void motor_set_mode_full(const motor_t& idx) {
      switch(idx) {
        case MOTOR_ONE:
          #ifdef MOTOR1_HAS_DRIVER
          g_motor1->set_full_step();
          #endif
          break;

        case MOTOR_TWO:
          #ifdef MOTOR2_HAS_DRIVER
          g_motor2->set_full_step();
          #endif
          break;

        default:
          return;
      }
    }

    static void motor_set_mode_half(const motor_t& idx) {
      switch(idx) {
        case MOTOR_ONE:
          #ifdef MOTOR1_HAS_DRIVER
          g_motor1->set_half_step();
          #endif
          break;

        case MOTOR_TWO:
          #ifdef MOTOR2_HAS_DRIVER
          g_motor2->set_half_step();
          #endif
          break;

        default:
          return;
      }
    }

    static void motor_set_target(const motor_t& idx, const uint32_t& value) {
      switch(idx) {
        case MOTOR_ONE:
          #ifdef MOTOR1_HAS_DRIVER
          g_motor1->set_target_position(value);
          #endif
          break;

        case MOTOR_TWO:
          #ifdef MOTOR2_HAS_DRIVER
          g_motor2->set_target_position(value);
          #endif
          break;

        default:
          return;
      }
    }

    static void motor_set_position(const motor_t& idx, const uint32_t& value) {
      switch(idx) {
        case MOTOR_ONE:
          #ifdef MOTOR1_HAS_DRIVER
          g_motor1->set_current_position(value);
          #endif
          break;

        case MOTOR_TWO:
          #ifdef MOTOR2_HAS_DRIVER
          g_motor2->set_current_position(value);
          #endif
          break;

        default:
          return;
      }
    }

    #ifdef ENABLE_REMOTE_RESET
    static void system_reset(const uint32_t& wait) {
      wdt_disable();
      _delay_ms(wait);

      wdt_enable(WDTO_1S);
      while (true) { /* loop */ }
    }
    #endif

    #ifdef ENABLE_DTR_RESET
    static void    set_dtr_reset(const bool& value) { dtr_reset(value);       }
    static uint8_t get_dtr_reset()                  { return dtr_reset_get(); }
    #endif
};

#endif
