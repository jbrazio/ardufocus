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

#ifndef __STEPPER_H__
#define __STEPPER_H__

#include "ardufocus.h"
#include "struct.h"
#include "lookuptables.h"

class stepper
{
protected:
  uint8_t mode = 0x00;
  volatile uint16_t speed = 2;
  volatile stepper_position_t position = { false, 0, 0, 0, 0, 0, 0};

public:
  inline bool is_moving()
  {
    cli();
    const bool b = (position.moving);
    sei();
    return b;
  }

  inline void move()
  {
    cli();
    position.moving = true;
    sei();
  }

  inline uint16_t get_current_position()
  {
    return position.current;
  }

  inline void set_current_position(const uint16_t& target)
  {
    cli();
    position.current  = target;
    position.target   = target;
    sei();
  }

  inline uint16_t get_speed()
  {
    return speed;
  }

  inline void set_speed(const uint16_t& target)
  {
    cli();
    speed = target;
    sei();
  }

  inline uint16_t get_target_position()
  {
    return position.target;
  }

  inline void set_target_position(const uint16_t& target)
  {
    cli();

    position.target = target;

    #ifdef USE_LINEAR_ACCEL
      position.last = position.current;

      if (position.target > position.current) {
        position.delta = position.target - position.last;
      } else {
        position.delta = position.last - position.target;
      }

      position.accelin  = 100;

      if (position.delta < 200 ) {
        position.accelin = position.delta >> 1;
      }

      position.accelout = position.delta - position.accelin;
    #endif

    sei();
  }

  inline void tick()
  {
    if (position.moving) {
      // Set the stepping frequency
      switch(speed) {
        case 1:
          OCR1A = lookup::step_freq_table[0]; // 500 Hz
          break;

        case 2:
          OCR1A = lookup::step_freq_table[1]; // 250 Hz
          break;

        case 4:
          OCR1A = lookup::step_freq_table[2]; // 125 Hz
          break;

        case 8:
          OCR1A = lookup::step_freq_table[3]; // 63 Hz
          break;

        case 16:
          OCR1A = lookup::step_freq_table[4]; // 32 Hz
          break;

        case 32:
          OCR1A = lookup::step_freq_table[5]; // 16 Hz
          break;

        default: break;
      }

      #ifdef USE_LINEAR_ACCEL
        // Calculates the current relative position
        const uint16_t pos = (position.target < position.current)
          ? position.last - position.current
          : position.current - position.last;

        // Acceleration control
        if (pos <= position.accelin) {
          OCR1A = map(pos, 0, position.accelin, lookup::step_freq_table[5], OCR1A);
        }

        else if (pos >= position.accelout) {
          OCR1A = map(pos, position.accelout, position.delta, OCR1A, lookup::step_freq_table[5]);
        }
      #endif

      // Move the focus point out
      if (position.target > position.current) {
        ++position.current;
        #ifdef INVERT_MOTOR_DIR
          step_cw();
        #else
          step_ccw();
        #endif
      }

      // Move the focus point in
      else if (position.target < position.current) {
        --position.current;
        #ifdef INVERT_MOTOR_DIR
          step_ccw();
        #else
          step_cw();
        #endif
      }

      // Stop movement
      else { halt(); }
    }
  }

  virtual inline void halt()
  {
    cli();
    position.target = position.current;
    position.moving = false;
    sei();
  }

  virtual inline uint8_t get_step_mode() {
    return mode;
  }

  virtual inline void set_full_step() { ; }
  virtual inline void set_half_step() { ; }

  virtual inline void step_cw()  { ; }
  virtual inline void step_ccw() { ; }
};

#endif
