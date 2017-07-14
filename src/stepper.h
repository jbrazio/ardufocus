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

// total movement 37300

class stepper
{
protected:
  uint8_t mode = 0x00;
  volatile uint16_t speed = 2;
  volatile stepper_position_t position = { 0, 0, false };

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
    position.current = position.target = target;
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
    sei();
  }

  inline void tick()
  {
    if (position.moving) {
      switch(speed) {
        case 1:
          OCR1A = 0x7D; // 500 Hz
          break;

        case 2:
          OCR1A = 0xFA; // 250 Hz
          break;

        case 4:
          OCR1A = 0x1F4; // 125 Hz
          break;

        case 8:
          OCR1A = 0x3E0; // 63 Hz
          break;

        case 16:
          OCR1A = 0x7A1; // 32 Hz
          break;

        case 32:
          OCR1A = 0xF42; // 16 Hz
          break;

        default: break;
      }

      // Move the focus point out
      if (position.target > position.current) {
        ++position.current;
        step_ccw();
      }

      // Move the focus point in
      else if (position.target < position.current) {
        --position.current;
        step_cw();
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
